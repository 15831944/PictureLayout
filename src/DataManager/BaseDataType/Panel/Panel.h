/*--------------------------------------------------------------------------------------------------------------------*/
//	Panel.h     -- ����������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2016.11.9
//	��ע��	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_Panel_H_
#define		_Panel_H_

#include "../Component/Component.h"

// �����Ϣ
class AFX_EXT_CLASS Panel: public Component
{
public:
	Panel(void);
	Panel(Panel& panel);
	virtual ~Panel(void);

public:
	// ���ӿں���

	// ��ȡС��
	Component* GetComponent(int NumInPanel);

	// ��ȡС������
	int GetComponentNum(void);						

	// ��ȡ���������
	float GetUtilization(void);						

	// ����廹ԭΪ
	void Recover(vector<Component*>& cpn_list);				

	// ��С�廹ԭ
	static void RecoverOneComponent(Component* pCpn);						

	// �Ƿ����һ�����
	bool IsBetterThan(Panel* pDstPanel);									

public:
	// ����

	// ��ȡ�Ź����������Ҫ��С��
	void GetAllNeededComponent_Sorted(vector<Component*>& list);			

	// ��������� ��ȡ���е�С��
	void GetAllNeedComponent_AreaLargeFirst(vector<Component*>& cpn_list);	


public:
	// �麯��

	// �Ƿ��з�����Ϣ
	virtual bool HaveDownerFaceInfo(void);											

public:
	int m_PanelID;					// �����
	int m_PanelType;				// ����������ڷ���		ԭ�ϣ�0 ���ϣ�1	
	float m_OrgLen;					// ԭʼ��				// ����ת��һ��Ҫ��ԭʼ��������мǣ�
	float m_OrgWidth;				// ԭʼ��				// ����ת��һ��Ҫ��ԭʼ��������мǣ�

	float m_OriginX;				// ԭ��X
	float m_OriginY;				// ԭ��Y



	vector<CutPath> m_vCutPath;		//�����и��߶�
	vector<CutPath> m_vPreCutPath;	//Ԥ�и��߶�
};


typedef vector<Panel*>	PanelList;


#endif
