
// MFCSpiderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCSpider.h"
#include "MFCSpiderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern CEdit* pEdit;
extern int nLength;
extern int flag;
extern string murl;
extern string mpath;
extern int mtype; 

// CMFCSpiderDlg 对话框




CMFCSpiderDlg::CMFCSpiderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCSpiderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_url = _T("");
	m_path = _T("");
	//  m_type = 0;
	m_type = 0;
}

void CMFCSpiderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_url);
	DDV_MaxChars(pDX, m_url, 100);
	DDX_Text(pDX, IDC_EDIT2, m_path);
	DDV_MaxChars(pDX, m_path, 100);
	//  DDX_Radio(pDX, IDC_RADIO1, m_type);
	//  DDV_MinMaxInt(pDX, m_type, 0, 3);
	DDX_Radio(pDX, IDC_RADIO1, m_type);
	DDV_MinMaxInt(pDX, m_type, 0, 3);
}

BEGIN_MESSAGE_MAP(CMFCSpiderDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCSpiderDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCSpiderDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCSpiderDlg 消息处理程序

BOOL CMFCSpiderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CEdit *m_EditAa=(CEdit *)GetDlgItem(IDC_EDIT1);
	font.CreatePointFont(280,"宋体");     //选择字体
	m_EditAa->SetFont(&font);

	flag=0;
	m_url="www.guokr.com";
	m_path="./";
	m_type=3;
	UpdateData(FALSE);
	//
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
    nLength = pEdit->GetWindowTextLength();
    //选定当前文本的末端
    pEdit->SetSel(nLength, nLength);
    //l追加文本
    pEdit->ReplaceSel("欢迎使用网络爬虫。\n");  
	//
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCSpiderDlg::OnPaint()
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
HCURSOR CMFCSpiderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCSpiderDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
}


void CMFCSpiderDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//
	if(flag==0)
		{
			flag=1;
			//
			GetDlgItem(IDC_BUTTON1)->SetWindowText("停止");
			//
			CWnd *pWnd = GetDlgItem(IDC_EDIT1);
			pWnd->EnableWindow(FALSE);
			pWnd = GetDlgItem(IDC_EDIT2);
			pWnd->EnableWindow(FALSE);
			pWnd = GetDlgItem(IDC_RADIO1);
			pWnd->EnableWindow(FALSE);
			pWnd = GetDlgItem(IDC_RADIO2);
			pWnd->EnableWindow(FALSE);
			pWnd = GetDlgItem(IDC_RADIO3);
			pWnd->EnableWindow(FALSE);
			pWnd = GetDlgItem(IDC_RADIO4);
			pWnd->EnableWindow(FALSE);
			//
			murl=m_url;
			mpath=m_path;
			mtype=m_type;
			pThread=AfxBeginThread(Fun,this);
			//
		}
	else
	{
		flag=0;
		//
		GetDlgItem(IDC_BUTTON1)->SetWindowText("等待线程结束");
		DWORD dwRet = 0;  
		MSG msg;  
		while (TRUE)  
		{  
		  dwRet = WaitForSingleObject(pThread->m_hThread,50);  
		  switch(dwRet)  
		  {  
		  case WAIT_OBJECT_0:   
			break;
		  case WAIT_TIMEOUT :  
			PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);  
			continue;  
		  default:  break;
		  }  
		  break;  
		}  
		//
		GetDlgItem(IDC_BUTTON1)->SetWindowText("开始");
		//
			CWnd *pWnd = GetDlgItem(IDC_EDIT1);
			pWnd->EnableWindow(TRUE);
			pWnd = GetDlgItem(IDC_EDIT2);
			pWnd->EnableWindow(TRUE);
			pWnd = GetDlgItem(IDC_RADIO1);
			pWnd->EnableWindow(TRUE);
			pWnd = GetDlgItem(IDC_RADIO2);
			pWnd->EnableWindow(TRUE);
			pWnd = GetDlgItem(IDC_RADIO3);
			pWnd->EnableWindow(TRUE);
			pWnd = GetDlgItem(IDC_RADIO4);
			pWnd->EnableWindow(TRUE);
		//
	}
}
