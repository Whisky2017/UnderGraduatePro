#if !defined(AFX_IP_H__3958F313_43CD_45C0_9796_947B59015E75__INCLUDED_)
#define AFX_IP_H__3958F313_43CD_45C0_9796_947B59015E75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIP dialog

class CIP : public CDialog
{
// Construction
public:
	CIP(CWnd* pParent = NULL);   // standard constructor
	CString IP;
// Dialog Data
	//{{AFX_DATA(CIP)
	enum { IDD = IDD_DIALOG1 };
	CIPAddressCtrl	m_ip;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIP)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IP_H__3958F313_43CD_45C0_9796_947B59015E75__INCLUDED_)
