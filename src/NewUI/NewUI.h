// NewUI.h : NewUI DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������






// �ж��Ƿ��ڲ��Ի�����
#ifdef DEBUG 

#pragma comment(lib, "../../lib/DataManagerd.lib")
#pragma comment(lib, "../../lib/FileReadWrited.lib")
#pragma comment(lib, "../../lib/tinyxmld.lib")
#pragma comment(lib, "../../lib/muParserd.lib")
#pragma comment(lib, "../../lib/AbnormalShapeManagerd.lib")

#pragma  message(".......................................��ǰΪdebug�汾")

#else

#pragma comment(lib, "../../lib/DataManager.lib")
#pragma comment(lib, "../../lib/FileReadWrite.lib")
#pragma comment(lib, "../../lib/tinyxml.lib")
#pragma comment(lib, "../../lib/muParser.lib")
#pragma comment(lib, "../../lib/AbnormalShapeManager.lib")

#pragma  message(".......................................��ǰΪrelease�汾")


#endif








// CNewUIApp
// �йش���ʵ�ֵ���Ϣ������� NewUI.cpp
//

class CNewUIApp : public CWinApp
{
public:
	CNewUIApp();



public:  
	//����ExitInstance  
	virtual int ExitInstance();   

	// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
