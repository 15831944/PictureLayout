// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// NewUI.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"


GdiplusStartupInput m_gdiplusStartupInput; 
ULONG_PTR m_gdiplusToken;  

GdiplusStartupInput* getGdiplusStartupInput()
{
	return &m_gdiplusStartupInput;
}

ULONG_PTR* getgdiplusToken()
{
	return &m_gdiplusToken;
}

