/*--------------------------------------------------------------------------------------------------------------------*/
///	CSingleton.h    --  �����������ļ�
//	
//	���ߣ�	yuanzb
//	ʱ�䣺	2019.9.8
//	��ע��	�㷨��Ҫ���ݽṹ
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
#pragma  message(".......................................��ǰΪdebug�汾")

#else

#pragma comment(lib, "../../lib/tinyxml.lib")
#pragma  message(".......................................��ǰΪrelease�汾")

#endif




class Panel;
class Component;


  /**
    * @brief	������ݹ����� \n
    *			ʹ�õ���ģʽ����������ĵ����ݽṹ
    */
class AFX_EXT_CLASS CSingleton
{
public:
	/** 
	* @brief ������
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
	// �ӿں���
	int Layout(int Method, int CutStyle, int Org);				/**< �����Ż�	*/ 
	void ReOptimizeSln(CSolution* pSln, int Org);				/**< �������ʵ͵İ����´�ɢ����	*/ 
	
	void ClearAllData(void);									/**< �����������	*/ 
	void ClearCurrentSolution(void);							/**< �����ǰ�Ż�����	*/ 
	void ClearBackupSolution(void);								/**< ��������Ż�����	*/ 
	void ClearRemainderManager(void);							/**< ������Ϲ�����	*/ 
	void SetBackupSolution(vector<CSolution*> &sln_list);		/**< ���ñ����Ż�����	*/ 
	
	void UpdateSlnNum(void);									/**< ���½��������	*/ 
	void UpdateComponentMachiningInfo(void);					/**< ���°�������㡢�ײ۵ȼӹ���Ϣ���Ű�����п����еİ������ת�ˣ������Ű�֮����ת���İ���������㡢�ײ�ҲҪ��֮��תһ�£�	*/ 
	void UpdatePreCombinedComponent(void);						/**< ����Ԥ��ϰ������������������Ԥ��ϰ����һЩ�ֶ�	*/// 
	
public:															
	// ������ؽӿ�											    
	int GetCurSolutionPanelNum(void);							/**< ��ȡ��ǰ�Ż������������	*/ 
	int GetBackupSolutionPanelNum(void);						/**< ��ȡ��ǰ�Ż������������	*/
	void BackupBestSolution(void);								/**< �������Ž������	*/ 
	bool BackupBetterSolution(int index);							/**< �������Ž������	*/ 


	int GetCurSolutionNum(void);								/**< ��ȡ��ǰ�Ż���������	*/ 
	int GetBackupSolutionNum(void);								/**< ��ȡ��ǰ�Ż���������	*/

public:
	// ԭʼ��Ϣ�ӿ�

	void SetBackupComponentInputItem(vector<ComponentInputItem>& InputItem);	/**< ��������С��ӿ�	*/
	void SetRawMaterialInfoList(vector<RawMaterialInfo>& InputItem);			/**< ԭ����Ϣ����ӿ�	*/

	void SetUsableRawMaterialInfoList(vector<RawMaterialInfo>& InputItem)	;	/**< ԭ����Ϣ����ӿ�	*/


public:
	// �Ż���������
	int m_CurSlnIndex;
	BaseInfo m_BaseInfo;														/**< ������Ϣ		*/ 
	vector<CSolution*> m_CurrentSolutionList;									/**< ��ǰ�Ż�����	*/ 
	vector<CSolution*> m_BackupSolutionList;									/**< �����Ż�����	*/ 
	vector<vector<Component*>> m_vComponentGroup;								/**< С����			*/ 

	vector<RawMaterialInfo> m_vRawMaterialList;									/**< ԭ����Ϣ����	*/ 
	
	vector<ComponentInputItem> m_vBackupComponentInputItem;						/**< ����С�屸��	*/ 
	RemainderManager m_RemainderManager;										/**< ���Ϲ�����	*/ 
	UINT m_uComponentInputItemSerialID;											/**< ���а�����кţ����ڱ�ʶÿһ�����а��	*/ 


public:
	// ����ͼƬ�����ӿ�
	int New_Layout(int Method, int CutStyle, int Org);				/**< �������Ż�	*/ 



};
#endif