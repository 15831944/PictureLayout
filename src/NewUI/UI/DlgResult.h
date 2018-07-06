/** @brief 这里写你的摘要
 *  @file 你的文件名
 *  @author 谁tmd的搞的
 *  @version 版本了
 *  @date  你啥时候搞的
 *  @note       注解. 例如: 本文件有什么具体功能啊,使用时要注意什么啊…..
 *  @since    自从  例如: 自从2012年地球爆炸后这个文件就从地球上消失了…..
 */


#pragma once

#include "../resource.h"
#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "listboxclipboard.h"
#include "./UICommon/DialogChildBase.h"
#include "./UICommon/PngButton.h"

// CDlgResult 对话框

class CDlgTotalResult;
class Component;
class PanelViewingParam;

#define WM_REFRESH_PANEL_VIEW               WM_USER+1

 /** 
    * @brief 简要说明文字 
    */
enum RemainderCutingSettingState
{
	CUTTING_SETTING_STATE_START,		/**< 原料宽度 */
	CUTTING_SETTING_STATE_END			/**< 原料宽度 */
};


  /**
    * @brief 类的简单概述 \n(换行)
    * 类的详细概述
    */
class CDlgResult : public CDialogChildBase
{
	DECLARE_DYNAMIC(CDlgResult)

public:
	CDlgResult(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgResult();

// 对话框数据
	enum { IDD = IDD_DIALOG_RESULT };

	void ResetResultDlg();
	void RefreshOptimizeResult();
	void StopDrawing();
	vector<Panel*> GetPanels(bool bChecked);
	bool IsPanelChecked(Panel* pPanel);
	PanelViewingParam* GetSelectedItemViewingParam();
	void ResetPastingState();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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

	/**  这里写这个函数是干什么用的
	@param[in]		输入参数1
	@param[in]		输入参数2
	@param[out]		输出参数1
	@return			返回值解释一下
	@warning		警告: 例如: 参数不能为空啊,内存要外部释放之类的费话
	@note			注解  随便你了
	@see            相当于是请参考xxoo函数之类的
	*/
	void ClearAllData();
	afx_msg void OnOpenSolution();
	afx_msg void OnLayout();
	afx_msg void OnOpenSourcePicInfo();

	void OnOptimize();

	void CheckAndDeleteOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem);


	vector<ComponentInputItem> m_vComponentInputItem;

	
	float m_width;				/**< 原料长度 */
	float m_height;				/**< 原料宽度 */

	float m_x_space;			/**< 锯缝 X方向间隙		*/
	float m_y_space;			/**< 锯缝 Y方向间隙		*/
	float m_left_offset;		/**< 锯缝 左预留间隙	*/
	float m_right_offset;		/**< 锯缝 右预留间隙	*/
	float m_top_offset;			/**< 锯缝 上预留间隙	*/
	float m_bottom_offset;		/**< 锯缝 下预留间隙	*/


	CComboBox control_arranging_origin;
	int m_arranging_origin;



};
