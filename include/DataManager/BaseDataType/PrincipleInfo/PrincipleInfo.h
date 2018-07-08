/** @brief		������Ϣͷ�ļ�
 *  @file		PrincipleInfo.h 
 *  @author		yaunzb
 *  @version	1.0
 *  @date		2018.7.8
 *  @note       
 *  @since    
 */

#ifndef		_PrincipleInfo_H_
#define		_PrincipleInfo_H_
#include <string>

using namespace std;

  /**
    * @brief	ԭ����Ϣ \n
    *			ԭ�ϲ��ʡ������۸�
    */
//������Ϣ
class AFX_EXT_CLASS PrincipleInfo
{
public:
	PrincipleInfo(void);
	~PrincipleInfo(void);

public:

	int m_LayoutOrg;							/**<  ����ԭ��			*/
	int m_LayoutMethod;							/**<  ��������			*/
	float m_x_space;							/**< ��� X�����϶		*/
	float m_y_space;							/**< ��� Y�����϶		*/
	float m_left_offset;						/**< ��� ��Ԥ����϶	*/
	float m_right_offset;						/**< ��� ��Ԥ����϶	*/
	float m_top_offset;							/**< ��� ��Ԥ����϶	*/
	float m_bottom_offset;						/**< ��� ��Ԥ����϶	*/
};


#endif