
// MFCSpiderDlg.h : 头文件
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
// CMFCSpiderDlg 对话框
class CMFCSpiderDlg : public CDialogEx
{
// 构造
public:
	CMFCSpiderDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCSPIDER_DIALOG };
	CFont  font;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	CWinThread* pThread;


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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

