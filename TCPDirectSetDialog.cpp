// HCReceiverTestMFCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TCPDirectSetDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框


CTCPDirectSetMFCDlg::CTCPDirectSetMFCDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTCPDirectSetMFCDlg::IDD, pParent)
{
	m_Receiver = NULL;
	m_mPort = 0;
}

void CTCPDirectSetMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_EDIT_IPAddress,m_wndIPAddress);
	DDX_Control(pDX,IDC_EDIT_Port,m_wndIPPort);
	DDX_Control(pDX,IDC_COMBO_Sourcelist,m_wndSourcelist);
}

BEGIN_MESSAGE_MAP(CTCPDirectSetMFCDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_Sourcelist, &CTCPDirectSetMFCDlg::OnCbnSelchangeCorssetSoucelist)
	ON_BN_CLICKED(IDC_BUTTON_Login, &CTCPDirectSetMFCDlg::OnBnClickedButtonlogin)
	ON_BN_CLICKED(IDC_BUTTON_Logout, &CTCPDirectSetMFCDlg::OnBnClickedButtonlogout)
END_MESSAGE_MAP()





BOOL CTCPDirectSetMFCDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	/*************************************************************/
	/*                 TCP直连登陆信息配置文件                      */
	/*************************************************************/
	TCHAR szPath[MAX_PATH];
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	m_strConfigPath.Format(_T("%s"), szPath);
	m_strConfigPath = m_strConfigPath.Left(m_strConfigPath.ReverseFind('\\') + 1);
	m_strConfigPath += _T("TCPDirectConfig.xml");	

	/**@brief< 从配置文件读取信息 */
	if (m_Receiver)
	{
		CComPtr<ITCPDirectInternetSetting>	TCPDirectSettins;
		CComPtr<IDGPSSettings>		DGPSSettins;

		m_Receiver->get_DGPSSettings(&DGPSSettins);
		if(DGPSSettins)
		{
			DGPSSettins->get_TCPDirectInternetSettings(&TCPDirectSettins);
			if(TCPDirectSettins)
			{
				m_mPort = 0;
				BSTR Buf = NULL;
				/**@brief< 从配置文件中加载信息 */
				if (TCPDirectSettins->LoadConfigFromFile(m_strConfigPath.AllocSysString()) == VARIANT_TRUE)
				{
					Buf = TCPDirectSettins->GetServerName();
					if(Buf)
					{
						m_strIPAddress = Buf;
						::SysFreeString(Buf);
					}

					m_mPort = TCPDirectSettins->GetServerPort();

					Buf = NULL;
					Buf = TCPDirectSettins->GetSelectedMountpoint();
					if (Buf)
					{
						m_strSelectSourceTye = Buf;
						::SysFreeString(Buf);
					}

					m_wndIPAddress.SetWindowText(m_strIPAddress);
					CString TempPort;
					TempPort.Format(_T("%d"), m_mPort);
					m_wndIPPort.SetWindowText(TempPort);
				}
					m_wndSourcelist.ResetContent();
					CString Temp;
					Buf = NULL;
					for (int i = 0; i < TCPDirectSettins->GetNumberOfMountpoins(); i++)
					{
						Buf = NULL;
					    TCPDirectSettins->GetMountpoint(i,&Buf);
						if (Buf)
						{
							Temp = Buf;
							::SysFreeString(Buf);
							m_wndSourcelist.AddString(Temp);
						}
					}
					m_wndSourcelist.SelectString(-1, m_strSelectSourceTye);
			}
		}
	}

	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTCPDirectSetMFCDlg::OnCbnSelchangeCorssetSoucelist()
{
	int nIndex = CB_ERR;
	nIndex = m_wndSourcelist.GetCurSel();
	if(nIndex != CB_ERR)
	{
		TCHAR szBuffer[256] = {0};
		m_wndSourcelist.GetLBText(nIndex, szBuffer);
		CString strTmp(szBuffer);
		m_strSelectSourceTye = strTmp; 
	}
}


void CTCPDirectSetMFCDlg::OnBnClickedButtonlogin()
{
	m_wndIPAddress.GetWindowText(m_strIPAddress);
	CString Temp;
	m_wndIPPort.GetWindowText(Temp);

	if (m_strIPAddress.IsEmpty() || Temp.IsEmpty())
	{
		::MessageBox(NULL, _T("IP or port error"), NULL, NULL);
		return;
	}

	m_mPort = _ttoi(Temp.GetBuffer());

	int nIndex = CB_ERR;
	nIndex = m_wndSourcelist.GetCurSel();
	if(nIndex != CB_ERR)
	{
		TCHAR szBuffer[256] = {0};
		m_wndSourcelist.GetLBText(nIndex, szBuffer);
		CString strTmp(szBuffer);
		m_strSelectSourceTye = strTmp; 
	}

	if (m_Receiver)
	{
		CComPtr<ITCPDirectInternetSetting>	TCPDirectSettins;
		CComPtr<IDGPSSettings>		DGPSSettins;

		m_Receiver->get_DGPSSettings(&DGPSSettins);
		if(DGPSSettins)
		{
			DGPSSettins->put_Source(TCPDirectInternetDGPSSource);
			DGPSSettins->get_TCPDirectInternetSettings(&TCPDirectSettins);
			if(TCPDirectSettins)
			{
				BSTR Buf = NULL;
				Buf = m_strIPAddress.AllocSysString();
				TCPDirectSettins->put_ServerName(Buf);

				TCPDirectSettins->put_ServerPort(m_mPort);

				Buf = m_strSelectSourceTye.AllocSysString();
				TCPDirectSettins->put_SelectedMountpoint(Buf);
			}
			BSTR Buf = NULL;
			DGPSSettins->DGPSStartWork(&Buf);
		
			OnOK();
		}
	}
}

void CTCPDirectSetMFCDlg::OnBnClickedButtonlogout()
{
	if (m_Receiver)
	{
		CComPtr<ITCPDirectInternetSetting>	TCPDirectSettins;
		CComPtr<IDGPSSettings>		DGPSSettins;

		m_Receiver->get_DGPSSettings(&DGPSSettins);
		if(DGPSSettins)
		{
			DGPSSettins->DGPSStopWork();
			::MessageBox(NULL, _T("Log out ok"), NULL, NULL);

			OnOK();
		}
	}
}

