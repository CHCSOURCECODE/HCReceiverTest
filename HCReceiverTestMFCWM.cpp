// HCReceiverTestMFCWM.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "HCReceiverTestMFCWM.h"
#include "HCReceiverTestMFCWMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHCReceiverTestMFCWMApp

BEGIN_MESSAGE_MAP(CHCReceiverTestMFCWMApp, CWinApp)
END_MESSAGE_MAP()


// CHCReceiverTestMFCWMApp ����
CHCReceiverTestMFCWMApp::CHCReceiverTestMFCWMApp()
	: CWinApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CHCReceiverTestMFCWMApp ����
CHCReceiverTestMFCWMApp theApp;

// CHCReceiverTestMFCWMApp ��ʼ��

BOOL CHCReceiverTestMFCWMApp::InitInstance()
{
    // ��Ӧ�ó����ʼ���ڼ䣬Ӧ����һ�� SHInitExtraControls �Գ�ʼ��
    // ���� Windows Mobile �ض��ؼ����� CAPEDIT �� SIPPREF��
    SHInitExtraControls();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CHCReceiverTestMFCWMDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
