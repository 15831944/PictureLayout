// NewUI.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "NewUI.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CNewUIApp

BEGIN_MESSAGE_MAP(CNewUIApp, CWinApp)
END_MESSAGE_MAP()


// CNewUIApp ����

CNewUIApp::CNewUIApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CNewUIApp ����

CNewUIApp theApp;


// CNewUIApp ��ʼ��

BOOL CNewUIApp::InitInstance()
{
	CWinApp::InitInstance();




	GdiplusStartup(/*&m_gdiplusToken*/getgdiplusToken(), /*&m_gdiplusStartupInput*/getGdiplusStartupInput(), NULL);  

	return TRUE;
}


int  CNewUIApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class   
	//GdiplusShutdown(m_gdiplusToken); //�ر�GDI +  

	return CWinApp::ExitInstance(); 
}