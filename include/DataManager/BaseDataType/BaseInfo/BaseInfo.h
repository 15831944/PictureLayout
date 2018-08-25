

#ifndef		_BaseInfo_H_
#define		_BaseInfo_H_
#include <string>

using namespace std;


//������Ϣ
class AFX_EXT_CLASS BaseInfo
{
public:
	BaseInfo(void);
	~BaseInfo(void);

public:
	// ������ص���Ϣ
	bool    m_LengthUnlimited;					/**<  ����������	*/
	bool    m_WidthUnlimited;					/**<  ���������	*/
	int		m_LayoutOrg;						/**<  ����ԭ��		*/
	int		m_LayoutMethod;						/**<  ��������		*/
	float	m_PanelLength;						/**<  ��峤��		*/
	float	m_PanelWidth;						/**<  �����		*/

	float	m_x_space;							/**<  X�����϶		*/
	float	m_y_space;							/**<  Y�����϶		*/
	float	m_left_offset;						/**<  ��Ԥ����϶	*/
	float	m_right_offset;						/**<  ��Ԥ����϶	*/
	float	m_top_offset;						/**<  ��Ԥ����϶	*/
	float	m_bottom_offset;					/**<  ��Ԥ����϶	*/

	float	m_SawKerfWidth;						/**< ��� ��������				*/
	float	m_DeburringWidth;					/**<  �ޱ߾��� 4�߶���			*/
	float m_UpHeight;							/**< ��ȫ�߶�					*/	
	float m_DownHeight;							/**< �µ����					*/
	float m_org_offset;							/**< 							*/
	float m_distance1;							/**< �µ�ƫ��1					*/
	float m_distance2;							/**< �µ�ƫ��2					*/
	float m_MinLength;							/**< ��С������ - ��С����	*/
	float m_MinWidth;							/**< ��С������ - ��С���	*/
	float m_MinArea;							/**< ��С������ - ��С���	*/
																				
public:																			
	// �Ż���ز���															 
																				
	int m_FirstSectionOPTimes;					/**<  ��һ�׶��Ż�����			*/
	int m_FirstSectionOPMethod;					/**<  ��һ�׶��Ż�����			*/
	int m_SecondSectionOPTimes;					/**<  �ڶ��׶��Ż�����			*/
	int m_ThirdSectionOPTimes;					/**<  �����׶��Ż�����			*/
	float m_ThirdSectionOAccptableUtilization;	/**<  �����׿ɽ����Ż���		   */
												
	bool m_bDownerFaceFirst;					/**<  �з�����Ϣ��������		   */

public:
	string m_strMainTopPath;



};


#endif