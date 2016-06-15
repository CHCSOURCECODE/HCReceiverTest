// HCReceiverTestMFCWMDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "TCPDirectSetDialog.h"

// CHCReceiverTestMFCWMDlg 对话框
class CHCReceiverTestMFCWMDlg : public CDialog
{
private:
	CTCPDirectSetMFCDlg  m_DialogTCPDirect;

private:
	CEdit			m_wndWGS84Latitude;
	CEdit			m_wndWGS84Longtitude;
	CEdit			m_wndWGS84Altitude;
	CEdit			m_wndTransformLatitude;
	CEdit			m_wndTransformLongtitude;
	CEdit			m_wndTransformAltitude;
	CEdit			m_wndHDOP;
	CEdit			m_wndVDOP;
	CEdit			m_wndHRMS;
	CEdit			m_wndVRMS;
	CEdit			m_wndTime;
	CEdit			m_wndSpeed;
	CEdit			m_wndCourse;
	CEdit			m_wndGPSStatus;
	CEdit			m_wndDGPSStatus;
	CEdit			m_wndQua;
	CEdit			m_wndErrorCode;
	CEdit			m_wndGPGGA;
	CEdit			m_wndSat;
	CButton			m_wndButtonStart;
	CButton			m_wndButtonStop;
	CButton			m_wndButtonCoord;

	CString			m_strConfigPath;
	CString			m_strProjectPath;


	///****************************************/
	///*     坐标转换组件                    */
	///***************************************/
	//IHCCoordSysPtr	m_coordTrans;

	/******************************/
	/*     接收机控制组件         */
	/******************************/
	IReceiverPtr   m_Receiver;

	/****************************************/
	/*     接收机控制组件事件Cookie         */
	/***************************************/
	DWORD		   m_dwReceiverEventCookie;
// 构造
public:
	CHCReceiverTestMFCWMDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HCRECEIVERTESTMFCWM_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	void    DllRegisterServer(CString strPath);
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()


	/*****************************************/
	/***声明接口映射表以及分发映射表**********/
	/*****************************************/
	DECLARE_INTERFACE_MAP()
	DECLARE_DISPATCH_MAP()

public:
	afx_msg void OnBnClickedButtonSetCoord();
	afx_msg void OnBnClickedButtonGpsset();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonSetCors();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonGetpos();

	/****************************************/
	/*           事件对应的函数             */
	/****************************************/
	afx_msg void OnNewPosition(double dB, double dL, double dH);
	afx_msg void OnNewConstellationinformation(LPDISPATCH Constellation);
	afx_msg void OnNewCourseAndSpeed(double dCourse, double dSpeed);
	afx_msg void OnNewHDOP(double dHDOP);
	afx_msg void OnNewVDOP(double dVDOP);
	afx_msg void OnNewHRMS(double dHRMS);
	afx_msg void OnNewVRMS(double dVRMS);
	afx_msg void OnNewQualityindicator(ReceiverQualityIndicator QualityIndicator, LPCTSTR QualityIndicatorString);
	afx_msg void OnNewGPSStatus(ReceiverGPSStatus GPSStatus, LPCTSTR GPSStatusString);
	afx_msg void OnNewDGPSStatus(ReceiverDGPSStatus DGPSStatus, LPCTSTR DGPSStatusString);
	afx_msg void OnNewGPSErrorCode(ReceiverErrorCode ErrorCode, LPCTSTR ErrorCodeString);
	afx_msg void OnNewGPSTime(DATE GPSTime);
	afx_msg void OnNewGPGGA(LPCTSTR strGPGGA);
	afx_msg void OnBnClickedButtonUniqueid();
	afx_msg void OnBnClickedButtonReg();
	afx_msg void OnBnClickedButtonTCPDirect();

	afx_msg void OnBnClickedButtonStartPPK();
	afx_msg void OnBnClickedButtonStopPPK();

	CStatic m_pStaticLongtitude;
	CStatic m_pStaticLatitude;
	CStatic m_pStaticAltitude;

	private:
		//初始化文件头
		void InitPPKFileHead();

		//开始记录PPK数据
		bool StartRcrdPPKData();

		//停止PPK数据记录
		void StopRcrdPPKData();

		//GPS时间转换到当地时间
		COleDateTime GPSTimeToLocal( double dGPSTime );

		//获取GPS时间
		double GetGPSTime();
};
