
// Receive_CarPictureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Receive_CarPicture.h"
#include "Receive_CarPictureDlg.h"
#include "afxdialogex.h"
#include "Log.h"
#include "OperateDataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Log m_log;

#define WM_MYUPDATEDATA WM_USER+101  

#define MAX_LISTEN_NUM 6 
#define RECV_DATA_BUF_SIZE 0x400   //1024(数据块)
#define RECV_INFO_BUF_SIZE 0x12    //18(发送信息)

SOCKET sockSrv;
int m_server_port;
CString m_PicturePath;
BOOL m_posting;
HWND m_hwnd;
static DWORD WINAPI RecvProc(LPVOID lpParameter);
static DWORD WINAPI ListenRecv(LPVOID lpParameter);

CReceive_CarPictureDlg::CReceive_CarPictureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReceive_CarPictureDlg::IDD, pParent)
	, m_port_string(_T(""))
	, m_PicturePath_string(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_log.WriteOperateLog(L"登录系统！");
}

CReceive_CarPictureDlg::~CReceive_CarPictureDlg()
{
	m_log.WriteOperateLog(L"注销系统！");
}

void CReceive_CarPictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP_IPADDRESS, m_ip_control);
	DDX_Text(pDX, IDC_PORT_EDIT, m_port_string);
	DDX_Text(pDX, IDC_EDIT2, m_PicturePath_string);
}

BEGIN_MESSAGE_MAP(CReceive_CarPictureDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_BUTTON, &CReceive_CarPictureDlg::OnBnClickedStartButton)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CReceive_CarPictureDlg::OnBnClickedStopButton)
	ON_BN_CLICKED(IDC_BROWSE_BUTTON, &CReceive_CarPictureDlg::OnBnClickedBrowseButton)
	ON_MESSAGE(WM_MYUPDATEDATA,OnUpdateSocketInfo) 
END_MESSAGE_MAP()


// CReceive_CarPictureDlg 消息处理程序

BOOL CReceive_CarPictureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	HICON m_hIcon= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	CFileFind finder; 
	BOOL ifFind = finder.FindFile(_T("./Config.ini"));  
	if(!ifFind)
		m_log.WriteErrorLog(L"读取配置文件错误!");

	::GetPrivateProfileStringW(_T("Dialog Skin Config Info"),_T("Skin"),_T("skin/BlueStandard.ssk"),m_skin.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
	m_skin.ReleaseBuffer();
	USES_CONVERSION;
	skinppLoadSkin(T2A(m_skin));//项目下的皮肤文件
	// TODO: 在此添加额外的初始化代码
	m_hwnd = m_hWnd;
	CString _port;
	::GetPrivateProfileString(_T("Config Info"),_T("Port"),_T("8000"),_port.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
	_port.ReleaseBuffer();
	::GetPrivateProfileString(_T("Config Info"),_T("PicturePath"),_T("C:/"),m_PicturePath_string.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));
	m_PicturePath_string.ReleaseBuffer();
	m_PicturePath = m_PicturePath_string;
	UpdateData(FALSE);
	m_PicturePath_string.Replace(L"\\", L"/");
	m_server_port = _ttoi(_port);
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(FALSE);
	m_posting = false;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CReceive_CarPictureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CReceive_CarPictureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CReceive_CarPictureDlg::OnBnClickedStartButton()
{
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(FALSE);
	HANDLE hThread=CreateThread(NULL, 0, ListenRecv, NULL, 0, NULL);  
    CloseHandle(hThread); 
}

LRESULT CReceive_CarPictureDlg::OnUpdateSocketInfo(WPARAM wParam, LPARAM lParam)
{
	SOCKADDR_IN *_addrClient = (SOCKADDR_IN *)wParam;
	
	if(_addrClient != NULL)
	{
		BYTE nField[4]; 
		USES_CONVERSION;
		CString _ip = A2T(inet_ntoa(_addrClient->sin_addr));
		for(int i = 0; i < 3; i++)
		{
			int nStart=_ip.Find('.')+1;
			nField[i] = _ttoi(_ip.Left(nStart));
			_ip = _ip.Right(_ip.GetLength() - nStart);
		}
		nField[3] = _ttoi(_ip);
		m_ip_control.SetAddress(nField[0], nField[1], nField[2], nField[3]);
		m_port_string.Format(L"%d", _addrClient->sin_port);
	}
	else
	{
		m_ip_control.ClearAddress();
		m_port_string = L"";
	}
	
	UpdateData(FALSE);

	return 0;
}

static DWORD WINAPI ListenRecv(LPVOID lpParameter)
{
	sockSrv=socket(AF_INET,SOCK_STREAM,0);
	if(INVALID_SOCKET==sockSrv)  
    {  
		AfxMessageBox(L"接收车照套接字创建失败！");  
		m_log.WriteErrorLog(L"接收车照套接字创建失败！");  
        ExitThread(-1);
    } 
	SOCKADDR_IN addrSrv; 
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);  
	addrSrv.sin_family=AF_INET;  
	
	addrSrv.sin_port=htons(m_server_port); 
	int retval = bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//绑定端口  
	if(SOCKET_ERROR==retval)  
    {  
        closesocket(sockSrv);  
        AfxMessageBox(L"套接字绑定失败!");  
		m_log.WriteErrorLog(L"套接字绑定失败!");  
        ExitThread(-1); 
    } 
	if(listen(sockSrv, MAX_LISTEN_NUM) == SOCKET_ERROR)
	{
		closesocket(sockSrv);  
        AfxMessageBox(L"套接字监听失败!");  
		m_log.WriteErrorLog(L"套接字监听失败!");  
        ExitThread(-1); 
	}
	while(1)
	{
		SOCKADDR_IN addrClient;   //连接上的客户端IP地址 
		int len=sizeof(SOCKADDR);
		SOCKET *sockConn = new SOCKET;
		*sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&len);
		if(*sockConn == INVALID_SOCKET)
			break;
		::SendMessageW(m_hwnd, WM_MYUPDATEDATA, (WPARAM)&addrClient, 0);
		//产生一个用于接收数据的线程   
		HANDLE hThread=CreateThread(NULL, 0, RecvProc, sockConn, 0, NULL);  
		CloseHandle(hThread);  
	}

	ExitThread(0);
}

/**
*接收协议
*数据块由1024(0x400)个字节组成, 发送信息有18(0x12)个字节组成
*发送信息第一个字符为'H'为发送文件名
*发送信息第一个字符为'E'为发送数据块
*发送信息第一个字符为'N'为不存在任何图片
*发送信息第一个字符为'F'为发送最后一个数据块数据块
*发送信息第二个字符为图片的个数
*发送信息第3~18字符为发送数据块的大小
*返回值为"OK"为成功接收
*返回值为"fail"为则创建本地文件失败
*返回值为"finish"为成功接收所有图片
*返回值为"error"为接收失败
**/
static DWORD WINAPI RecvProc(LPVOID lpParameter)
{
    SOCKET *sock = (SOCKET*)lpParameter; 

    int len=sizeof(SOCKADDR);
	char recvBuf[RECV_DATA_BUF_SIZE+RECV_INFO_BUF_SIZE] = {'\0'};        //256+17字节的接受缓冲数组 
    char fileName[RECV_DATA_BUF_SIZE] = {'\0'};       //256字节的文件名存储区 
    FILE* file = NULL;

	USES_CONVERSION;
    while(TRUE)
    {
		memset(recvBuf, '\0', RECV_DATA_BUF_SIZE+RECV_INFO_BUF_SIZE);
        //接受TCP数据 
        int recv_size = recv(*sock, recvBuf, RECV_DATA_BUF_SIZE+RECV_INFO_BUF_SIZE, 0);
		while(recv_size == -1)
		{
			recv_size = recv(*sock, recvBuf, RECV_DATA_BUF_SIZE+RECV_INFO_BUF_SIZE, 0);
		}

		if(recvBuf[0] == 'H')
		{
			memset(fileName, '\0', RECV_DATA_BUF_SIZE);
			//从收到的数据中提取文件名信息           
            for (int i = RECV_INFO_BUF_SIZE-1; i < (RECV_DATA_BUF_SIZE+RECV_INFO_BUF_SIZE) && recvBuf[i] != '\0'; i++)  
                fileName[i-RECV_INFO_BUF_SIZE+1] = recvBuf[i];   
			 //创建一个文件用于复制接收的文件数据 
			CString _fileName = m_PicturePath + L"/" + fileName;			
			if (!(file = fopen(T2A(_fileName), "wb")))
			{
				AfxMessageBox(L"创建本地文件失败！");
				m_log.WriteErrorLog(_fileName + L"创建失败！");
				fclose(file);
				send(*sock, "fail", sizeof("fail")+1, 0);
				if(recvBuf[1] == '4')
				{
					::SendMessageW(m_hwnd, WM_MYUPDATEDATA, NULL, 0);
					break;
				}
			}
			else
				send(*sock, "OK", sizeof("OK")+1, 0);
		}
        //收到的消息头为'E',即对方发来的是文件数据 
        else if (recvBuf[0] == 'E')
        {
            //将文件数据写入本地文件中 
            fwrite(&recvBuf[RECV_INFO_BUF_SIZE], 1, RECV_DATA_BUF_SIZE, file);
			send(*sock, "OK", sizeof("OK")+1, 0);
		}
        //收到的消息头为'F',即对方发来最后一个数据块 
        else if (recvBuf[0] == 'F')
        {
            //获取数据块的大小 
            int bufSize = atoi(&recvBuf[2]);
            //将数据块写入本地文件，并关闭文件 
            fwrite(&recvBuf[RECV_INFO_BUF_SIZE], 1, bufSize, file);
            fclose(file);		
			//接收完5张图片
			if(recvBuf[1] == '4')
			{
				send(*sock, "finish", sizeof("finish")+1, 0);
				::SendMessageW(m_hwnd, WM_MYUPDATEDATA, NULL, 0);
				break;
			}
			else
				send(*sock, "OK", sizeof("OK")+1, 0);
        }
		else if (recvBuf[0] == 'N')
		{
			send(*sock, "OK", sizeof("OK")+1, 0);
			break;
		}
        //收到未定义的数据头 
        else
		{
			send(*sock, "error", sizeof("error")+1, 0);
			fclose(file);
            AfxMessageBox(L"传送数据过程中出现错误！");
			m_log.WriteErrorLog(L"传送数据过程中出现错误！");
			m_posting = false;
			break;
		}
    }
	m_posting = false;
	closesocket(*sock);
	delete sock;
	ExitThread(0);
}

void CReceive_CarPictureDlg::OnBnClickedStopButton()
{
	if (m_posting)  //bool m_posting 表示程序是否正在发送文件 
    {
        MessageBox(L"数据发送中，请稍候再试。");
        return;
    }
	//关闭套接字
	closesocket(sockSrv);
	m_ip_control.ClearAddress();
	UpdateData(FALSE);
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(FALSE);
}

void CReceive_CarPictureDlg::OnBnClickedBrowseButton()
{
	if (m_posting)  //bool m_posting 表示程序是否正在发送文件 
    {
        MessageBox(L"数据发送中，请稍候再试。");
        return;
    }
	m_PicturePath_string = GetSelectFolderPath();//选择的文件夹路径
	UpdateData(FALSE);
	::WritePrivateProfileStringW(_T("Config Info"),_T("PicturePath"),m_PicturePath_string,_T("./Config.ini"));  
	m_PicturePath_string.Replace(L"\\", L"/");
	m_PicturePath = m_PicturePath_string;
}

CString CReceive_CarPictureDlg::GetSelectFolderPath()
{
	BROWSEINFO bi;  
	ZeroMemory(&bi, sizeof(BROWSEINFO));  
	bi.hwndOwner = m_hWnd;  
	bi.ulFlags   = BIF_RETURNONLYFSDIRS;  
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
	BOOL bRet = FALSE;  
	TCHAR szFolder[MAX_PATH*2];  
	szFolder[0] = _T('\0');  

	if (pidl)  
	{  
		if (SHGetPathFromIDList(pidl, szFolder))    
			bRet = TRUE;  
		IMalloc *pMalloc = NULL;  
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)  
		{   
			pMalloc->Free(pidl);   
			pMalloc->Release();  
		}  

		return szFolder;
	}  
	else
	{
		CString _ret = m_PicturePath_string;
		_ret.Replace(L"/", L"\\");
		return _ret;
	}
}
