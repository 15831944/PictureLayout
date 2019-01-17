

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
	int		m_LayoutOrg;						/**<  ����ԭ��		0�����½�  1�����Ͻ�  2�����½� 3�����Ͻ�*/
	int		m_LayoutMethod;						/**<  ��������		 0��ʡ������ 1��������� 2���ͻ����� 3������ģʽ*/	
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
	float	m_UpHeight;							/**< ��ȫ�߶�					*/	
	float	m_DownHeight;							/**< �µ����					*/
	float	m_org_offset;							/**< 							*/
	float	m_distance1;							/**< �µ�ƫ��1					*/
	float	m_distance2;							/**< �µ�ƫ��2					*/
	float	m_MinLength;							/**< ��С������ - ��С����	*/
	float	m_MinWidth;							/**< ��С������ - ��С���	*/
	float	m_MinArea;							/**< ��С������ - ��С���	*/
																				
public:																			
	// �Ż���ز���															 
																				
	int m_FirstSectionOPTimes;					/**<  ��һ�׶��Ż�����			*/
	int m_FirstSectionOPMethod;					/**<  ��һ�׶��Ż�����			*/
	int m_SecondSectionOPTimes;					/**<  �ڶ��׶��Ż�����			*/
	int m_ThirdSectionOPTimes;					/**<  �����׶��Ż�����			*/
	float m_ThirdSectionOAccptableUtilization;	/**<  �����׿ɽ����Ż���		   */
												
	bool m_bDownerFaceFirst;					/**<  �з�����Ϣ��������		   */
	bool m_bCustomerFirst;			

public:
	// ������� 
	int m_FileTextPosition;						/**<  �ļ���λ��		0������  1������  2������ 3������  4������  5������*/
	int m_AutoSpace;							/**<  ���ܼ��			0��������  1������ */
	int m_OneLabel;								/**<  ��ǩֻ��һ��		0��������  1������  */
	int m_OneLabelSpace;						/**<  ��ǩֻ��һ��ʱ��ļ��  */
	int m_PositionSignType;						/**<  ��λ������		0��Բ  1: ʮ��*/
	float m_PositionSignSize;					/**<  ��λ����С		mm  */
	float m_PositionSignDist;					/**<  ��λ��			��λ�����Ϊ1000mmʱ��dxf�ļ�ÿ��1000mm��ʾһ��Բ  */
	float m_PositionSignOffset;					/**<  ��λ��ƫ��		��λ������ֽ�ŵļ��  */
	float m_FontSize;							/**<  �����Сpt		��=pt/7   ��= pt/3  20pt,ÿ���ַ���3mm ��7mm */

public:
	string m_strMainTopPath;



};


#endif