/*--------------------------------------------------------------------------------------------------------------------*
///	Outline.h   --  �������������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2019.11.8
//	��ע��	���ڸĽ��������������㷨	L(x1, x2, y),�Ժ���ܸ��������Ϊ�����������
//	
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_Outline_H_ 
#define		_Outline_H_

class	Panel;
class	Component;

class AFX_EXT_CLASS Outline
{
public:
	typedef enum
	{
		OutlineType_Horizon = 0,
		OutlineType_Vertical

	}OutlineType;


public:
	Outline();
	virtual ~Outline();

public:
	// ����������
	bool operator> (const Outline& ln) const ;		// >����������
	bool operator< (const Outline& ln) const;		// <����������
	bool operator== (const Outline& ln) const;		// ==����������





public:
	// �����ӿ�
	float GetLength(void) const;					// ��ȡ����
	float GetWidth(void)const;					// ��ȡ���
	void SetOutline(float start_x, float end_x, float start_y, float end_y, Component* pParent);	// ����������
	bool Containable(Component* pCpn);		// �ܷ�����
	bool IsLegal(void);						// �������Ƿ�Ϸ�

public:
	// ��Ա����
	int m_index;
	OutlineType m_type;
	float m_start_x;	// ���������x����
	float m_start_y;	// ���������y����
	float m_end_x;		// �������յ�x����
	float m_end_y;		// �������յ��յ�x


	Component* m_pParent;	// ���ָ�룬�����Ŀ���

};




#endif