// clientDlg.cpp 
//

#include "stdafx.h"
#include "client.h"
#include "clientDlg.h"
#include "IP.h"

#include <windows.h>
#include <winsock.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_CLIENT_MSG WM_USER+102
typedef struct descriptor{
	int	fd;
	SSL	*ssl;
}descriptor_t;

SSL_CTX* ctx;
descriptor_t client;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientDlg)
	m_str = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_str);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	//{{AFX_MSG_MAP(CClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DISCONNECT, OnDisconnect)
	//}}AFX_MSG_MAP
ON_MESSAGE(WM_CLIENT_MSG,OnClientMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//初始化OpenSSL环境
	client.fd=0;
	client.ssl=NULL;
	SSLeay_add_ssl_algorithms();
	SSL_load_error_strings();
	ctx = SSL_CTX_new (SSLv23_client_method());//设置SSL协议版本
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//连接服务器端并接收服务器消息的线程函数
void ClientThreadProc(void* void_parm)
{
	WSADATA     	wsaData;
	int err;
	int sd;
	struct sockaddr_in sa;
	SSL*     ssl;
	X509*    server_cert;
	char*    str;
	char     buffer [8912];
	int	 maxFd;
	fd_set	writeFds, readFds, excFds;
	DWORD dwIP;
	dwIP = (DWORD)void_parm;
	//初始化windows socket
	if (WSAStartup(MAKEWORD(1, 1), &wsaData))
	{
		return;
	}
	//新建socket
	sd = socket (AF_INET, SOCK_STREAM, 0);
	memset (&sa, '\0', sizeof(sa));
	//设置服务端IP地址、和端口
	sa.sin_family      = AF_INET;
	sa.sin_addr.s_addr = dwIP; 
	sa.sin_port        = htons(8443); 
	//连接服务器
	err = connect(sd, (struct sockaddr*) &sa,
		sizeof(sa));   
	if(err < 0)
	{
		char *msg = (char *)malloc(128);
		strcpy(msg,"连接服务器失败!");
		SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,sd,(long )msg);		
		return;
	}
	//新建SSL句柄
	ssl = SSL_new (ctx);   
    //设置socket句柄到SSL句柄
	SSL_set_fd (ssl, sd);
	//SSL连接
	err = SSL_connect (ssl);
	if(err != 1)
	{
		char *msg = (char *)malloc(128);
		strcpy(msg,"连接服务器失败!");
		SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,sd,(long )msg);		
		return;
	}
	server_cert = SSL_get_peer_certificate (ssl); 
    str = X509_NAME_oneline (X509_get_subject_name (server_cert),0,0);
	OPENSSL_free (str);
	str = X509_NAME_oneline (X509_get_issuer_name  (server_cert),0,0);
	OPENSSL_free (str);
	X509_free (server_cert);
	printf ("SSL connection using %s\n", SSL_get_cipher (ssl));
	maxFd = sd;
	client.fd=sd;
	client.ssl=ssl;
	char *msg = (char *)malloc(128);
	strcpy(msg,"连接服务器成功!");
	SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,sd,(long )msg);
	
	for(;;)
	{
		FD_ZERO(&writeFds);
		FD_ZERO(&readFds);
		FD_ZERO(&excFds);
		FD_SET(sd, &readFds);
		//select 异步等待服务器
		int nfd = select(maxFd + 1, &readFds, &writeFds, &excFds, NULL);
		if(nfd <= 0)
		{
			if(errno == EINTR)	/* interrupted system call */
				continue;
			return;
		}
		if(FD_ISSET(sd, &readFds))
		{
			//接收服务器消息
			int len = SSL_read(ssl,buffer,sizeof(buffer));
			if(len <=0)
			{
				char *msg = (char *)malloc(128);
				strcpy(msg,"服务器退出!");
				SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,sd,(long )msg);
				return;
			}
			buffer[len]='\0';
			char *msg = (char *)malloc(len +1);
			strcpy(msg,buffer);
			//通过WM_CLIENT_MSG消息，发送到主窗体
			SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,sd,(long )msg);
		}
	}
	return;
}
//链接服务器
void CClientDlg::OnConnect() 
{
		
	unsigned long		idThread;
	DWORD dwIP=0;
	if(client.fd==0)//判断是否已经连接
	{
		CIP ipaddr;//让用户输入客户端IP
		if(ipaddr.DoModal() == IDOK)
		{
			
			dwIP=inet_addr(ipaddr.IP.GetBuffer(0));
		}
		//创建线程，连接服务器，并接收服务器端消息
		CreateThread(NULL,0,
			(LPTHREAD_START_ROUTINE)ClientThreadProc,
			(void *)dwIP, 0 ,&idThread);
	}
	else
	{
		m_list.InsertString(0,"已经连接!");
	}
}
//发送消息到服务器
void CClientDlg::OnSend() 
{
	UpdateData();
	if(client.fd !=0)
	{
		//发送消息
		SSL_write(client.ssl,m_str.GetBuffer(0),m_str.GetLength());
		m_str.ReleaseBuffer();
	}
	else
	{
		return;
	}
	//插入发送的消息的列表框
	m_list.InsertString(0,m_str);
	m_str.Empty();
	UpdateData(FALSE);
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetActiveWindow();	
}
//处理WM_CLIENT_MSG消息
LRESULT CClientDlg::OnClientMsg(WPARAM wParam,LPARAM lParam) 
{
	
	CString msg;
	msg.Format("【Socket:%d】  %s",wParam,(char *)lParam);
	m_list.InsertString(0,msg);
	free((void*)lParam);
	return 0L;
}

void CClientDlg::OnDestroy() 
{

	WSACleanup();
	SSL_CTX_free(ctx);
	CDialog::OnDestroy();
}
//断开链接
void CClientDlg::OnDisconnect() 
{
	if(client.fd !=0)
	{
		closesocket(client.fd);
		SSL_shutdown(client.ssl);
		SSL_free(client.ssl);
		client.ssl=NULL;
		client.fd=0;
		m_list.InsertString(0,"已断开和服务器的链接!");
	}
}
