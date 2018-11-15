/** @brief ����д���ժҪ
 *  @file ����ļ���
 *  @author ˭tmd�ĸ��
 *  @version �汾��
 *  @date  ��ɶʱ����
 *  @note       ע��. ����: ���ļ���ʲô���幦�ܰ�,ʹ��ʱҪע��ʲô����..
 *  @since    �Դ�  ����: �Դ�2012�����ը������ļ��ʹӵ�������ʧ�ˡ�..
 */


#pragma once

#include "../resource.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/DataManager/BaseDataType/RawMaterialInfo/RawMaterialInfo.h"
#include "../../../include/DataManager/BaseDataType/BaseInfo/BaseInfo.h"
#include "listboxclipboard.h"
#include "./UICommon/DialogChildBase.h"
#include "./UICommon/PngButton.h"
#include "../UIData/UIData.h"
#include "../Misc/OtherShapeHelper.h"

// CDlgResult �Ի���

class CDlgTotalResult;
class Component;
class PanelViewingParam;

#define WM_REFRESH_PANEL_VIEW               WM_USER+1


#define DEFAULT_WIDTH						(1000000) 


#define IDTIMER1  1



 /** 
    * @brief ��Ҫ˵������ 
    */
enum RemainderCutingSettingState
{
	CUTTING_SETTING_STATE_START,		/**< ԭ�Ͽ�� */
	CUTTING_SETTING_STATE_END			/**< ԭ�Ͽ�� */
};


  /**
    * @brief ��ļ򵥸��� \n(����)
    * �����ϸ����
    */
class CDlgResult : public CDialogChildBase
{
	DECLARE_DYNAMIC(CDlgResult)

public:
	CDlgResult(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgResult();

// �Ի�������
	enum { IDD = IDD_DIALOG_RESULT };

	void ResetResultDlg();
	void RefreshOptimizeResult();
	void StopDrawing();
	vector<Panel*> GetPanels(bool bChecked);
	bool IsPanelChecked(Panel* pPanel);
	PanelViewingParam* GetSelectedItemViewingParam();
	void ResetPastingState();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT OnRefreshPanelView(WPARAM wparam, LPARAM lparam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMenuResetMachiningOrder();
	afx_msg void OnMenuCutComponent();
	afx_msg void OnMenuCopyComponent();
	afx_msg void OnMenuPasteComponent();
	afx_msg void OnMenuRemoveComponent();
	afx_msg void OnMenuRemainderCutting();
	afx_msg void OnMenuRotatePastingComponent();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer( UINT nIDEvent );

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	afx_msg void OnLbnSelchangeClipBoard();
	afx_msg LRESULT OnShowOrHideClipboard(WPARAM wparam, LPARAM lparam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

	CRect GetPanelViewRect();
	
	void DrawPanel(CDC* pDC, Panel* pPanel, CRect rcDrawArea, PanelViewingParam& param);
	void DrawOneLine(Graphics& g, CRect rcPanel, float fScale, PointInfo& vertexStart, PointInfo& vertexEnd, Color color = Color::Red);
	
	void DrawMachiningInfo(Graphics& g, CRect rcPanel, float fScale, Component* pComponent);
	void DrawDetail(Graphics& g, CRect rcComponent, float fScale, Component* pComponent);
	void DrawRemainderCuting(Graphics& g, CRect rcPanel, float fScale, Panel& thePanel);
	
	CRect GetComponentUIRect(float fPanelXLen, float fPanelYLen, float fComponentX, float fComponentY, float fComponentXLen, float fComponentYLen);
	float GetDrawingScale(float fPanelXLen, float fPanelYLen);

	void AddComponentToClipboard(Component* pComponent);
	

	void ShowOrHideClipboard();
	void DrawComponentToPos(Component* pComponent, CPoint ptComponent, bool bDrawOKIcon, Panel* pPanel);
	bool IsPosCanPasteComponent(Panel* pPanel, Component* pComponent, PointFloat ptComponentUILeftTop);
	PointFloat ConvertUIPosToPanelPos(Panel* pPanel, CPoint ptInUI);
	CPoint ConvertPanelPosToUIPos(Panel* pPanel, PointFloat ptInPanel);
	void CancelPasting();
	bool IsAttachMode();

	PointFloat GetAttachPanelPos_Component(CPoint ptMouseInUI, float fComponentXLen, float fComponentYLen);
	void RotatePastingComponent();
	void PastingComponent_OnMouseMove(CPoint point);

	void DrawRemainderCuttingToPos(CutPath& theRemainderCutting, Panel* pPanel);
	void StopRemainderCutting();
	PointFloat GetAttachPanelPos_RemainderCutting_Start(CPoint ptMouseInUI);
	PointFloat GetAttachPanelPos_RemainderCutting_End(CPoint ptMouseInUI, PointFloat ptStartInPanel);

	CDlgTotalResult* m_pDlgTotalResult;

	CPoint m_ptRClicked;

	CListBoxClipBoard m_lbClipBoard;
	Component* m_pPastingComponent;
	bool m_bNeedToEraseOldPastingComponent;
	CPoint m_ptOldPastingComponent;
	bool m_bOldPastingComponentOK;
	PointFloat m_ptfAttachPosInPanel;

	CutPath* m_pRemainderCutting;
	bool m_bNeedToEraseOldRemainderCutting;
	int m_nRemainderCuttingSettingState;



public:


	/** 
	* @brief �����������
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	void ClearAllData();


	/** 
	* @brief �����������
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	afx_msg void OnOpenSolution();

	/** 
	* @brief �����������
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	afx_msg void OnLayout();

	/** 
	* @brief ��ͼƬ��Ϣ
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	afx_msg void OnOpenSourcePicInfo();

	/** 
	* @brief ���������ַ���
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	void  InputNormalString(string str);

	/** 
	* @brief ������������
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	bool CopyToClipboard(const char* pszData, const int nDataLen);


	/** 
	* @brief ��������Խ���̩
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	afx_msg void OnConnectMaintop();

	void setEditCtrlString(int pos_x, int pos_y, string str, int sleep_time = 0);

	static void SetBottomAction(CString title, int id, UINT action);
	static void SetCtrlText(CString title, int id,  CString ctrl_type, CString text);

	static void EmptyCtrlContent(HWND hWnd);
	static CWnd* FindWndByCtrlID(CWnd* pWnd, UINT nCtrlID, LPCTSTR szClassName);
	static int FindWindowLike(CString& namelink);
	static BOOL CALLBACK EnumWindowsLikeProc(HWND hwnd,LPARAM lParam);

	/** 
	* @brief �Ż�
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	void OnOptimize();

	/** 
	* @brief ���
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	void CheckAndDeleteOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem);

	/** 
	* @brief ���ԭ��
	* @author yuanzb
	* @return 
	* @note
	* 
	* @since 1.0
	*/
	bool CheckRawMaterialUsable(vector<ComponentInputItem>& vComponentInputItem, RawMaterialInfo rm_info, BaseInfo b_info);

	vector<PreCombineItem> m_vPreCombineItem;
	vector<ComponentInputItem> m_vComponentInputItem;						/**< ����������	*/ 
	vector<RawMaterialInfo> m_vRawMaterialList;								/**< ԭ����Ϣ����	*/ 
	BaseInfo m_BaseInfo;											/**< ԭ����Ϣ����	*/ 

	
	float m_len;				/**< ԭ�ϳ��� */
	float m_width;				/**< ԭ�Ͽ�� */

	float m_x_space;			/**<  X�����϶		*/
	float m_y_space;			/**<  Y�����϶		*/
	float m_left_offset;		/**<  ��Ԥ����϶	*/
	float m_right_offset;		/**<  ��Ԥ����϶	*/
	float m_top_offset;			/**<  ��Ԥ����϶	*/
	float m_bottom_offset;		/**<  ��Ԥ����϶	*/


	CComboBox control_arranging_origin;
	int m_arranging_origin;		/**<  ����ԭ��	*/


	string m_strSrcFilePath;
	bool open_rt;



};
