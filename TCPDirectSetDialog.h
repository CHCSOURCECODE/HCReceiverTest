

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

	// ����
public:
	CTCPDirectSetMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

	// �Ի�������
	enum { IDD = IDD_DIALOG_TCPDirect };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedButtonlogin();
	afx_msg void OnBnClickedButtonlogout();
	afx_msg void OnCbnSelchangeCorssetSoucelist();

	DECLARE_MESSAGE_MAP()

};
