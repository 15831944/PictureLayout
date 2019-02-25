#ifndef		_JtyReadWrite_h_
#define		_JtyReadWrite_h_
#pragma  once

#include "../../../include/TinyXml/tinyxml.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"

#include <vector>
#include <string>

class Component;

using namespace std;
using namespace TinyXml;

class AFX_EXT_CLASS  JtyReadWrite
{
public:
	//	��׼��д����
	static bool ReadJty(CString szFileDir);										// ����jty
	static bool WriteJty(CString szFileDir);									// ����Ϊjty
	static bool SaveOneTree2Xml(TiXmlElement* pCurNode, Component* pCurCpn);	// ���������νṹ��XML�ļ�
	static bool LoadOneTreeFromXml(TiXmlElement* pCurNode, Component* pCurCpn);	// ��XML�ļ���ȡ������ͽṹ

	// ���������Ρ��ײ�����

	static void SaveOutline(TiXmlElement* pCpnElem, Component* pCpn);			// ��������������
	

	// ��ȡ������Ρ��ײ�����
	static void ReadOutlineHoleSlotInfo(TiXmlElement* pCurCpnNode, Component* pNewCpn);// ��ȡ������Ρ��ײ�����
	static void ReadOutline(TiXmlElement* pCpnElem, Component* pCpn);			// ��ȡ�����������
	





};

#endif