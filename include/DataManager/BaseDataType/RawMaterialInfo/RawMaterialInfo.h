/** @brief		ԭ����Ϣͷ�ļ�
 *  @file		RawMaterialInfo.h 
 *  @author		yaunzb
 *  @version	1.0
 *  @date		2018.7.8
 *  @note       
 *  @since    
 */

#ifndef		_RawMaterialInfo_H_
#define		_RawMaterialInfo_H_
#include <string>

using namespace std;

  /**
    * @brief	ԭ����Ϣ \n
    *			ԭ�ϲ��ʡ������۸�
    */
//������Ϣ
class AFX_EXT_CLASS RawMaterialInfo
{
public:
	RawMaterialInfo(void);
	~RawMaterialInfo(void);

public:
	// ԭ����ص���Ϣ
	
	float	m_PanelLength;					/**<  ��峤��		*/
	float	m_PanelWidth;					/**<  �����		*/
	string	m_Material;						/**<  ������		*/
};


#endif