
// MFCSpiderDlg.h : ͷ�ļ�
//
#include <string>  
#include <iostream>  
#include <fstream>  
#include <vector>  
#include "winsock2.h"  
#include <time.h>  
#include <queue>  
#include <unordered_set>  
#pragma once

using namespace std;  

UINT Fun(LPVOID pParam);
// CMFCSpiderDlg �Ի���
class CMFCSpiderDlg : public CDialogEx
{
// ����
public:
	CMFCSpiderDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCSPIDER_DIALOG };
	CFont  font;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	CWinThread* pThread;


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedCancel();
	CString m_url;
	CString m_path;
	int m_type;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};

