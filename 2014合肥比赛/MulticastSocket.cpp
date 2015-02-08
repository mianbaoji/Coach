// MulticastSocket.cpp : implementation file
//

#include "stdafx.h"
#include "MulticastSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMulticastSocket

CMulticastSocket::CMulticastSocket()
{
	bForceNoLoopback = FALSE;
	bDataReceived = TRUE;		/* Variable defined for this project. Not necessarily part of CMulticastSocket */

	pEdit = NULL;
	plist = NULL;
	pOldRef = NULL;
	pField = NULL;

	m_strRFIP = ".";
}

CMulticastSocket::~CMulticastSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMulticastSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CMulticastSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CMulticastSocket member functions

BOOL CMulticastSocket::CreateReceivingSocket(LPCTSTR strGroupIP, UINT nGroupPort)
{
	/* Create socket for receiving packets from multicast group */
	if(!Create(nGroupPort, SOCK_DGRAM, FD_READ,NULL))
		return FALSE;

	BOOL bMultipleApps = TRUE;		/* allow reuse of local port if needed */
	SetSockOpt(SO_REUSEADDR, (void*)&bMultipleApps, sizeof(BOOL), SOL_SOCKET);

	/* Fill m_saHostGroup_in for sending datagrams */
	memset(&m_saHostGroup, 0, sizeof(m_saHostGroup));
	m_saHostGroup.sin_family = AF_INET;
	m_saHostGroup.sin_addr.s_addr = inet_addr(strGroupIP);
	m_saHostGroup.sin_port = htons((USHORT)nGroupPort);

	/* Join the multicast group */
	m_mrMReq.imr_multiaddr.s_addr = inet_addr(strGroupIP);	/* group addr */ 
	m_mrMReq.imr_interface.s_addr = htons(INADDR_ANY);		/* use default */ 
	if(setsockopt(m_hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char FAR *)&m_mrMReq, sizeof(m_mrMReq)) < 0)
		return FALSE;

	return TRUE;
}

BOOL CMulticastSocket::CreateSendingSocket(UINT nTTL, BOOL bLoopBack)
{
	if(!m_SendSocket.Create(0, SOCK_DGRAM, 0))		// Create an unconnected UDP socket
		return FALSE;

	if(!SetTTL(nTTL))								// Set Time to Live as specified by user
		AfxMessageBox("Warning! Error Setting TTL");

	SetLoopBack(bLoopBack);							// Enable/Disable Loopback

	return TRUE;
}

BOOL CMulticastSocket::SetTTL(UINT nTTL)
{
	/* Set Time to Live to parameter TTL */
	if(m_SendSocket.SetSockOpt(IP_MULTICAST_TTL, &nTTL, sizeof(int), IPPROTO_IP) == 0)
		return FALSE;		/* Error Setting TTL */
	else
		return TRUE;		/* else TTL set successfully */
}

void CMulticastSocket::SetLoopBack(BOOL bLoop)
{
	/* Set LOOPBACK option to TRUE OR FALSE according to IsLoop parameter */
	int nLoopBack = (int)bLoop;
	if(m_SendSocket.SetSockOpt(IP_MULTICAST_LOOP, &nLoopBack, sizeof(int), IPPROTO_IP) == 0)
	{
		if(!bLoop)						/* if required to stop loopback */
		{
			bForceNoLoopback = TRUE;		/* Internally making a note that loopback has to be disabled forcefilly */

			// Get IP/Port for send socket in order to disable loopback forcefully */
			char localHost[255];
			gethostname(localHost, 255);
			struct hostent *host = gethostbyname(localHost);	/* Get local host IP */
			m_strLocalIP = inet_ntoa (*(struct in_addr*)*host->h_addr_list);
			CString Dummy;			// Dummy string to be passed to the GetSockName function
			m_SendSocket.GetSockName(Dummy, m_nLocalPort);		/* Get Port Number for Sending Port */
		}
	}
}

void CMulticastSocket::OnReceive(int nErrorCode)
{
	int nError = ReceiveFrom (m_strBuffer, 32000, m_strSendersIP, m_nSendersPort);
	if(nError == SOCKET_ERROR)
		AfxMessageBox("Error receiving data from the host group");
	else
	{
		if (!bForceNoLoopback || (bForceNoLoopback && !(m_strSendersIP == m_strLocalIP && m_nSendersPort == m_nLocalPort)))
		{
			// 1. If loopbackback is not to be forced then interface handles the loopback itself
			// 2. If you have to loopback and SOCKOPT LOOPBACK fails, no problem, interfaces loopback by default
			// 3. If you have to stop loopback and SOCKOPT LOOPBACK fails, ignore messages coming from your own sending socket

			// TODO : Add your code for here. The packet received is in m_strBuffer
			bDataReceived = TRUE;	/* Making note that a message has arrived */

			//////////////////////////////////////////////////////////////////////////
			//直接显示内容
			//plist->AddString(m_strBuffer);

			//////////////////////////////////////////////////////////////////////////
			if (m_strSendersIP.Find(m_strRFIP) >= 0)
			{
				//解析
				static int nTextLength;
				m_strBuffer[nError] = '\0';

				XmlParse(m_strBuffer);

				nTextLength = pEdit->GetWindowTextLength();
				pEdit->SetSel(nTextLength, nTextLength);
				pEdit->ReplaceSel(m_strBuffer);

				nTextLength += nError+1;
				pEdit->SetSel(nTextLength, nTextLength);
				pEdit->ReplaceSel("\r\n\r\n");		
			}
			else
			{
				CString strErr;
				strErr.Format("指令来自可疑IP %s",m_strSendersIP);
				plist->AddString(strErr);
				plist->SetCurSel(plist->GetCount()-1);
			}
			//////////////////////////////////////////////////////////////////////////
			//纯解析
// 			static int nTextLength;
// 			m_strBuffer[nError] = '\0';
// 			
// 			XmlParse(m_strBuffer);
// 			
// 			nTextLength = pEdit->GetWindowTextLength();
// 			pEdit->SetSel(nTextLength, nTextLength);
// 			pEdit->ReplaceSel(m_strBuffer);
// 			
// 			nTextLength += nError+1;
// 			pEdit->SetSel(nTextLength, nTextLength);
// 			pEdit->ReplaceSel("\r\n\r\n");		
			//////////////////////////////////////////////////////////////////////////
		}
	}

	CAsyncSocket::OnReceive(nErrorCode); 
}

BOOL CMulticastSocket::LeaveGroup()
{
	if(setsockopt (m_hSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char FAR *)&m_mrMReq, sizeof(m_mrMReq)) < 0)
		return FALSE;

	m_SendSocket.Close();		// Close sending socket
	Close();					// Close receving socket
	return TRUE;
}

BOOL CMulticastSocket::SendTo(const void* strMessage, int nSize)
{
	if(m_SendSocket.SendTo(strMessage, nSize, (SOCKADDR*)&m_saHostGroup, sizeof(SOCKADDR), 0) == SOCKET_ERROR)
		return FALSE;
	else
		return TRUE;
}

BOOL CMulticastSocket::JoinGroup(CString GroupIP, UINT nGroupPort, UINT nTTL, BOOL bLoopback)
{
		if(!CreateReceivingSocket(GroupIP, nGroupPort))		/* Create Socket for receiving and join the host group */
		{
			m_SendSocket.Close();		// Close sending socket
			Close();					// Close receving socket
			return FALSE;
		}
		if(!CreateSendingSocket(nTTL, bLoopback))			/* Create Socket for sending */
		{
			m_SendSocket.Close();		// Close sending socket
			Close();					// Close receving socket
			return FALSE;
		}

		if (plist != NULL)
		{
			CString strInfo;
			strInfo.Format("Start multicast for protocol 2010 on port: %d",nGroupPort);
			plist->AddString(strInfo);
		}

		return TRUE;
}

void CMulticastSocket::XmlParse(char *inTxet)
{
	CString cmmt(inTxet);
	CString strHeader;
	CString strTmp;
	CString strTeam;
	CString strID;
	int res;
	int m_cmdIndex;
	
	TiXmlDocument m_xmlDoc;
	m_xmlDoc.Parse(inTxet);

	TiXmlNode* node = 0;
	TiXmlElement* RootElement = 0;

	TiXmlElement* pElem;
	TiXmlHandle docHandle( &m_xmlDoc );
	TiXmlAttribute* pAttr;

	RootElement = m_xmlDoc.RootElement();
	if (RootElement == NULL)
	{
		return;
	}

	//第一级
	int cmp = strcmp("RefboxEvent",RootElement->Value());
	if (cmp == 0)
	{
		/**************************第二级***************************************/
		//一、试探GameInfo
		pElem = docHandle.FirstChild("RefboxEvent").FirstChild("GameInfo").ToElement();
		if (pElem)
		{
			pAttr = pElem->FirstAttribute();
			//id
			res = pElem->QueryIntAttribute("id",&m_cmdIndex);

			//timeSecondHalf
			
			pAttr = pAttr->Next();
			//timeHalfTime
			
			pAttr = pAttr->Next();
			//timeFirstHalf
			
			pAttr = pAttr->Next();
			//time	
			strHeader.Format("[%s]RE(%d)", pAttr->Value(),m_cmdIndex);
			
			pAttr = pAttr->Next();
			//stage

			strTmp.Format("%s",pElem->Value());
			/*/////////////////////////////////////////////////////////////////////////
			TiXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).FirstChild( "Child" ).ToElement();
			
			for( child; child; child=child->NextSiblingElement() )
			{
				// do something
			}
			/////////////////////////////////////////////////////////////////////////*/
		
			//cmmt.Format("GameInfo");
			cmmt = strTmp + strHeader;	
			cmmt = cmmt + " - ("+m_strSendersIP + ")";
			plist->AddString(cmmt);
			plist->SetCurSel(plist->GetCount()-1);

			if (pField != NULL)
			{
				pField->bCntRefbox = true;
			}
		}
		
		
		//二、试探Referee
		pElem = docHandle.FirstChild("RefboxEvent").FirstChild("Referee").ToElement();
		if (pElem)
		{
			//strTmp.Format("%s",pElem->Value());
			//time
			pAttr = pElem->FirstAttribute();
			//id
			res = pElem->QueryIntAttribute("id",&m_cmdIndex);	
			strHeader.Format("[%s]RE(%d)", pAttr->Value(),m_cmdIndex);

			//cmd
			pElem=pElem->FirstChild()->ToElement();
			if (pElem)
			{
				if (pField != NULL)
				{
					pField->bCntRefbox = true;
				}
				strTmp.Format("%s",pElem->Value());

				//单指令
				//start
				if (strTmp == "GameStart")
				{
					OldRefClient(COMM_START);
					cmmt = strTmp + " " + strHeader;
				}

				//stop
				if (strTmp == "GameStop")
				{
					OldRefClient(COMM_STOP);
					cmmt = strTmp + " " + strHeader;
				}

				//Cancel
				if (strTmp == "Cancel")
				{
					OldRefClient(COMM_CANCEL);
					cmmt = strTmp + " " + strHeader;
				}
				
				//DroppedBall
				if (strTmp == "DroppedBall")
				{
					OldRefClient(COMM_DROPPED_BALL);
					cmmt = strTmp + " " + strHeader;
				}	

				//EndHalf
				if (strTmp == "StageChange")
				{		
					cmmt = strTmp + " " + strHeader;
				}	
				
				//Parking
				if (strTmp == "Parking")
				{		
					cmmt = strTmp + " " + strHeader;
				}	
				
				//对象指令
				//kickoff
				if (strTmp == "KickOff")
				{
					pAttr = pElem->FirstAttribute();
					if (pAttr)
					{
						strTeam.Format("%s",pAttr->Value());
						if (strTeam == "Cyan")
						{
							OldRefClient(COMM_KICKOFF_CYAN);
						}
						else
						{
							OldRefClient(COMM_KICKOFF_MAGENTA);
						}
						cmmt.Format("%s %s",strTmp,strTeam);
						cmmt = cmmt + " " + strHeader;
					}
				}

				
				//freekick
				if (strTmp == "FreeKick")
				{
					pAttr = pElem->FirstAttribute();
					if (pAttr)
					{
						strTeam.Format("%s",pAttr->Value());
						if (strTeam == "Cyan")
						{
							OldRefClient(COMM_FREEKICK_CYAN);
						}
						else
						{
							OldRefClient(COMM_FREEKICK_MAGENTA);
						}
						cmmt.Format("%s %s",strTmp,pAttr->Value());
						cmmt = cmmt + " " + strHeader;
					}
				}
				
				//GoalKick
				if (strTmp == "GoalKick")
				{
					pAttr = pElem->FirstAttribute();
					if (pAttr)
					{
						strTeam.Format("%s",pAttr->Value());
						if (strTeam == "Cyan")
						{
							OldRefClient(COMM_GOALKICK_CYAN);
						}
						else
						{
							OldRefClient(COMM_GOALKICK_MAGENTA);
						}
						cmmt.Format("%s %s",strTmp,pAttr->Value());
						cmmt = cmmt + " " + strHeader;
					}
				}
				
				//Throwin
				if (strTmp == "ThrowIn")
				{
					pAttr = pElem->FirstAttribute();
					if (pAttr)
					{
						strTeam.Format("%s",pAttr->Value());
						if (strTeam == "Cyan")
						{
							OldRefClient(COMM_THROWIN_CYAN);
						}
						else
						{
							OldRefClient(COMM_THROWIN_MAGENTA);
						}
						cmmt.Format("%s %s",strTmp,pAttr->Value());
						cmmt = cmmt + " " + strHeader;
					}
				}
				
				//CornerKick
				if (strTmp == "Corner")
				{
					pAttr = pElem->FirstAttribute();
					if (pAttr)
					{
						strTeam.Format("%s",pAttr->Value());
						if (strTeam == "Cyan")
						{
							OldRefClient(COMM_CORNER_CYAN);
						}
						else
						{
							OldRefClient(COMM_CORNER_MAGENTA);
						}
						cmmt.Format("%s %s",strTmp,pAttr->Value());
						cmmt = cmmt + " " + strHeader;
					}
				}

				//Penalty
				if (strTmp == "Penalty")
				{
					pAttr = pElem->FirstAttribute();
					if (pAttr)
					{
						strTeam.Format("%s",pAttr->Value());
						if (strTeam == "Cyan")
						{
							OldRefClient(COMM_PENALTY_CYAN);
						}
						else
						{
							OldRefClient(COMM_PENALTY_MAGENTA);
						}
						cmmt.Format("%s %s",strTmp,pAttr->Value());
						cmmt = cmmt + " " + strHeader;
					}
				}	

				//长指令
				/*/PlayerOut
				if (strTmp == "PlayerOut")
				{
					bool flag = true;
					cmmt= strTmp;
					pAttr = pElem->FirstAttribute();
					//team
					if (pAttr)
					{
						strTeam .Format("%s",pAttr->Value());
						strTmp .Format(" %s",pAttr->Value());
						cmmt += strTmp;
						pAttr = pAttr->Next();
					}

					//reason
					if (pAttr)
					{
 						strTmp .Format("%s",pAttr->Value());
						if (strTmp == "outSanctionRestOfGame")
						{
							flag = false;
						}
						pAttr = pAttr->Next();
					}

					//id
					if (pAttr)
					{
						strID.Format("%s",pAttr->Value());
						strTmp .Format(" Player%s",pAttr->Value());
						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
	
					//sec
					if (pAttr && flag)
					{
						if (pOldRef != NULL)
						{
							pOldRef->PlayerOut(strTeam,atoi(strID),atoi(pAttr->Value()));
						}
						strTmp .Format(" %ssec",pAttr->Value());
						cmmt += strTmp;
					}
					
					cmmt = strHeader + cmmt;
				}*/	

				/*/PlayerIn
				if (strTmp == "PlayerIn")
				{
					cmmt= strTmp;
					pAttr = pElem->FirstAttribute();
					//team
					if (pAttr)
					{
						strTeam.Format("%s",pAttr->Value());
						strTmp .Format(" %s",pAttr->Value());
						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
					
					//reason
					if (pAttr)
					{
// 						strTmp .Format(" %s",pAttr->Value());
// 						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
					
					//id
					if (pAttr)
					{
						if (pOldRef != NULL)
						{
							pOldRef->PlayerIn(strTeam,atoi(pAttr->Value()));
						}
						strTmp .Format(" Player%s",pAttr->Value());
						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
					
					cmmt = strHeader + cmmt;
				}*/	

				//Substitution
				if (strTmp == "Substitution")
				{
					cmmt= strTmp;
					pAttr = pElem->FirstAttribute();
					//team
					if (pAttr)
					{
						strTeam.Format("%s",pAttr->Value());
						strTmp .Format(" %s",pAttr->Value());
						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
					
					//Player out
					if (pAttr)
					{
						strID.Format("%s",pAttr->Value());
						strTmp .Format(" Out:P%s",pAttr->Value());
						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
					
					//Player In
					if (pAttr)
					{
						if (pOldRef != NULL)
						{
							pOldRef->PlayerSubstitution(strTeam,atoi(pAttr->Value()),atoi(strID));
						}
						strTmp .Format(" In:P%s",pAttr->Value());
						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
					
					cmmt = strHeader + cmmt;
				}

				//GoalAwarded
				if (strTmp == "GoalAwarded")
				{
					//time="08:34" team="Magenta" stage="firstHalf" player="3" own="false
					cmmt= strTmp;
					pAttr = pElem->FirstAttribute();
					//time
					if (pAttr)
					{
// 						strTmp .Format(" %s",pAttr->Value());
// 						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
					
					//team
					if (pAttr)
					{
						strTmp .Format(" %s",pAttr->Value());
						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
					
					//stage
					if (pAttr)
					{
// 						strTmp .Format(" %s",pAttr->Value());
// 						cmmt += strTmp;
						pAttr = pAttr->Next();
					}

					//player
					if (pAttr)
					{
						strTmp .Format(" Player%s",pAttr->Value());
						cmmt += strTmp;
						pAttr = pAttr->Next();
					}

					//own
					if (pAttr)
					{
// 						strTmp .Format(" own%s",pAttr->Value());
// 						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
					
					cmmt = strHeader + cmmt;
				}	

				//CardAwarded
				if (strTmp == "CardAwarded")
				{
					//<CardAwarded time="08:34" team="Magenta" stage="firstHalf" player="7" number="1" color="yellow"/>
					cmmt= strTmp;
					pAttr = pElem->FirstAttribute();
					//time
					if (pAttr)
					{
// 						strTmp .Format(" %s",pAttr->Value());
// 						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
					
					//team
					if (pAttr)
					{
						strTmp .Format(" %s",pAttr->Value());
						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
					
					//stage
					if (pAttr)
					{
						// 						strTmp .Format(" %s",pAttr->Value());
						// 						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
					
					//player
					if (pAttr)
					{
						strTmp .Format(" Player%s",pAttr->Value());
						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
					
					//number
					if (pAttr)
					{
						pAttr = pAttr->Next();
					}

					//color	
					if (pAttr)
					{
						strTmp .Format(" %s",pAttr->Value());
						cmmt += strTmp;
						pAttr = pAttr->Next();
					}
					
					cmmt = strHeader + cmmt;
				}
				
				cmmt = cmmt + " - ("+m_strSendersIP + ")";
				plist->AddString(cmmt);
				plist->SetCurSel(plist->GetCount()-1);
			}		
		}
		/**************************第二级***************************************/
	}
}

void CMulticastSocket::OldRefClient(char inCmd)
{
	if (pOldRef != NULL)
	{
		pOldRef->Parse(inCmd);
	}
}

BOOL CMulticastSocket::Create(UINT nSocketPort, int nSocketType, long lEvent, LPCTSTR lpszSocketAddress)
{
	if (Socket(nSocketType, lEvent))
	{
		BOOL bMultipleApps = TRUE;	 
		SetSockOpt(SO_REUSEADDR, (void*)&bMultipleApps, sizeof(BOOL), SOL_SOCKET);
		
		if (Bind(nSocketPort,lpszSocketAddress))
		{
			return TRUE;
		}
		int nResult = GetLastError();
		Close();
		WSASetLastError(nResult);
	}
	return FALSE;
}

void CMulticastSocket::SetRefBoxIP(CString inRFIP)
{
	m_strRFIP = inRFIP;
}
