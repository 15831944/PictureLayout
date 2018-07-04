// DlgWeiXin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewUIDlg.h"
// #include "afxdialogex.h"
// #include "../Misc/Misc.h"


// CDlgWeiXin �Ի���

IMPLEMENT_DYNAMIC(CDlgNewUI, CDialog)

CDlgNewUI::CDlgNewUI(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewUI::IDD, pParent)
	
{

}

CDlgNewUI::~CDlgNewUI()
{

}

void CDlgNewUI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgNewUI, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgWeiXin ��Ϣ�������
BOOL CDlgNewUI::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pDlgResult = new CDlgResult(this);
	m_pDlgResult->Create(CDlgResult::IDD, this);



	
	m_pDlgResult->ShowWindow(SW_SHOW);
	//m_pDlgResult->MoveWindow(/*0, 300, 500, 500*/GetViewRect());
	CRect rt;
	GetClientRect(rt);

	m_pDlgResult->MoveWindow(rt);
















	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}





CRect CDlgNewUI::GetViewRect()
{
	CRect rcRet;
	CRect rcClient, rcTab;
	GetClientRect(rcClient);
	GetWindowRect(rcTab);
	ScreenToClient(rcTab);
	rcRet.CopyRect(rcClient);
	rcRet.InflateRect(0, -rcTab.Height(), 0, 0);
	return rcRet;


// 	CRect rcRet;
// 	CRect rcClient, rcTab;
// 	GetClientRect(rcClient);
// 	m_Tab.GetWindowRect(rcTab);
// 	ScreenToClient(rcTab);
// 	rcRet.CopyRect(rcClient);
// 	rcRet.InflateRect(0, -rcTab.Height(), 0, 0);
// 	return rcRet;
}






void CDlgNewUI::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	CRect rcClient;
	GetClientRect(rcClient);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	dcMem.SelectObject(&bmpMem);

	
	Graphics g(dcMem.m_hDC);
	COLORREF colBK = RGB(224, 241, 252);//GetSysColor(CTLCOLOR_DLG);//GetBkColor(dc.m_hDC);

	g.FillRectangle(&SolidBrush(Color(GetRValue(colBK), GetGValue(colBK), GetBValue(colBK))), rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());


	

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	bmpMem.DeleteObject();
	dcMem.DeleteDC();
}


BOOL CDlgNewUI::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}