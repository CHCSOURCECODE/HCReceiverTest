/**
*	@file		d:\SVN本地副本\SDK\trunk\HCReceiver\HCReceiverEventDispIDs.h
*   @brief
*	@author		
*	@version	
*	@note		该文件用于在MFC中使用 BEGIN_DISPATCH_MAP / DISP_FUNCTION_ID / END_DISPATCH_MAP 宏接收事件
*	@date		2012:11:19	18:20:56
*/

#if !defined(HCReceiverEventDispIDs_h)
#define HCReceiverEventDispIDs_h


//IReceiverEvents dispatch IDs
#define DISPID_GPSRECEIVER_NewPosition						1
#define DISPID_GPSRECEIVER_NewConstellationInformation      2
#define DISPID_GPSRECEIVER_NewCourseAndSpeed                3
#define DISPID_GPSRECEIVER_NewHDOP                          4
#define DISPID_GPSRECEIVER_NewVDOP                          5
#define DISPID_GPSRECEIVER_NewHRMS                          6
#define DISPID_GPSRECEIVER_NewVRMS                          7
#define DISPID_GPSRECEIVER_NewQualityIndicator              8
#define DISPID_GPSRECEIVER_NewGPSStatus                     9
#define DISPID_GPSRECEIVER_NewDGPSStatus                    10
#define DISPID_GPSRECEIVER_NewGPSErrorCode                  11
#define DISPID_GPSRECEIVER_NewGPSTime                       12
#define DISPID_GPSRECEIVER_NewGPGGA                         13
#define DISPID_GPSRECEIVER_NewPDOP                          14
//#define DISPID_GPSRECEIVER_NewObservation                   15
//#define DISPID_GPSRECEIVER_NewEphemeris                     16

#endif