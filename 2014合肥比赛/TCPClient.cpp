// TCPClient.cpp: implementation of the CTCPClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TCPClient.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

UINT TCPReceiveThread(LPVOID pParam)
{
	CTCPClient *pClient =(CTCPClient*)pParam;

	SOCKET cntskt = pClient->cntskt;

	//����һ����������������
	char *buffer=new char[1024];
	while (1)
	{
		//�߳̽�����������������ֱ���յ�����
		int irecv = recv(cntskt,buffer,1024,0);
		//�������ӶϿ�������ѭ��
		if (irecv <= 0)
		{
			break;
		}

		pClient->Received(buffer,irecv);
	}
	pClient->printf("�� "+pClient->m_strDesIP+" �����ӶϿ���");
	//release resource
	delete[] buffer;
	closesocket(cntskt);
	pClient->m_bOnline = false;
	pClient->isDisconnected();

	return 0;
}

//�����̺߳�����
UINT TCPConnectThread(LPVOID pParam)
{
	CTCPClient* pclient = (CTCPClient*) pParam;
	pclient->cntskt = socket(AF_INET,SOCK_STREAM,0);

	if (pclient->cntskt == INVALID_SOCKET)
	{
		pclient->ConnectError();
		return 0L;
	}
	pclient->m_bOnline = true;
	struct sockaddr_in serversocketaddr;
	serversocketaddr.sin_family = AF_INET;
	serversocketaddr.sin_addr.S_un.S_addr = inet_addr(pclient->m_DesIP);
	serversocketaddr.sin_port = htons(pclient->m_nDesPort);

	//����һ��ip�ַ����������ʾ��
	pclient->m_strDesIP.Format("%d.%d.%d.%d",
			serversocketaddr.sin_addr.S_un.S_un_b.s_b1, 
			serversocketaddr.sin_addr.S_un.S_un_b.s_b2,
			serversocketaddr.sin_addr.S_un.S_un_b.s_b3,
			serversocketaddr.sin_addr.S_un.S_un_b.s_b4);

	//���ӷ�����
	if (connect(pclient->cntskt,(struct sockaddr *)&serversocketaddr,sizeof(serversocketaddr)) != SOCKET_ERROR)
	{
		//�����ӿͻ�����Ϣ��ӵ�ͨѶ����Ŀͻ����б���
		if (pclient->m_pShowList != NULL)
		{
			CString msg;
			msg.Format("�ɹ����ӵ�  %s",pclient->m_strDesIP);
			pclient->m_pShowList->AddString(msg);
			pclient->m_pShowList->SetCurSel(pclient->m_pShowList->GetCount()-1);
		}
		pclient->ConnectSucceed();
		//��ʼ����
		AfxBeginThread(TCPReceiveThread,pParam);
		return TRUE;
	}
	pclient->m_bOnline = false;
	pclient->ConnectError();
	return FALSE;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTCPClient::CTCPClient()
{
	m_pShowList = NULL;

	m_bOnline = false;
}

CTCPClient::~CTCPClient()
{
	if (cntskt != INVALID_SOCKET)
	{
		closesocket(cntskt);
	}
}

BOOL CTCPClient::ConnectTo(CString inDesIP, int inDesPort)
{
	if (inDesPort == m_nDesPort && inDesIP.Find(m_strDesIP)!= -1 && m_bOnline == true)
	{
		return FALSE;
	}

	//��ʼ��
	m_strDesIP = inDesIP;
	strcpy(m_DesIP,m_strDesIP.GetBuffer(m_strDesIP.GetLength()));
	m_nDesPort = inDesPort;
	m_strDesIP.ReleaseBuffer();

	AfxBeginThread(TCPConnectThread,this);
	return TRUE;
}

void CTCPClient::ConnectError()
{
	CString str(" ����ʧ��...");
	str = m_strDesIP+str;
	printf(str);
}

void CTCPClient::ConnectSucceed()
{
	
}

void CTCPClient::Received(void *inbuf, int inlen)
{
	
}

void CTCPClient::printf(CString instr)
{
	if (m_pShowList != NULL)
	{
		m_pShowList->AddString(instr);
		m_pShowList->SetCurSel(m_pShowList->GetCount()-1);
	}
}

void CTCPClient::Send(char *inbuf, int inLen)
{
	if (m_bOnline == true)
	{
		send(cntskt,inbuf,inLen,0);
	}
}

void CTCPClient::isDisconnected()
{

}
