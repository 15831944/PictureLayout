/*--------------------------------------------------------------------------------------------------------------------*/
//	Panel.h     -- 大板类声明文件
//	
//	作者：	yuanzb
//	时间：	2019.11.9
//	备注：	
//
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_Panel_H_
#define		_Panel_H_

#include "../Component/Component.h"

// 大板信息
class AFX_EXT_CLASS Panel: public Component
{
public:
	Panel(void);
	Panel(Panel& panel);
	virtual ~Panel(void);

public:
	// 大板接口函数

	// 获取小板
	Component* GetComponent(int NumInPanel);

	// 获取小板数量
	int GetComponentNum(void);						

	// 获取大板利用率
	float GetUtilization(void);						

	// 将大板还原为
	void Recover(vector<Component*>& cpn_list);				

	// 将小板还原
	static void RecoverOneComponent(Component* pCpn);						

	// 是否比另一块大板好
	bool IsBetterThan(Panel* pDstPanel);									

public:
	// 新增

	// 获取排过序的所有需要的小板
	void GetAllNeededComponent_Sorted(vector<Component*>& list);			

	// 面积大优先 获取所有的小板
	void GetAllNeedComponent_AreaLargeFirst(vector<Component*>& cpn_list);	


public:
	// 虚函数

	// 是否有反面信息
	virtual bool HaveDownerFaceInfo(void);											

public:
	int m_PanelID;					// 板件号
	int m_PanelType;				// 大板类型用于分类		原料：0 余料：1	
	float m_OrgLen;					// 原始长				// 坐标转换一定要用原始板件长宽，切记！
	float m_OrgWidth;				// 原始宽				// 坐标转换一定要用原始板件长宽，切记！

	float m_OriginX;				// 原点X
	float m_OriginY;				// 原点Y



	vector<CutPath> m_vCutPath;		//余料切割线段
	vector<CutPath> m_vPreCutPath;	//预切割线段
};


typedef vector<Panel*>	PanelList;


#endif
