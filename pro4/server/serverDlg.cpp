// serverDlg.cpp
//

#include "stdafx.h"
#include "server.h"
#include "serverDlg.h"
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

#define CERTF  "cert.cer"			//服务器证书
#define KEYF   "key.pem"			//服务器私钥
#define ROOTCERTF "root.cer"		//根证书
#define MAX_CLIENT 100				//允许链接的最大客户端数
#define WM_CLIENT_MSG WM_USER+102	//自定义的消息
SSL_CTX* ctx;						//SSL上下文句柄
descriptor_t Clients[MAX_CLIENT];	//保存客户端链接句柄的数组
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
// CServerDlg dialog

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
	m_str = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_str);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_WM_DESTROY()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CLIENT_MSG,OnClientMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
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
	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();
	SSL_METHOD *meth;
	meth = SSLv23_server_method();	//使用SSL V2或V3协议
	ctx = SSL_CTX_new (meth);		//初始化SSL上下文
	if (!ctx) 
	{
		return FALSE;
	}
	//设置服务器证书
	if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM) <= 0) 
	{
		return FALSE;
	}
	//设置服务器私钥
	if (SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM) <= 0)
	{
		return FALSE;
	}
	//检查服务器私钥和证书是否匹配
	if (!SSL_CTX_check_private_key(ctx))
	{
		return FALSE;
	}
	//初始化客户端链接结构体数组
	for(int i=0;i<MAX_CLIENT;i++)
	{
		Clients[i].fd=0;
		Clients[i].ssl=NULL;
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerDlg::OnPaint() 
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//处理客户端事务线程函数，接收客户端消息
void	client( void* void_parmint )
{
	int clientFd = (int) void_parmint;
	int	 maxFd;
	int len;
	int flag=0;
	fd_set	writeFds, readFds, excFds;
	char	buffer[8192];
	descriptor_t clientDesc;
	clientDesc.fd = clientFd;
	clientDesc.ssl = NULL;
	SSL		*ssl;
	//新建SSL连接句柄
	if((ssl = SSL_new(ctx)) == NULL)
	{
		return;
	}
	//设置SSL连接Socket句柄
	SSL_set_fd(ssl, clientFd);
	//接收SSL连接
	if(SSL_accept(ssl) <= 0)
	{
		return;
	}
	clientDesc.ssl = ssl;
	//把客户端链接句柄保存到全局变量中。
	for(int i=0;i<MAX_CLIENT;i++)
	{
		if(Clients[i].fd==0)
		{
			Clients[i].fd=clientFd;
			Clients[i].ssl=ssl;
			flag = 1;
			break;
		}
	}
	if(!flag)
	{
		//已经达到链接最大限制
		SSL_write(ssl,"服务器已满!",strlen("服务器已满!"));
		SSL_shutdown(ssl);
		SSL_free(ssl);
		return;
	}
	maxFd = clientFd;
	for(;;)
	{
		FD_ZERO(&writeFds);
		FD_ZERO(&readFds);
		FD_ZERO(&excFds);
		FD_SET(clientFd, &readFds);
		//异步的方式等待客户端数据
		int nfd = select(maxFd + 1, &readFds, &writeFds, &excFds, NULL);
		if(nfd <= 0)
		{
			if(errno == EINTR)	/* interrupted system call */
				continue;
			return;
		}
		if(FD_ISSET(clientFd, &readFds))//有客户端数据需要读取
		{
			//接收客户端数据
			len = SSL_read(clientDesc.ssl,buffer,sizeof(buffer));
			if(len <=0)
			{
				char *msg = (char *)malloc(128);
				strcpy(msg,"客户端退出!");
				SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,clientFd,(long )msg);
				return;
			}
			buffer[len]='\0';
			char *msg = (char *)malloc(len +1);
			strcpy(msg,buffer);
			//发送WM_CLIENT_MSG消息到主窗体，把接收到的消息显示到列表框
			SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,clientFd,(long )msg);
		}
	}
	return;
}
//接收连接线程函数
void AcceptThreadProc( void* void_parm )
{
	int sockFd = (int) void_parm;
	HANDLE hd;
	unsigned long idThread;
	int clientFd;
	//接收连接
	clientFd = accept(sockFd, NULL, NULL);
	if(clientFd < 0){
		if(errno == EINTR)	/* interrupted system call */
			return;
		ExitThread(0);
	}
	//接收客户端连接后，开启线程处理客户端事务即接收客户端消息
	hd=CreateThread(NULL,0,
		(LPTHREAD_START_ROUTINE)client,
		(void *)clientFd, 0 ,&idThread);
}
//启动服务线程函数
void StartServer(void* void_parm)
{
	
	WSADATA     	wsaData;
	int err;
	int listen_sd;
	
	unsigned long		idThread;
	struct sockaddr_in sa_serv;
	HANDLE		hd;
	int port = (int) void_parm;
	//初始化windows socket
	if (WSAStartup(MAKEWORD(1, 1), &wsaData))
	{
		char *msg = (char *)malloc(128);
		strcpy(msg,"初始化Socket失败!");
		SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,0,(long )msg);
		return;
	}
	//新建socket句柄
	listen_sd = socket (AF_INET, SOCK_STREAM, 0);
	//初始化sockaddr_in结构体，设置TCP协议和端口
	memset (&sa_serv, '\0', sizeof(sa_serv));
	sa_serv.sin_family      = AF_INET;
	sa_serv.sin_addr.s_addr = INADDR_ANY;
	sa_serv.sin_port        = htons (port);         
	//绑定端口
	err = bind(listen_sd, (struct sockaddr*) &sa_serv,
		sizeof (sa_serv));          
	if(err < 0)
	{
		char *msg = (char *)malloc(128);
		strcpy(msg,"绑定Socket失败!");
		SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,listen_sd,(long )msg);
		return;
	}
	//侦听，tcp连接
	err = listen (listen_sd, 5);  
	char *msg = (char *)malloc(128);
	strcpy(msg,"启动服务成功!");
	SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,listen_sd,(long )msg);
	for(;;)
	{
		//启动接收连接线程
		hd=CreateThread(NULL,0,
			(LPTHREAD_START_ROUTINE)AcceptThreadProc,
			(void *)listen_sd, 0 ,&idThread);
		//等待该线程执行完毕，开启下一个线程，继续等待接收连接。
		WaitForSingleObject(hd, INFINITE);
	 }
	
}
//启动服务
void CServerDlg::OnStart() 
{
	unsigned long		idThread;
	int port=8443;
	//创建线程，启动服务
	CreateThread(NULL,0,
		(LPTHREAD_START_ROUTINE)StartServer,
		(void *)port, 0 ,&idThread);
	  
}

//向客户端发送消息
void CServerDlg::OnSend() 
{
	UpdateData();
	//遍续所有已经链接客户端，发送消息
	for(int i=0;i<MAX_CLIENT;i++)
	{
		if(Clients[i].fd != 0)
		{
			//发送消息
			SSL_write(Clients[i].ssl,m_str.GetBuffer(0),m_str.GetLength());
			m_str.ReleaseBuffer();
		}
	}
	m_list.InsertString(0,m_str);
	m_str.Empty();
	UpdateData(FALSE);
	((CEdit *)GetDlgItem(IDC_EDIT1))->SetActiveWindow();
}

void CServerDlg::OnDestroy()
{
	WSACleanup();
	SSL_CTX_free(ctx);
	CDialog::OnDestroy();
}

void CServerDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}
//处理线程WM_CLIENT_MSG消息，显示到列表框中
LRESULT CServerDlg::OnClientMsg(WPARAM wParam,LPARAM lParam)
{
	CString msg;
	msg.Format("【Socket:%d】  %s",wParam,(char *)lParam);
	m_list.InsertString(0,msg);
	free((void*)lParam);
	return 0L;
}
