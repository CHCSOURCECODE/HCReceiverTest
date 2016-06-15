// HCReceiverTestMFCWMDlg.cpp: implementation file
//

#include "stdafx.h"
#include "HCReceiverTestMFCWM.h"
#include "HCReceiverTestMFCWMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**************************************************************
HCReceiver working principle:
Explaination: HCReceiver is a dll to configurate the data output of LT500 series devices, having the functions of parsing data, recording PPK data, logging in CORS, etc.
        You can obtain the parsed data from the following two modes:
	  1.Passive messaging mode£¨Msg Model£©
	      After HCReceiver parse out the position, precision, time and other information, these information can be sent through the message interface,
		  and you can register message function for receiving information sent by HCReceiver
		
		You can learn more from the below @MsgModel comments

	  2.Positive obtaining mode£¨Get model£©
	    You can learn from the below @MsgModel comments

 Use steps:
       1.Put HCReceiver.dll, atl90.dll, HCResources.dll, msvcr90.dll and UniqueID.dll into the software directory
	   2.Intialize COM envirnment£¨@Setp - 2£©
	   3.Register HCReceiver.dll£¨@Setp - 3£©
	   4.Create HCReceiver object£¨@Setp - 4£©
	   5.Mapping interface£¨only for Msg Model£©£¨@Setp - 5£©
	   6.Obtain information by passive or positive mode£¨@Setp - 6£©
	     
***************************************************************/

CHCReceiverTestMFCWMDlg::CHCReceiverTestMFCWMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHCReceiverTestMFCWMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	/********************************/
	/*                              */
	/********************************/

	m_Receiver = NULL;
	m_dwReceiverEventCookie = 0;
}

void CHCReceiverTestMFCWMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_WGS84_LATITUDE, m_wndWGS84Latitude);
	DDX_Control(pDX, IDC_EDIT_WGS84_LONGITUDE, m_wndWGS84Longtitude);
	DDX_Control(pDX, IDC_EDIT_WGS84_ALTITUDE, m_wndWGS84Altitude);

	DDX_Control(pDX, IDC_EDIT_TRANSFORM_LATITUDE, m_wndTransformLatitude);
	DDX_Control(pDX, IDC_EDIT_TRANSFORM_LONGITUDE, m_wndTransformLongtitude);
	DDX_Control(pDX, IDC_EDIT_TRANSFORM_ALTITUDE, m_wndTransformAltitude);

	DDX_Control(pDX, IDC_EDIT_HDOP, m_wndHDOP);
	DDX_Control(pDX, IDC_EDIT_VDOP, m_wndVDOP);
	DDX_Control(pDX, IDC_EDIT_HRMS, m_wndHRMS);
	DDX_Control(pDX, IDC_EDIT_VRMS, m_wndVRMS);

	DDX_Control(pDX, IDC_EDIT_TIME, m_wndTime);
	DDX_Control(pDX, IDC_EDIT_SPEED, m_wndSpeed);
	DDX_Control(pDX, IDC_EDIT_COURSE, m_wndCourse);

	DDX_Control(pDX, IDC_EDIT_GPSSTATUS, m_wndGPSStatus);
	DDX_Control(pDX, IDC_EDIT_DGPSSTATUS, m_wndDGPSStatus);
	DDX_Control(pDX, IDC_EDIT_QUA, m_wndQua);
	DDX_Control(pDX, IDC_EDIT_ERRORCODE, m_wndErrorCode);
	//	DDX_Control(pDX, IDC_EDIT_GPGGA, m_wndGPGGA);
	DDX_Control(pDX, IDC_EDIT_SAT, m_wndSat);

	DDX_Control(pDX, IDC_BUTTON_START, m_wndButtonStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_wndButtonStop);
	DDX_Control(pDX, IDC_BUTTON_SETCOORD, m_wndButtonCoord);

	DDX_Control(pDX, IDC_STATIC_TRAN_LONGTITUDE, m_pStaticLongtitude);
	DDX_Control(pDX, IDC_STATIC_TRAN_LATITUDE, m_pStaticLatitude);
	DDX_Control(pDX, IDC_STATIC_TRAN_ALTITUDE, m_pStaticAltitude);
}

BEGIN_MESSAGE_MAP(CHCReceiverTestMFCWMDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP	
	ON_BN_CLICKED(IDC_BUTTON_SETCOORD, &CHCReceiverTestMFCWMDlg::OnBnClickedButtonSetCoord)
	ON_BN_CLICKED(IDC_BUTTON_GPSSET, &CHCReceiverTestMFCWMDlg::OnBnClickedButtonGpsset)
	ON_BN_CLICKED(IDC_BUTTON_START, &CHCReceiverTestMFCWMDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_SETCORS, &CHCReceiverTestMFCWMDlg::OnBnClickedButtonSetCors)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CHCReceiverTestMFCWMDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_GETPOS, &CHCReceiverTestMFCWMDlg::OnBnClickedButtonGetpos)
	ON_BN_CLICKED(IDC_BUTTON_UNIQUEID, &CHCReceiverTestMFCWMDlg::OnBnClickedButtonUniqueid)
	ON_BN_CLICKED(IDC_BUTTON_REG, &CHCReceiverTestMFCWMDlg::OnBnClickedButtonReg)
	ON_BN_CLICKED(IDC_BUTTON_TCPDirect,&CHCReceiverTestMFCWMDlg::OnBnClickedButtonTCPDirect)

	ON_BN_CLICKED(IDC_BUTTON_PPKStart,&CHCReceiverTestMFCWMDlg::OnBnClickedButtonStartPPK)
	ON_BN_CLICKED(IDC_BUTTON_PPKStop,&CHCReceiverTestMFCWMDlg::OnBnClickedButtonStopPPK)

END_MESSAGE_MAP()

/*************************************/
/*  @Setp - 5 Interface mapping table                      */
/*************************************/
BEGIN_INTERFACE_MAP(CHCReceiverTestMFCWMDlg, CDialog)
	INTERFACE_PART(CHCReceiverTestMFCWMDlg, __uuidof(_IReceiverEvents), Dispatch)
END_INTERFACE_MAP()

/********************************************/
/*    @Setp - 5   Distribute mapping table                */
/********************************************/
BEGIN_DISPATCH_MAP(CHCReceiverTestMFCWMDlg, CDialog)
	DISP_FUNCTION_ID(CHCReceiverTestMFCWMDlg, "", DISPID_GPSRECEIVER_NewPosition,                     OnNewPosition, VT_EMPTY, VTS_R8 VTS_R8 VTS_R8)
	DISP_FUNCTION_ID(CHCReceiverTestMFCWMDlg, "", DISPID_GPSRECEIVER_NewConstellationInformation,     OnNewConstellationinformation, VT_EMPTY, VTS_DISPATCH)
	DISP_FUNCTION_ID(CHCReceiverTestMFCWMDlg, "", DISPID_GPSRECEIVER_NewCourseAndSpeed,               OnNewCourseAndSpeed, VT_EMPTY, VTS_R8 VTS_R8)
	DISP_FUNCTION_ID(CHCReceiverTestMFCWMDlg, "", DISPID_GPSRECEIVER_NewHDOP,                         OnNewHDOP, VT_EMPTY, VTS_R8)
	DISP_FUNCTION_ID(CHCReceiverTestMFCWMDlg, "", DISPID_GPSRECEIVER_NewVDOP,                         OnNewVDOP, VT_EMPTY, VTS_R8)
	DISP_FUNCTION_ID(CHCReceiverTestMFCWMDlg, "", DISPID_GPSRECEIVER_NewHRMS,                         OnNewHRMS, VT_EMPTY, VTS_R8)
	DISP_FUNCTION_ID(CHCReceiverTestMFCWMDlg, "", DISPID_GPSRECEIVER_NewVRMS,                         OnNewVRMS, VT_EMPTY, VTS_R8)
	DISP_FUNCTION_ID(CHCReceiverTestMFCWMDlg, "", DISPID_GPSRECEIVER_NewQualityIndicator,             OnNewQualityindicator, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CHCReceiverTestMFCWMDlg, "", DISPID_GPSRECEIVER_NewGPSStatus,                    OnNewGPSStatus, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CHCReceiverTestMFCWMDlg, "", DISPID_GPSRECEIVER_NewDGPSStatus,                   OnNewDGPSStatus, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CHCReceiverTestMFCWMDlg, "", DISPID_GPSRECEIVER_NewGPSErrorCode,                 OnNewGPSErrorCode, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CHCReceiverTestMFCWMDlg, "", DISPID_GPSRECEIVER_NewGPSTime,                      OnNewGPSTime, VT_EMPTY, VTS_DATE)
	DISP_FUNCTION_ID(CHCReceiverTestMFCWMDlg, "", DISPID_GPSRECEIVER_NewGPGGA,                        OnNewGPGGA, VT_EMPTY,  VTS_BSTR)
END_DISPATCH_MAP()


/************************************************************************/
/*                   Register HCReceiver                                     */
/************************************************************************/
void CHCReceiverTestMFCWMDlg::DllRegisterServer(CString strPath)
{
	CString strTmp(strPath);

	strTmp += _T("HCReceiver.dll");
	HINSTANCE hHandle = NULL;
	hHandle = ::LoadLibrary(strTmp.GetBuffer());
	if(hHandle)
	{
		FARPROC pro = NULL;
		pro = GetProcAddress(hHandle, _T("DllRegisterServer"));
		if(pro)
		{
			pro();
		}

		::FreeLibrary(hHandle);
	}

}

BOOL CHCReceiverTestMFCWMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	SetIcon(m_hIcon, TRUE);			// Set large icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	/*************************************************/
	/*                 @Setp - 2 
						Initialize COM libriay               
	/*************************************************/

	EnableAutomation();
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	/*************************************************************/
	/*                 @Setp - 3 Register HCReceiver             */
	/*************************************************************/
	TCHAR szPath[MAX_PATH];
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	m_strConfigPath.Format(_T("%s"), szPath);
	m_strConfigPath = m_strConfigPath.Left(m_strConfigPath.ReverseFind('\\') + 1);
	m_strProjectPath = m_strConfigPath;
	DllRegisterServer(m_strConfigPath);


	/*************************************************************/
	/*                 Configuration file of NTRIP login infomation  */
	/*************************************************************/
	m_strConfigPath += _T("CORSConfig.xml");


	/*************************************************/
	/*                  @Setp - 4 Create HCReceiver object         */
	/*************************************************/
	HRESULT hr = S_OK;
	hr = m_Receiver.CreateInstance(__uuidof(Receiver));
	if(FAILED(hr) || m_Receiver == NULL)
	{
		MessageBox(_T("Error"), _T("Create Receiver Failed"), MB_ICONERROR);
		return FALSE;
	}

	LPUNKNOWN pUnkSink = GetIDispatch(FALSE);
	if (!AfxConnectionAdvise(m_Receiver, __uuidof(_IReceiverEvents), pUnkSink, FALSE, &m_dwReceiverEventCookie))
	{
		MessageBox(_T("Error"), _T("Connection Failed"), MB_ICONERROR);

		return FALSE;
	}



	return TRUE;  // Return TRUE unless you set the focus to the control
}

BOOL CHCReceiverTestMFCWMDlg::DestroyWindow()
{
	if(m_Receiver)
	{
		VARIANT_BOOL bRet;
		bRet = VARIANT_TRUE;
		bRet = m_Receiver->IsTracking();
		if(bRet == VARIANT_TRUE)
		{
			m_Receiver->StopTracking();
			m_Receiver->DisableDebugLog();
		}

		LPUNKNOWN pUnkSink = GetIDispatch(FALSE);

		if (m_dwReceiverEventCookie != 0)
		{
			AfxConnectionUnadvise(m_Receiver, __uuidof(_IReceiverEvents), pUnkSink, FALSE, m_dwReceiverEventCookie);
		}

		m_Receiver = NULL;
	}

	return CDialog::DestroyWindow();
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CHCReceiverTestMFCWMDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_HCRECEIVERTESTMFCWM_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_HCRECEIVERTESTMFCWM_DIALOG));
	}
}
#endif

/************************************************************************/
/* @Setp - 6 / @MsgModel
     Position information  
/* Position information will be sent when it is parsed by HCReceiver,then you can get position information here
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnNewPosition(double dB, double dL, double dH)
{
	CString strLatitude;
	CString strLongtitude;
	CString strAltitude;

	strLatitude.Format(_T("%f"), dB);
	strLongtitude.Format(_T("%f"), dL);
	strAltitude.Format(_T("%f"), dH);

	m_wndWGS84Latitude.SetWindowText(strLatitude);
	m_wndWGS84Longtitude.SetWindowText(strLongtitude);
	m_wndWGS84Altitude.SetWindowText(strAltitude);

}


int m_nGPS = 0;//GPS Sat Number
int m_nGLonass = 0;//GLONASS Sat Number
int m_nBDs = 0; //BDS Sat Number

int m_nGPSLock = 0;//GPS Locked Sat Number
int m_nGLonassLock = 0;//GLONASS Locked Sat Number
int m_nBDsLock = 0;//BDS Locked Sat Number

/************************************************************************/
/* @Setp - 6 /  @MsgModel
   Sat Msg 
* when satellilte information is parsed by HCReceiver, it will be sent through this interface

  HCReceiver sends message according to GNSSTYPE, it will send a message for each type of satellite information 
  Taking LT500T as example, when HCReceiver parsed GPS satellilte information, it will call this message, and GLONASS will call it too...
			  So you will receive 4 times of message,  respectively are satellite information of GPS, GLONASS, BDS and SBAS
  

  GNSSTYPE£º
  GNSSGPS: represent GPS satellite
  GNSSGLONASS£ºrepresent GLONASS satellite
  GNSSCOMPASS£ºrepresent COMPASS satellite
  GNSSSBAS: represent SBAS satellite
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnNewConstellationinformation(LPDISPATCH Constellation)
{
	IGPSConstellationPtr gpsInfo(Constellation);

	LONG nCount = 0;
	gpsInfo->get_NumberOfSatellitesInList(&nCount);
	int nClock = 0;
	ReceiverGNSSType SatelliteType;
	double ElevationAngle;
	double Azimuth;
	double L1SNR;
	double L2SNR;
	VARIANT_BOOL IsUsed;
	LONG   nPrn;
	for(int i = 0; i < nCount; i++)
	{	
		gpsInfo->GetSatelliteInformation(i, &nPrn, &SatelliteType, &ElevationAngle, &Azimuth, &L1SNR, &L2SNR, &IsUsed);

		if(IsUsed == VARIANT_TRUE)
		{
			nClock++;
		}
	}

	if (SatelliteType == GNSSGPS)
	{
		m_nGPSLock = nClock;
		m_nGPS = nCount;
	}
	else if (SatelliteType == GNSSGLONASS)
	{
		m_nGLonassLock = nClock;
		m_nGLonass = nCount;
	}
	else if (SatelliteType == GNSSCOMPASS)
	{
		m_nBDsLock = nClock;
		m_nBDs = nCount;
	}

	CString strTmp;
	strTmp.Format(_T("%d|%d"), m_nBDsLock + m_nGLonassLock + m_nGPSLock, m_nGPS + m_nGLonass + m_nBDs);
	m_wndSat.SetWindowText(strTmp);
}

/************************************************************************/
/* @Setp - 6 /  @MsgModel
   Direction and speed
*  The direction and speed information will be sent through this interface after being parsed by HCReceiver 
  Seeing as below£º
         dCourse£ºdirection
		 dSpeed£º speed
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnNewCourseAndSpeed(double dCourse, double dSpeed)
{
	CString strCourse;
	CString strSpeed;
	strCourse.Format(_T("%f"), dCourse);
	strSpeed.Format(_T("%f"), dSpeed);

	m_wndSpeed.SetWindowText(strSpeed);
	m_wndCourse.SetWindowText(strCourse);
}

/************************************************************************/
/* @Setp - 6 / @MsgModel 
   HDOP information 
*  HDOP information will be sent through this interface after HDOP info is parsed by HCReceiver
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnNewHDOP(double dHDOP)
{
	CString strHDOP;

	strHDOP.Format(_T("%f"), dHDOP);
	m_wndHDOP.SetWindowText(strHDOP);
}

/************************************************************************/
/* @Setp - 6 /  @MsgModel
   VDOP information
*  VDOP information will be sent through this interface after VDOP information is parsed by HCReceiver
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnNewVDOP(double dVDOP)
{
	CString strVDOP;

	strVDOP.Format(_T("%f"), dVDOP);
	m_wndVDOP.SetWindowText(strVDOP);
}

/************************************************************************/
/*@Setp - 6 /  @MsgModel 
   OnNewHRMS horizontal precision information
*  OnNewHRMS inforface will be sent through this interface after HRMS information is parsed by HCReceiver
 
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnNewHRMS(double dHRMS)
{
	CString strHRMS;

	strHRMS.Format(_T("%f"), dHRMS);
	m_wndHRMS.SetWindowText(strHRMS);
}

/************************************************************************/
/* @Setp - 6 /  @MsgModel
   OnNewVRMS vetical precision information 
*  OnNewVRMS information will be sent through this interface after VRMS info is parsed by HCReceiver
 
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnNewVRMS(double dVRMS)
{
	CString strVRMS;

	strVRMS.Format(_T("%f"), dVRMS);
	m_wndVRMS.SetWindowText(strVRMS);
}

/************************************************************************/
/*@Setp - 6  /  @MsgModel
   Status information of satellilte parsing quality
   ReceiverQualityIndicator£ºquality status of current satellilte, including autonomous, difference, float, fix, etc.
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnNewQualityindicator(ReceiverQualityIndicator QualityIndicator, LPCTSTR QualityIndicatorString)
{

	CString strReuslt = _T("");
	if (QualityIndicator == Dif)
	{
		strReuslt = "DIF";
	}
	else if (QualityIndicator == Fix)
	{
		strReuslt = "Fix";
	}
	else if (QualityIndicator == Floating)
	{
		strReuslt = "Floating";
	}
	else if (QualityIndicator == Rtd)
	{
		strReuslt = "Rtd";
	}
	else if (QualityIndicator == SearchSat)
	{
		strReuslt = "SearchSat";
	}
	else if (QualityIndicator == Single)
	{
		strReuslt = "Single";
	}
	else if (QualityIndicator == Unknow)
	{
		strReuslt = "Unknow";
	}
	else if (QualityIndicator == Waas)
	{
		strReuslt = "Waas";
	}

	m_wndQua.SetWindowText(strReuslt);
}

/************************************************************************/
/* @Setp - 6 /  @MsgModel
    Status information
    Current status of HCReceiver, such as parsing position, obtaining differential data and so on
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnNewGPSStatus(ReceiverGPSStatus GPSStatus, LPCTSTR GPSStatusString)
{
	CString strStatus(GPSStatusString);

	m_wndGPSStatus.SetWindowText(strStatus);
}


void CHCReceiverTestMFCWMDlg::OnNewDGPSStatus(ReceiverDGPSStatus DGPSStatus, LPCTSTR DGPSStatusString)
{
	CString strDGPSStatus(DGPSStatusString);

	m_wndDGPSStatus.SetWindowText(strDGPSStatus);
}


int i = 0;
/************************************************************************/
/* @Setp - 6 /  @MsgModel
   HCReciver error message£¬ 
   Error and reminder information encountered in the work, such as low precision, network abnormal, update of CORS data, etc.
   If you can't get the position information normally, it can help you to analyse
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnNewGPSErrorCode(ReceiverErrorCode ErrorCode, LPCTSTR ErrorCodeString)
{
	//CString strErrorCode(ErrorCodeString);

	if (ErrorCode == UpdataData)
	{
		if (i > 100)
		{
			i = 0;
		}
		CString strTmp;
		strTmp.Format(_T("UpdataData:%d"), i);
		m_wndErrorCode.SetWindowText(strTmp);
		++i;
	}
	else
	{
		CString strErrorCode(ErrorCodeString);
		m_wndErrorCode.SetWindowText(strErrorCode);
	}
	

	if(ErrorCode == LoadSuccess)
	{
		CComPtr<IDGPSSettings> pDGPSSettings = 0;
		CComPtr<IInternetSettings> pInternetSettins = 0;
		m_Receiver->get_DGPSSettings(&pDGPSSettings);
		if(pDGPSSettings)
		{
			pDGPSSettings->get_InternetSettings(&pInternetSettins);
			if(pInternetSettins)
			{				
				pInternetSettins->SaveConfigToFile(m_strConfigPath.AllocSysString());
			}
		}
	}

	if(ErrorCode == RegisteExpire)
	{
		MessageBox(ErrorCodeString, _T("Tip"), MB_ICONINFORMATION);
	}

	if(ErrorCode == LoadSuccess)
	{
		MessageBox(ErrorCodeString, _T("Tip"), MB_ICONINFORMATION);
	}

}

/************************************************************************/
/* @Setp - 6 /  @MsgModel
   GPS Time information
   New GPS time information will be sent throuth this interface when HCReceiver receive it
   GPSTime£ºGPS original time, not local time
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnNewGPSTime(DATE GPSTime)
{
	COleDateTime time;
	time.m_dt = GPSTime;

	CString strTime;
	strTime = time.Format(_T("%m:%d-%H:%M:%S"));

	m_wndTime.SetWindowText(strTime);
}

/************************************************************************/
/* @Setp - 6 /  @MsgModel
   GPGGA information
   GGA data will be sent through this interface when HCReceiver receive it
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnNewGPGGA(LPCTSTR strGPGGA)
{
	CString szGPGGA(strGPGGA);
}



void CHCReceiverTestMFCWMDlg::OnBnClickedButtonSetCoord()
{
	
}

/************************************************************************/
/* @GetMode  
  Display the serial port configuration information, including COM, baud rate, HDOP threshold, VDOP threshod, HRMS threshold and VRMS threshold
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnBnClickedButtonGpsset()
{
	if(m_Receiver)
	{
		VARIANT_BOOL bRet;
		bRet = VARIANT_TRUE;
		bRet = m_Receiver->ShowPropertyDialog();
	}
}

/************************************************************************/
/*   Open serial port 
     
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnBnClickedButtonStart()
{
	if(m_Receiver)
	{
		VARIANT_BOOL bRet = VARIANT_FALSE;
		bRet = m_Receiver->StartTracking();
		if(bRet == VARIANT_TRUE)
		{
			::MessageBox(NULL, _T("Open Successed"), _T("Tip"), MB_ICONINFORMATION);

			/*******************************************************************
			//Open HCReceiver Log 
			//You Can Open for Debug, Do not use it for Relese.
			/******************************************************************/
			CString strPath;
			strPath = m_strProjectPath;
			strPath += _T("Log.txt");
			m_Receiver->EnableDebugLog(strPath.AllocSysString());
			
		}
		else
		{
			MessageBox(_T("Open Failed"), _T("Tip"), MB_ICONINFORMATION);
		}
	}
}

/************************************************************************/
/* Close serial port                                                                    
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnBnClickedButtonStop()
{
	if(m_Receiver)
	{
		VARIANT_BOOL bRet;
		bRet = VARIANT_TRUE;
		bRet = m_Receiver->IsTracking();
		if(bRet == VARIANT_TRUE)
		{
			m_Receiver->StopTracking();
			m_Receiver->DisableDebugLog();//Close log file if you call m_Receiver->EnableDebugLog£¨£©
		}
	}
}

/************************************************************************/
/* CORS relevent information                                                                      
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnBnClickedButtonSetCors()
{
	if(m_Receiver)
	{
		CComPtr<IInternetSettings>	InternetSettins;
		CComPtr<IDGPSSettings>		DGPSSettins;

		m_Receiver->get_DGPSSettings(&DGPSSettins);
		if(DGPSSettins)
		{
			DGPSSettins->get_InternetSettings(&InternetSettins);
			if(InternetSettins)
			{
				/**@brief< Set the current differential source type as InternetDGPS*/
				ReceiverDGPSSource DGPSSource = InternetDGPSSource;
				DGPSSettins->get_Source(&DGPSSource);
				if(DGPSSource != InternetDGPSSource)
				{
					DGPSSettins->put_Source(InternetDGPSSource);
				}

				/**@brief< Load login information from the configuration file */
				InternetSettins->LoadConfigFromFile(m_strConfigPath.AllocSysString());
				InternetSettins->ShowPropertyDialog();				
			}
		}
	}
}



/************************************************************************/
/*  @GetMode   
   Obtain position information from HCReceiver
    
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnBnClickedButtonGetpos()
{
	if(m_Receiver)
	{
		CComPtr<IGPSPosition> pPostion = NULL;
		VARIANT_BOOL bRet;
		bRet = VARIANT_TRUE;
		bRet = m_Receiver->GetPosition(&pPostion);
		if(bRet == VARIANT_TRUE)
		{
			double dSpeed = 0.0;
			double dCourse = 0.0;
			dSpeed = pPostion->GetSpeed();
			dCourse = pPostion->GetCourse();
			CString strTmp;
			strTmp.Format(_T("L:%f\rB:%f\rH:%f"), pPostion->GetLongitude(), pPostion->GetLatitude(), pPostion->GetAltitude());
			MessageBox(strTmp, _T("Tip"), NULL);
		}
	}
}

/******************************************************
     @GetMode 
*******************************************************/
void CHCReceiverTestMFCWMDlg::OnBnClickedButtonUniqueid()
{
	if(m_Receiver)
	{
		BSTR bstrID;
		m_Receiver->get_UniqueID(&bstrID);
		MessageBox(CString(bstrID), _T("Tip"), MB_ICONINFORMATION);
	}
}

/****************************************************
@GetMode 
*****************************************************/
void CHCReceiverTestMFCWMDlg::OnBnClickedButtonReg()
{
	if(m_Receiver)
	{
		BSTR bstrReg;
		ReceiverRegister RegisterStatus;
		bstrReg = m_Receiver->GetRegisterStatus(&RegisterStatus);
		MessageBox(CString(bstrReg), _T("Tip"), MB_ICONINFORMATION);
	}
}
void CHCReceiverTestMFCWMDlg::OnBnClickedButtonTCPDirect()
{
	if(m_Receiver)
	{
		m_DialogTCPDirect.SetDGPSSettings(m_Receiver);
		m_DialogTCPDirect.DoModal();
	}
}

/************************************************************************/
/*   Start recording PPK data                                                      
/************************************************************************/

void CHCReceiverTestMFCWMDlg::OnBnClickedButtonStartPPK()
{
	if (!m_Receiver)
	{
		::MessageBox(NULL, _T("Unkown Error"),NULL, NULL);
		return;
	}

	if (m_Receiver->IsTracking() == VARIANT_FALSE)
	{
		::MessageBox(NULL, _T("Please open Serial ports"), NULL, NULL);
		return;
	}

	if (StartRcrdPPKData())
	{
		::MessageBox(NULL, _T("Start Record PPK Data OK"), NULL, NULL);
	}
	else
	{
		::MessageBox(NULL, _T("Start Record PPK Data Failed"), NULL, NULL);
	}
	return;
}

/************************************************************************/
/*  Stop recording PPK                                                                 
/************************************************************************/
void CHCReceiverTestMFCWMDlg::OnBnClickedButtonStopPPK()
{
	StopRcrdPPKData();
}

/************************************************************************/
/*     Intialize PPK information, including OEM type, device ID, time, etc.
      
/************************************************************************/
void CHCReceiverTestMFCWMDlg::InitPPKFileHead()
{
	if (!m_Receiver)
	{
		return;
	}
	// Add HCN file header, 160 bytes

	// Obtain the board type, so as to make sure the version number of the HCN file header
	CString strVer;
	OEM_TYPE oemType = OEM_TYPE_BD_NOVATEL;
	oemType = m_Receiver->GetGetOEMType();
	switch (oemType)
	{
	case OEM_TYPE_TRIM_TBD:
		strVer = _T("60.0");
		break;
	case OEM_TYPE_BD_NOVATEL:
		strVer = _T("54.0");
		break;
	case OEM_TYPE_BD_HEMISPHERE:
		strVer = _T("70.0");
		break;
	default:
		strVer = _T("54.0");
		break;
	}


	//Device ID
	CString strDeviceUniqueID = m_Receiver->GetUniqueID();
	

	COleDateTime dateTime ;

	double dGPSTime = GetGPSTime();
	if (dGPSTime < 1.0) // Returns 0 is an invalid value, it is no particular meaning comparing with 1, it's just judge whether it is a valid value
	{
		dateTime = COleDateTime::GetCurrentTime(); //Obtain current time
	}
	else
	{
		dateTime = GPSTimeToLocal(dGPSTime);
	}

	CString FileTime;
	FileTime.Format(_T("%d/%d/%d"),dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetDay());

	CString nModel(_T("0"));

	/**@brief< Antenna height, default to 0 in the software */
	double antHigh(0.0);
	
	CString strHigh;
	strHigh.Format(_T("%.3lf"),antHigh);

	/**@brief< Antenna Type, default to 0*/
	CString antType(_T("0"));
	CString measureTo(_T("0"));

	m_Receiver->SetPPKFileHead(strVer.AllocSysString(),strDeviceUniqueID.AllocSysString(),FileTime.AllocSysString(),nModel.AllocSysString(),strHigh.AllocSysString(),antType.AllocSysString(),measureTo.AllocSysString());
}

/*********************************************
 Start recording PPK
/*******************************************/

bool CHCReceiverTestMFCWMDlg::StartRcrdPPKData()
{
	bool rRet = false;
	if (!m_Receiver)
	{
		return rRet;
	}

	if (m_Receiver->GetSavePPKData() == VARIANT_TRUE)
	{
		::MessageBox(NULL, _T("Recording, if you want to Restart, please stop it first"), NULL, NULL);
		return false;
	}

	InitPPKFileHead();

	//Set the file path
	CString ProjectPath;

	TCHAR pathChar[MAX_PATH] = {0};
	memset(pathChar, 0 , sizeof(char) * MAX_PATH);

	//Get exe path
	GetModuleFileName(NULL, pathChar, MAX_PATH);
	CString path(pathChar);
	ProjectPath = path.Left(path.ReverseFind('\\') + 1);

	// Create date strings as the filename
	COleDateTime dateTime;
	double dGPSTime = GetGPSTime();
	if (dGPSTime < 1.0) // Returns 0 is an invalid value, it is no particular meaning comparing with 1, it's just judge whether it is a valid value
	{
		dateTime = COleDateTime::GetCurrentTime(); //Obtain current time
	}
	else
	{
		dateTime = GPSTimeToLocal(dGPSTime);
	}

	CString strDate;
	strDate.Format(_T("%d-%02d-%02d"), dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetDay());
	strDate += _T(".hcn");

	ProjectPath += strDate;

	return m_Receiver->SetSavePPKData(ProjectPath.AllocSysString(),VARIANT_TRUE);
}

/******************************************************
Stop recording
********************************************************/
void CHCReceiverTestMFCWMDlg::StopRcrdPPKData()
{
	if (!m_Receiver)
	{
		return;
	}

	m_Receiver->SetSavePPKData("",VARIANT_FALSE);
}


//GPS To LocalTime
COleDateTime CHCReceiverTestMFCWMDlg::GPSTimeToLocal( double dGPSTime )
{
	COleDateTime dtRes(dGPSTime);
	TIME_ZONE_INFORMATION tzi;
	DWORD dwRet = ::GetTimeZoneInformation(&tzi);
	COleDateTimeSpan dtSpan(0, 0, tzi.Bias, 0);
	dtRes -= dtSpan;
	return dtRes;
}


double CHCReceiverTestMFCWMDlg::GetGPSTime()
{
	double dGpsTime(0.0);

	if (m_Receiver)
	{
		CComPtr<IGPSPosition> m_pGPSPosition = 0;
		VARIANT_BOOL m_pGetPostionPtr = VARIANT_FALSE;
		m_pGetPostionPtr = m_Receiver->GetPosition(&m_pGPSPosition);

		if (!m_pGPSPosition || VARIANT_FALSE == m_pGetPostionPtr )
			return dGpsTime;
		return m_pGPSPosition->GetGPSTime();
	}
	return dGpsTime;
}