// serverDlg.h : header file
//

#if !defined(AFX_SERVERDLG_H__1F8E13EA_0670_47CA_A053_C2B7E98ACAFA__INCLUDED_)
#define AFX_SERVERDLG_H__1F8E13EA_0670_47CA_A053_C2B7E98ACAFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "openssl/ssl.h"
/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog
typedef struct descriptor{
	int	fd;
	SSL	*ssl;
}descriptor_t;

class CServerDlg : public CDialog
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DIALOG };
	CListBox	m_list;
	CString	m_str;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	LRESULT OnClientMsg(WPARAM wParam,LPARAM lParam);
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnSend();
	afx_msg void OnDestroy();
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__1F8E13EA_0670_47CA_A053_C2B7E98ACAFA__INCLUDED_)
