
// MFCSpiderDlg.cpp : ʵ���ļ�
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

// CMFCSpiderDlg �Ի���




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


// CMFCSpiderDlg ��Ϣ�������

BOOL CMFCSpiderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CEdit *m_EditAa=(CEdit *)GetDlgItem(IDC_EDIT1);
	font.CreatePointFont(280,"����");     //ѡ������
	m_EditAa->SetFont(&font);

	flag=0;
	m_url="www.guokr.com";
	m_path="./";
	m_type=3;
	UpdateData(FALSE);
	//
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT3);
    nLength = pEdit->GetWindowTextLength();
    //ѡ����ǰ�ı���ĩ��
    pEdit->SetSel(nLength, nLength);
    //l׷���ı�
    pEdit->ReplaceSel("��ӭʹ���������档\n");  
	//
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCSpiderDlg::OnPaint()
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
HCURSOR CMFCSpiderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCSpiderDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	exit(0);
}


void CMFCSpiderDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//
	if(flag==0)
		{
			flag=1;
			//
			GetDlgItem(IDC_BUTTON1)->SetWindowText("ֹͣ");
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
		GetDlgItem(IDC_BUTTON1)->SetWindowText("�ȴ��߳̽���");
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
		GetDlgItem(IDC_BUTTON1)->SetWindowText("��ʼ");
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
