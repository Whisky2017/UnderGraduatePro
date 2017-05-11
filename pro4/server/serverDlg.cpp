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

#define CERTF  "cert.cer"			//������֤��
#define KEYF   "key.pem"			//������˽Կ
#define ROOTCERTF "root.cer"		//��֤��
#define MAX_CLIENT 100				//�������ӵ����ͻ�����
#define WM_CLIENT_MSG WM_USER+102	//�Զ������Ϣ
SSL_CTX* ctx;						//SSL�����ľ��
descriptor_t Clients[MAX_CLIENT];	//����ͻ������Ӿ��������
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
	//��ʼ��OpenSSL����
	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();
	SSL_METHOD *meth;
	meth = SSLv23_server_method();	//ʹ��SSL V2��V3Э��
	ctx = SSL_CTX_new (meth);		//��ʼ��SSL������
	if (!ctx) 
	{
		return FALSE;
	}
	//���÷�����֤��
	if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM) <= 0) 
	{
		return FALSE;
	}
	//���÷�����˽Կ
	if (SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM) <= 0)
	{
		return FALSE;
	}
	//��������˽Կ��֤���Ƿ�ƥ��
	if (!SSL_CTX_check_private_key(ctx))
	{
		return FALSE;
	}
	//��ʼ���ͻ������ӽṹ������
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
//����ͻ��������̺߳��������տͻ�����Ϣ
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
	//�½�SSL���Ӿ��
	if((ssl = SSL_new(ctx)) == NULL)
	{
		return;
	}
	//����SSL����Socket���
	SSL_set_fd(ssl, clientFd);
	//����SSL����
	if(SSL_accept(ssl) <= 0)
	{
		return;
	}
	clientDesc.ssl = ssl;
	//�ѿͻ������Ӿ�����浽ȫ�ֱ����С�
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
		//�Ѿ��ﵽ�����������
		SSL_write(ssl,"����������!",strlen("����������!"));
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
		//�첽�ķ�ʽ�ȴ��ͻ�������
		int nfd = select(maxFd + 1, &readFds, &writeFds, &excFds, NULL);
		if(nfd <= 0)
		{
			if(errno == EINTR)	/* interrupted system call */
				continue;
			return;
		}
		if(FD_ISSET(clientFd, &readFds))//�пͻ���������Ҫ��ȡ
		{
			//���տͻ�������
			len = SSL_read(clientDesc.ssl,buffer,sizeof(buffer));
			if(len <=0)
			{
				char *msg = (char *)malloc(128);
				strcpy(msg,"�ͻ����˳�!");
				SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,clientFd,(long )msg);
				return;
			}
			buffer[len]='\0';
			char *msg = (char *)malloc(len +1);
			strcpy(msg,buffer);
			//����WM_CLIENT_MSG��Ϣ�������壬�ѽ��յ�����Ϣ��ʾ���б��
			SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,clientFd,(long )msg);
		}
	}
	return;
}
//���������̺߳���
void AcceptThreadProc( void* void_parm )
{
	int sockFd = (int) void_parm;
	HANDLE hd;
	unsigned long idThread;
	int clientFd;
	//��������
	clientFd = accept(sockFd, NULL, NULL);
	if(clientFd < 0){
		if(errno == EINTR)	/* interrupted system call */
			return;
		ExitThread(0);
	}
	//���տͻ������Ӻ󣬿����̴߳���ͻ������񼴽��տͻ�����Ϣ
	hd=CreateThread(NULL,0,
		(LPTHREAD_START_ROUTINE)client,
		(void *)clientFd, 0 ,&idThread);
}
//���������̺߳���
void StartServer(void* void_parm)
{
	
	WSADATA     	wsaData;
	int err;
	int listen_sd;
	
	unsigned long		idThread;
	struct sockaddr_in sa_serv;
	HANDLE		hd;
	int port = (int) void_parm;
	//��ʼ��windows socket
	if (WSAStartup(MAKEWORD(1, 1), &wsaData))
	{
		char *msg = (char *)malloc(128);
		strcpy(msg,"��ʼ��Socketʧ��!");
		SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,0,(long )msg);
		return;
	}
	//�½�socket���
	listen_sd = socket (AF_INET, SOCK_STREAM, 0);
	//��ʼ��sockaddr_in�ṹ�壬����TCPЭ��Ͷ˿�
	memset (&sa_serv, '\0', sizeof(sa_serv));
	sa_serv.sin_family      = AF_INET;
	sa_serv.sin_addr.s_addr = INADDR_ANY;
	sa_serv.sin_port        = htons (port);         
	//�󶨶˿�
	err = bind(listen_sd, (struct sockaddr*) &sa_serv,
		sizeof (sa_serv));          
	if(err < 0)
	{
		char *msg = (char *)malloc(128);
		strcpy(msg,"��Socketʧ��!");
		SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,listen_sd,(long )msg);
		return;
	}
	//������tcp����
	err = listen (listen_sd, 5);  
	char *msg = (char *)malloc(128);
	strcpy(msg,"��������ɹ�!");
	SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_CLIENT_MSG,listen_sd,(long )msg);
	for(;;)
	{
		//�������������߳�
		hd=CreateThread(NULL,0,
			(LPTHREAD_START_ROUTINE)AcceptThreadProc,
			(void *)listen_sd, 0 ,&idThread);
		//�ȴ����߳�ִ����ϣ�������һ���̣߳������ȴ��������ӡ�
		WaitForSingleObject(hd, INFINITE);
	 }
	
}
//��������
void CServerDlg::OnStart() 
{
	unsigned long		idThread;
	int port=8443;
	//�����̣߳���������
	CreateThread(NULL,0,
		(LPTHREAD_START_ROUTINE)StartServer,
		(void *)port, 0 ,&idThread);
	  
}

//��ͻ��˷�����Ϣ
void CServerDlg::OnSend() 
{
	UpdateData();
	//���������Ѿ����ӿͻ��ˣ�������Ϣ
	for(int i=0;i<MAX_CLIENT;i++)
	{
		if(Clients[i].fd != 0)
		{
			//������Ϣ
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
//�����߳�WM_CLIENT_MSG��Ϣ����ʾ���б����
LRESULT CServerDlg::OnClientMsg(WPARAM wParam,LPARAM lParam)
{
	CString msg;
	msg.Format("��Socket:%d��  %s",wParam,(char *)lParam);
	m_list.InsertString(0,msg);
	free((void*)lParam);
	return 0L;
}
