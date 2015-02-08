// ITactic.h: interface for the ITactic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITACTIC_H__21AB9A85_6B74_4927_87E2_C05A4602C108__INCLUDED_)
#define AFX_ITACTIC_H__21AB9A85_6B74_4927_87E2_C05A4602C108__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MSG_Control.h"

class ITactic  
{
public:
	MSG_Control m_ctrl[7];
	ITactic();
	virtual ~ITactic();

};

#endif // !defined(AFX_ITACTIC_H__21AB9A85_6B74_4927_87E2_C05A4602C108__INCLUDED_)
