#pragma once
#include "../resource.h"
#include "DlgResult.h"

// CDlgWeiXin �Ի���

  
class  CDlgNewUI : public CDialog
{
	DECLARE_DYNAMIC(CDlgNewUI)

public:
	CDlgNewUI(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgNewUI();

// �Ի�������
	enum { IDD = IDD_DIALOG_NEW_UI };



public:

	CDlgResult* m_pDlgResult;

	CRect GetViewRect();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

};
