// IP.cpp : implementation file
//

#include "stdafx.h"
#include "client.h"
#include "IP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIP dialog


CIP::CIP(CWnd* pParent /*=NULL*/)
	: CDialog(CIP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIP)
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIP, CDialog)
	//{{AFX_MSG_MAP(CIP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIP message handlers

void CIP::OnOK() 
{
	
	m_ip.GetWindowText(IP);
	CDialog::OnOK();
}
