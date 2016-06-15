

#pragma once
#include "Resourceppc.h"
#include "afxwin.h"

class CTCPDirectSetMFCDlg : public CDialog
{
private:
	CEdit					m_wndIPAddress;
	CEdit					m_wndIPPort;
	CComboBox				m_wndSourcelist;
	CString                 m_strConfigPath;

	CString					m_strIPAddress;
	long					m_mPort;
	CString					m_strSelectSourceTye;

	IReceiverPtr			m_Receiver;	

public:
	void SetDGPSSettings(IReceiverPtr InReceiverPtr)
	{
		ASSERT(InReceiverPtr != NULL);
		m_Receiver = InReceiverPtr;
	}

	// 构造
public:
	CTCPDirectSetMFCDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_DIALOG_TCPDirect };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedButtonlogin();
	afx_msg void OnBnClickedButtonlogout();
	afx_msg void OnCbnSelchangeCorssetSoucelist();

	DECLARE_MESSAGE_MAP()

};
