/*--------------------------------------------------------------------------------------------------------------------*/
///	CSingleton.h    --  单例类声明文件
//	
//	作者：	yuanzb
//	时间：	2018.9.8
//	备注：	算法主要数据结构
//	
/*--------------------------------------------------------------------------------------------------------------------*/

#ifndef		_CSingleton_H_ 
#define		_CSingleton_H_

#include <vector>
#include "../RawMaterialInfo/RawMaterialInfo.h"
#include "../PrincipleInfo/PrincipleInfo.h"
#include "../BaseInfo/BaseInfo.h"
#include "../CommonData/CommonData.h"
#include "../RemainderManager/RemainderManager.h"
#include "../RemainderGroup/RemainderGroup.h"



#include "../../../../include/TinyXml/tinyxml.h"

using namespace TinyXml;


#ifdef DEBUG 

#pragma comment(lib, "../../lib/tinyxmld.lib")
#pragma  message(".......................................当前为debug版本")

#else

#pragma comment(lib, "../../lib/tinyxml.lib")
#pragma  message(".......................................当前为release版本")

#endif




class Panel;
class Component;


  /**
    * @brief	软件数据管理器 \n
    *			使用单例模式，管理最核心的数据结构
    */
class AFX_EXT_CLASS CSingleton
{
public:
	/** 
	* @brief 单例类
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	~CSingleton();
	static CSingleton* GetSingleton(void);

private:
	CSingleton();
	static CSingleton* m_pSingleton;

public:
	// 接口函数
	int Layout(int Method, int CutStyle, int Org);				/**< 排样优化	*/ 
	void ReOptimizeSln(CSolution* pSln, int Org);				/**< 对利用率低的板重新打散排样	*/ 
	
	void ClearAllData(void);									/**< 清除所有数据	*/ 
	void ClearCurrentSolution(void);							/**< 清除当前优化方案	*/ 
	void ClearBackupSolution(void);								/**< 清除备份优化方案	*/ 
	void ClearRemainderManager(void);							/**< 清除余料管理器	*/ 
	void SetBackupSolution(vector<CSolution*> &sln_list);		/**< 设置备份优化方案	*/ 
	
	void UpdateSlnNum(void);									/**< 更新解决方案号	*/ 
	void UpdateComponentMachiningInfo(void);					/**< 更新板件轮廓点、孔槽等加工信息（排板过程中可能有的板件被旋转了，所以排板之后被旋转过的板件的轮廓点、孔槽也要随之旋转一下）	*/ 
	void UpdatePreCombinedComponent(void);						/**< 更新预组合板件：根据排样结果填充预组合板件的一些字段	*/// 
	
public:															
	// 备份相关接口											    
	int GetCurSolutionPanelNum(void);							/**< 获取当前优化方案大板数量	*/ 
	int GetBackupSolutionPanelNum(void);						/**< 获取当前优化方案大板数量	*/
	void BackupBestSolution(void);								/**< 保存最优解决方案	*/ 
	bool BackupBetterSolution(int index);							/**< 保存最优解决方案	*/ 


	int GetCurSolutionNum(void);								/**< 获取当前优化方案数量	*/ 
	int GetBackupSolutionNum(void);								/**< 获取当前优化方案数量	*/

public:
	// 原始信息接口

	void SetBackupComponentInputItem(vector<ComponentInputItem>& InputItem);	/**< 备份输入小板接口	*/
	void SetRawMaterialInfoList(vector<RawMaterialInfo>& InputItem);			/**< 原料信息链表接口	*/

	void SetUsableRawMaterialInfoList(vector<RawMaterialInfo>& InputItem)	;	/**< 原料信息链表接口	*/


public:
	// 优化方案数据
	int m_CurSlnIndex;
	BaseInfo m_BaseInfo;														/**< 基本信息		*/ 
	vector<CSolution*> m_CurrentSolutionList;									/**< 当前优化方案	*/ 
	vector<CSolution*> m_BackupSolutionList;									/**< 备份优化方案	*/ 
	vector<vector<Component*>> m_vComponentGroup;								/**< 小板组			*/ 

	vector<RawMaterialInfo> m_vRawMaterialList;									/**< 原料信息链表	*/ 
	
	vector<ComponentInputItem> m_vBackupComponentInputItem;						/**< 输入小板备份	*/ 
	RemainderManager m_RemainderManager;										/**< 余料管理器	*/ 
	UINT m_uComponentInputItemSerialID;											/**< 需切板件序列号，用于标识每一块需切板件	*/ 


public:
	// 新增图片排样接口
	int New_Layout(int Method, int CutStyle, int Org);				/**< 新排样优化	*/ 



};
#endif