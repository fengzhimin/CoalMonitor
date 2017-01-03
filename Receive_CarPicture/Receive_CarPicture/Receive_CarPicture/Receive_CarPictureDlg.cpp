
// Receive_CarPictureDlg.cpp : ʵ���ļ�
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
#define RECV_DATA_BUF_SIZE 0x400   //1024(���ݿ�)
#define RECV_INFO_BUF_SIZE 0x12    //18(������Ϣ)

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
	m_log.WriteOperateLog(L"��¼ϵͳ��");
}

CReceive_CarPictureDlg::~CReceive_CarPictureDlg()
{
	m_log.WriteOperateLog(L"ע��ϵͳ��");
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


// CReceive_CarPictureDlg ��Ϣ�������

BOOL CReceive_CarPictureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	HICON m_hIcon= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	CFileFind finder; 
	BOOL ifFind = finder.FindFile(_T("./Config.ini"));  
	if(!ifFind)
		m_log.WriteErrorLog(L"��ȡ�����ļ�����!");

	::GetPrivateProfileStringW(_T("Dialog Skin Config Info"),_T("Skin"),_T("skin/BlueStandard.ssk"),m_skin.GetBuffer(MAX_PATH),MAX_PATH,_T("./Config.ini"));  
	m_skin.ReleaseBuffer();
	USES_CONVERSION;
	skinppLoadSkin(T2A(m_skin));//��Ŀ�µ�Ƥ���ļ�
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CReceive_CarPictureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		AfxMessageBox(L"���ճ����׽��ִ���ʧ�ܣ�");  
		m_log.WriteErrorLog(L"���ճ����׽��ִ���ʧ�ܣ�");  
        ExitThread(-1);
    } 
	SOCKADDR_IN addrSrv; 
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);  
	addrSrv.sin_family=AF_INET;  
	
	addrSrv.sin_port=htons(m_server_port); 
	int retval = bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//�󶨶˿�  
	if(SOCKET_ERROR==retval)  
    {  
        closesocket(sockSrv);  
        AfxMessageBox(L"�׽��ְ�ʧ��!");  
		m_log.WriteErrorLog(L"�׽��ְ�ʧ��!");  
        ExitThread(-1); 
    } 
	if(listen(sockSrv, MAX_LISTEN_NUM) == SOCKET_ERROR)
	{
		closesocket(sockSrv);  
        AfxMessageBox(L"�׽��ּ���ʧ��!");  
		m_log.WriteErrorLog(L"�׽��ּ���ʧ��!");  
        ExitThread(-1); 
	}
	while(1)
	{
		SOCKADDR_IN addrClient;   //�����ϵĿͻ���IP��ַ 
		int len=sizeof(SOCKADDR);
		SOCKET *sockConn = new SOCKET;
		*sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&len);
		if(*sockConn == INVALID_SOCKET)
			break;
		::SendMessageW(m_hwnd, WM_MYUPDATEDATA, (WPARAM)&addrClient, 0);
		//����һ�����ڽ������ݵ��߳�   
		HANDLE hThread=CreateThread(NULL, 0, RecvProc, sockConn, 0, NULL);  
		CloseHandle(hThread);  
	}

	ExitThread(0);
}

/**
*����Э��
*���ݿ���1024(0x400)���ֽ����, ������Ϣ��18(0x12)���ֽ����
*������Ϣ��һ���ַ�Ϊ'H'Ϊ�����ļ���
*������Ϣ��һ���ַ�Ϊ'E'Ϊ�������ݿ�
*������Ϣ��һ���ַ�Ϊ'N'Ϊ�������κ�ͼƬ
*������Ϣ��һ���ַ�Ϊ'F'Ϊ�������һ�����ݿ����ݿ�
*������Ϣ�ڶ����ַ�ΪͼƬ�ĸ���
*������Ϣ��3~18�ַ�Ϊ�������ݿ�Ĵ�С
*����ֵΪ"OK"Ϊ�ɹ�����
*����ֵΪ"fail"Ϊ�򴴽������ļ�ʧ��
*����ֵΪ"finish"Ϊ�ɹ���������ͼƬ
*����ֵΪ"error"Ϊ����ʧ��
**/
static DWORD WINAPI RecvProc(LPVOID lpParameter)
{
    SOCKET *sock = (SOCKET*)lpParameter; 

    int len=sizeof(SOCKADDR);
	char recvBuf[RECV_DATA_BUF_SIZE+RECV_INFO_BUF_SIZE] = {'\0'};        //256+17�ֽڵĽ��ܻ������� 
    char fileName[RECV_DATA_BUF_SIZE] = {'\0'};       //256�ֽڵ��ļ����洢�� 
    FILE* file = NULL;

	USES_CONVERSION;
    while(TRUE)
    {
		memset(recvBuf, '\0', RECV_DATA_BUF_SIZE+RECV_INFO_BUF_SIZE);
        //����TCP���� 
        int recv_size = recv(*sock, recvBuf, RECV_DATA_BUF_SIZE+RECV_INFO_BUF_SIZE, 0);
		while(recv_size == -1)
		{
			recv_size = recv(*sock, recvBuf, RECV_DATA_BUF_SIZE+RECV_INFO_BUF_SIZE, 0);
		}

		if(recvBuf[0] == 'H')
		{
			memset(fileName, '\0', RECV_DATA_BUF_SIZE);
			//���յ�����������ȡ�ļ�����Ϣ           
            for (int i = RECV_INFO_BUF_SIZE-1; i < (RECV_DATA_BUF_SIZE+RECV_INFO_BUF_SIZE) && recvBuf[i] != '\0'; i++)  
                fileName[i-RECV_INFO_BUF_SIZE+1] = recvBuf[i];   
			 //����һ���ļ����ڸ��ƽ��յ��ļ����� 
			CString _fileName = m_PicturePath + L"/" + fileName;			
			if (!(file = fopen(T2A(_fileName), "wb")))
			{
				AfxMessageBox(L"���������ļ�ʧ�ܣ�");
				m_log.WriteErrorLog(_fileName + L"����ʧ�ܣ�");
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
        //�յ�����ϢͷΪ'E',���Է����������ļ����� 
        else if (recvBuf[0] == 'E')
        {
            //���ļ�����д�뱾���ļ��� 
            fwrite(&recvBuf[RECV_INFO_BUF_SIZE], 1, RECV_DATA_BUF_SIZE, file);
			send(*sock, "OK", sizeof("OK")+1, 0);
		}
        //�յ�����ϢͷΪ'F',���Է��������һ�����ݿ� 
        else if (recvBuf[0] == 'F')
        {
            //��ȡ���ݿ�Ĵ�С 
            int bufSize = atoi(&recvBuf[2]);
            //�����ݿ�д�뱾���ļ������ر��ļ� 
            fwrite(&recvBuf[RECV_INFO_BUF_SIZE], 1, bufSize, file);
            fclose(file);		
			//������5��ͼƬ
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
        //�յ�δ���������ͷ 
        else
		{
			send(*sock, "error", sizeof("error")+1, 0);
			fclose(file);
            AfxMessageBox(L"�������ݹ����г��ִ���");
			m_log.WriteErrorLog(L"�������ݹ����г��ִ���");
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
	if (m_posting)  //bool m_posting ��ʾ�����Ƿ����ڷ����ļ� 
    {
        MessageBox(L"���ݷ����У����Ժ����ԡ�");
        return;
    }
	//�ر��׽���
	closesocket(sockSrv);
	m_ip_control.ClearAddress();
	UpdateData(FALSE);
	GetDlgItem(IDC_START_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(FALSE);
}

void CReceive_CarPictureDlg::OnBnClickedBrowseButton()
{
	if (m_posting)  //bool m_posting ��ʾ�����Ƿ����ڷ����ļ� 
    {
        MessageBox(L"���ݷ����У����Ժ����ԡ�");
        return;
    }
	m_PicturePath_string = GetSelectFolderPath();//ѡ����ļ���·��
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
