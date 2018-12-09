// DlgResult.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <atlconv.h >
#include "DlgResult.h"
#include "afxdialogex.h"
#include "DlgTotalResult.h"
#include "../Misc/Misc.h"
#include "../Misc/ProgramMisc.h"
#include "DlgSetMachiningOrder.h"



#include <windows.h>
#include <WinUser.h>
#include <stdio.h>
#include <stdlib.h>
#include <atltypes.h>
#include <string>
#include <vector>
#include <sstream>
//#include <afxwin.h>






#include "../../../include/DataManager/BaseDataType/CommonData/CommonData.h"
#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleton.h"
#include "../../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../../include/DataManager/BaseDataType/Panel/Panel.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"
#include "../../../include/FileReadWrite/DxfReadWrite/DxfReadWrite.h"
#include "../../../include/FileReadWrite/HgyReadWrite/HgyReadWrite.h"
#include "../../../include/FileReadWrite/Misc/HGCode.h"
#include "../../../include/FileReadWrite/PltReadWrite/PltReadWrite.h"
#include "../../../include/FileReadWrite/PdfReadWrite/PdfReadWrite.h"



#include "../Drawing/Drawing.h"
#include "../DataProcess/DataProcess.h"




using namespace std;

#pragma comment (lib, "User32.lib")












// CDlgResult �Ի���

#define PI 3.14159
#define KNIFE_DOWN_CIRCLE_DIA 10
#define DRAW_PANEL_GAP 50
#define TOTAL_RESULT_DLG_WIDTH 440
#define CLIPBOARD_WIDTH 150
#define CLIPBOARD_ITEM_HEIGHT 150
#define ATTACH_DISTANCE 50



#define	VERSION_PRO				(1)
#define	VERSION_NORMAL			(2)

#define CUR_VERSION				VERSION_NORMAL


#define MESSAGE_SET_CTRL_TEXT	(1)


#define  SLEEP_1MS			(1)
#define  SLEEP_10MS			(10)
#define  SLEEP_100MS		(100)
#define  SLEEP_500MS		(500)
#define  SLEEP_1000MS		(1000)
#define  SLEEP_2000MS		(2000)
#define  SLEEP_3000MS		(3000)
#define  SLEEP_4000MS		(4000)
#define  SLEEP_5000MS		(5000)

#define FIND_TIMES			(200)



IMPLEMENT_DYNAMIC(CDlgResult, CDialogChildBase)

	CDlgResult::CDlgResult(CWnd* pParent /*=NULL*/)
	: CDialogChildBase(CDlgResult::IDD, pParent)
	, m_pDlgTotalResult(NULL)
	, m_pPastingComponent(NULL)
	, m_bNeedToEraseOldPastingComponent(false)
	, m_pRemainderCutting(NULL)
	, m_bNeedToEraseOldRemainderCutting(false)
{

	m_len				= 0.0;	
	m_width				= 0.0;	
	m_x_space			= 0.0;
	m_y_space			= 0.0;
	m_left_offset		= 0.0;
	m_right_offset		= 0.0;
	m_top_offset		= 0.0;
	m_bottom_offset		= 0.0;

	m_arranging_origin = 0;
}

CDlgResult::~CDlgResult()
{
	if(m_pPastingComponent)
		delete m_pPastingComponent;

	if(m_pRemainderCutting)
		delete m_pRemainderCutting;

	CSingleton* pSingleton = CSingleton::GetSingleton();
	if (pSingleton != NULL)
	{
		pSingleton->ClearAllData();
	}

}

void CDlgResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLIPBOARD, m_lbClipBoard);

	DDX_Text(pDX, IDC_EDIT_NEW_LEN, m_len);
	DDX_Text(pDX, IDC_EDIT_NEW_WIDTH, m_width);

	DDX_Text(pDX, IDC_EDIT_NEW_X_SPACE, m_x_space);				 
	DDX_Text(pDX, IDC_EDIT_NEW_Y_SPACE, m_y_space);				 
																 
	DDX_Text(pDX, IDC_EDIT_NEW_LEFT_OFFSET, m_left_offset);			 
	DDX_Text(pDX, IDC_EDIT_NEW_RIGHT_OFFSET, m_right_offset);			 
																 
	DDX_Text(pDX, IDC_EDIT_NEW_TOP_OFFSET, m_top_offset);
	DDX_Text(pDX, IDC_EDIT_NEW_BOTTOM_OFFSET, m_bottom_offset);


	DDX_Control(pDX, IDC_COMBO_LAYOUT_ORIGIN, control_arranging_origin);
	DDX_CBIndex(pDX, IDC_COMBO_LAYOUT_ORIGIN, m_arranging_origin);

}


BEGIN_MESSAGE_MAP(CDlgResult, CDialogChildBase)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_REFRESH_PANEL_VIEW, &CDlgResult::OnRefreshPanelView)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_MENU_RESET_MACHINING_ORDER, &CDlgResult::OnMenuResetMachiningOrder)
	ON_COMMAND(ID_MENU_CUT_COMPONENT, &CDlgResult::OnMenuCutComponent)
	ON_COMMAND(ID_MENU_COPY_COMPONENT, &CDlgResult::OnMenuCopyComponent)
	ON_COMMAND(ID_MENU_PASTE_COMPONENT, &CDlgResult::OnMenuPasteComponent)
	ON_COMMAND(ID_MENU_REMOVE_COMPONENT, &CDlgResult::OnMenuRemoveComponent)
	ON_COMMAND(ID_MENU_REMAINDER_CUTTING, &CDlgResult::OnMenuRemainderCutting)
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_DXF, &CDlgResult::OnBtnExportDxf)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_PLT, &CDlgResult::OnBtnExportPlt)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_PDF, &CDlgResult::OnBtnExportPdf)

	
	ON_LBN_SELCHANGE(IDC_LIST_CLIPBOARD, &CDlgResult::OnLbnSelchangeClipBoard)
	ON_MESSAGE(WM_SHOW_OR_HIDE_CLIPBOARD, &CDlgResult::OnShowOrHideClipboard)
	ON_COMMAND(ID_MENU_ROTATE_PASTING_COMPONENT, &CDlgResult::OnMenuRotatePastingComponent)

	ON_BN_CLICKED(IDC_BUTTON_READ_HGO, &CDlgResult::OnOpenSolution)
	ON_BN_CLICKED(IDC_BUTTON_LAYOUT, &CDlgResult::OnLayout)
	ON_BN_CLICKED(IDC_BUTTON_READ_PIC_INFO, &CDlgResult::OnOpenSourcePicInfo)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT_MAINTOP, &CDlgResult::OnConnectMaintop)




	
END_MESSAGE_MAP()


// CDlgResult ��Ϣ�������


BOOL CDlgResult::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pDlgTotalResult = new CDlgTotalResult(this);
	m_pDlgTotalResult->Create(CDlgTotalResult::IDD, this);
	m_pDlgTotalResult->MoveWindow(100, 100, 100, 500);
	m_pDlgTotalResult->ShowWindow(SW_SHOW);

	m_lbClipBoard.ShowWindow(SW_HIDE);

	// ��ʼ��
	control_arranging_origin.Clear();
	control_arranging_origin.InsertString(0,_T("���½�"));
	control_arranging_origin.InsertString(1,_T("���Ͻ�"));
	control_arranging_origin.InsertString(2,_T("���½�"));
	control_arranging_origin.InsertString(3,_T("���Ͻ�"));


	control_arranging_origin.SetCurSel(0);



	SetTimer(IDTIMER1, 1000, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgResult::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent) 
	{
	case IDTIMER1:
		{   
			//AfxMessageBox("���붨ʱ��1");
			KillTimer(IDTIMER1);

			// ���ļ�
			OnOpenSourcePicInfo();
			if (open_rt)
			{
				// ����
				OnLayout();
			}

			break;
		}
	
	default:
		;
	}

	CDialogChildBase::OnTimer(nIDEvent);
}



void CDlgResult::OnBtnExportPdf()
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(pParam && pParam->m_pPanel)
	{
		CString strFilePath;

		CString filter = "pdf �ļ�(*.pdf)|*.pdf|�����ļ� (*.*)|*.*||";
		CFileDialog fileDlg (FALSE, _T("pdf"), _T("001.pdf")/*strDefFileName*/,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);

		if ( fileDlg.DoModal() == IDOK)
		{
			strFilePath = fileDlg.GetPathName();


			if (PdfReadWrite::OutputPdf(pParam->m_pPanel, strFilePath.GetBuffer()) == true )
			{
				AfxMessageBox("�ļ�����ɹ���");
			}
			else
			{
				AfxMessageBox("�ļ�����ʧ�ܣ�");
			}
		}
	}
}



void CDlgResult::OnBtnExportPlt()
{
	// ������ǰ�����dxf�ļ�
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(pParam && pParam->m_pPanel)
	{
		CString strFilePath;

		CString filter = "plt �ļ�(*.plt)|*.plt|�����ļ� (*.*)|*.*||";
		CFileDialog fileDlg (FALSE, _T("plt"), _T("001.plt")/*strDefFileName*/,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);

		if ( fileDlg.DoModal() == IDOK)
		{
			strFilePath = fileDlg.GetPathName();


			if (PltReadWrite::OutputPlt(pParam->m_pPanel, strFilePath.GetBuffer()) == true )
			{
				AfxMessageBox("�ļ�����ɹ���");
			}
			else
			{
				AfxMessageBox("�ļ�����ʧ�ܣ�");
			}
		}
	}
}



void CDlgResult::OnBtnExportDxf()
{
	// ������ǰ�����dxf�ļ�
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(pParam && pParam->m_pPanel)
	{
		CString strFilePath;

		CString filter = "dxf �ļ�(*.dxf)|*.dxf|�����ļ� (*.*)|*.*||";
		//CString strDefFileName = GetFileNameInPath(m_strOpenedFile) + _T(".dxf");
		CFileDialog fileDlg (FALSE, _T("dxf"), _T("001.dxf")/*strDefFileName*/,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filter, NULL);
		//CString strDefDir = SelectPathDlg().DefaultSavePath();
		//fileDlg.m_ofn.lpstrInitialDir = strDefDir;

		if ( fileDlg.DoModal() == IDOK)
		{
			strFilePath = fileDlg.GetPathName();

			CString DownerFilePath = strFilePath;
			DownerFilePath.Replace(".dxf", "@W.dxf");

			if (DxfReadWrite::OutputUpperFaceDxf(pParam->m_pPanel, strFilePath) == true )
			{
				AfxMessageBox("�ļ�����ɹ���");
			}
			else
			{
				AfxMessageBox("�ļ�����ʧ�ܣ�");
			}
		}
	}
}


void CDlgResult::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	CRect rcClient, rcDrawArea;
	GetClientRect(rcClient);
	//GetDrawArea(rcDrawArea);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	dcMem.SelectObject(&bmpMem);


	Graphics g(dcMem.m_hDC);
	//COLORREF colBK = RGB(255, 255, 224);//GetSysColor(CTLCOLOR_DLG);//GetBkColor(dc.m_hDC);
	COLORREF colBK = RGB(255, 255, 255);
	g.FillRectangle(&SolidBrush(Color(GetRValue(colBK), GetGValue(colBK), GetBValue(colBK))), rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());

	rcDrawArea = GetPanelViewRect();

	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(pParam && pParam->m_pPanel)
	{
		DrawPanel(&dcMem, pParam->m_pPanel, rcDrawArea, *pParam);
	}


	CRect rt_demo(0, 200, 200, 0);
	rt_demo.NormalizeRect();

	int row_num = rcClient.Width() / rt_demo.Width() + 1;
	int column_num = rcClient.Height() / rt_demo.Height() + 1;

	for (int i_row = 0; i_row <= row_num; i_row++)
	{
		for (int j_column = 0; j_column <= column_num; j_column++)
		{
			PointF pos;

			pos.X = rt_demo.Width() *(i_row);
			pos.Y = rt_demo.Height()*(j_column);

			Gdiplus::Font myFont(L"Segoe UI", 60, FontStyleRegular,UnitPixel);	
			StringFormat format;
			format.SetAlignment(StringAlignmentCenter);
			SolidBrush blackBrush(Color(20, 100, 100, 100));
			//g.DrawString(L"Demo", -1, &myFont, pos, &format,&blackBrush);

			RectF rfLogo( rcClient.left + pos.X ,rcClient.top+ pos.Y, rt_demo.Width(), rt_demo.Height());
			

			g.TranslateTransform(pos.X, pos.Y);
			g.RotateTransform(-45);
			g.TranslateTransform(-pos.X, -pos.Y);
			g.DrawString(L"Demo", -1, &myFont, rfLogo, &format, &blackBrush);
			g.ResetTransform();
		}
	}




// 	Gdiplus::Font myFont(L"Segoe UI", 50, FontStyleRegular,UnitPixel);	
// 	StringFormat format;
// 	format.SetAlignment(StringAlignmentCenter);
// 	SolidBrush blackBrush(Color(50, 100, 100, 100));
// 	g.DrawString(L"Demo", -1, &myFont, PointF(200 ,200), &format,&blackBrush);

// 	int nXPos = 30, nYPos = 80;
// 	int nRow = 0;
// 
// 
// 	RectF rfMeasure(0, 100, 100, 0);
// 	CString strTmp="Demo";
// 
// 	while(nYPos < rcClient.Height())
// 	{
// 		nXPos = (nRow%2 == 0) ? 30 : 30 + rfMeasure.Width;
// 		while(nXPos < rcClient.Width())
// 		{
// 			float fTextCenterX = rcClient.left + (nXPos + rfMeasure.Width/2.0);
// 			float fTextCenterY = rcClient.top + (nYPos + rfMeasure.Height/2.0);
// 			RectF rfLogo(rcClient.left + nXPos, rcClient.top + nYPos, rfMeasure.Width + 10, rfMeasure.Height + 10);
// 			g.TranslateTransform(fTextCenterX, fTextCenterY);
// 			g.RotateTransform(-45);
// 			g.TranslateTransform(-fTextCenterX, -fTextCenterY);
// 			g.DrawString(A2W(strTmp), -1, &fontLogo, rfLogo, &sfLogo, &brushLogo);
// 			g.ResetTransform();
// 
// 			nXPos += 450;
// 		}
// 		nYPos += 450;
// 		nRow++;
// 	}







	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	bmpMem.DeleteObject();
	dcMem.DeleteDC();
}


BOOL CDlgResult::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}














#if 1



void CDlgResult::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(m_pDlgTotalResult && m_pDlgTotalResult->GetSafeHwnd())
	{
		CRect rcTotalResult(cx-TOTAL_RESULT_DLG_WIDTH, 0, cx, cy);
		m_pDlgTotalResult->MoveWindow(rcTotalResult);

		CRect rcClipboard(cx- TOTAL_RESULT_DLG_WIDTH - CLIPBOARD_WIDTH, 0, cx - TOTAL_RESULT_DLG_WIDTH, cy);
		m_lbClipBoard.MoveWindow(rcClipboard);

		CRect rcPanelView = GetPanelViewRect();


		Invalidate();
	}
}



CRect CDlgResult::GetPanelViewRect()
{
	CRect rcRet;

	CRect rcClient, rcDlgTotalResult, rcClipboard;
	GetClientRect(rcClient);
	m_pDlgTotalResult->GetWindowRect(rcDlgTotalResult);
	m_lbClipBoard.GetWindowRect(rcClipboard);
	ScreenToClient(rcDlgTotalResult);
	ScreenToClient(rcClipboard);

	rcRet.left = rcClient.left + 10;
	rcRet.top = rcClient.top + 10;
	rcRet.right = (m_lbClipBoard.IsWindowVisible() ? rcClipboard.left : rcDlgTotalResult.left) - 10;
	rcRet.bottom = rcClient.bottom - 50;

	return rcRet;
}

LRESULT CDlgResult::OnRefreshPanelView(WPARAM wparam, LPARAM lparam)
{
	//m_bDrawPanel = true;
	//DrawDiagram((PanelViewingParam*)wparam);
	InvalidateRect(GetPanelViewRect());
	
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();

	if(pParam)
	{
		CSolution* pCurSel = pParam->m_pSolution;

		if (pCurSel)
		{

			m_len				= pCurSel->m_BaseInfo.m_PanelLength		;	
			//m_width				= pCurSel->m_BaseInfo.m_PanelWidth		;	
			m_x_space			= pCurSel->m_BaseInfo.m_x_space			;
			m_y_space			= pCurSel->m_BaseInfo.m_y_space			;
			m_left_offset		= pCurSel->m_BaseInfo.m_left_offset		;
			m_right_offset		= pCurSel->m_BaseInfo.m_right_offset	;
			m_top_offset		= pCurSel->m_BaseInfo.m_top_offset		;
			m_bottom_offset		= pCurSel->m_BaseInfo.m_bottom_offset	;
			m_arranging_origin	= pCurSel->m_BaseInfo.m_LayoutOrg		;

			UpdateData(FALSE);
		}


	}



	bool bEnable = true;
	if(!pParam || !pParam->m_pPanel)
		bEnable = false;


	return 0;
}

void CDlgResult::DrawPanel(CDC* pDC, Panel* pPanel, CRect rcDrawArea, PanelViewingParam& param)
{
	Graphics g(pDC->m_hDC);
	g.SetSmoothingMode(SmoothingModeHighQuality);


	Panel& thePanel = *(param.m_pPanel);
	int nPanelLen = /*2000*//*thePanel.m_RealLength*/thePanel.m_OrgLen;
	int nPanelWidth = /*1000*//*thePanel.m_RealWidth*/thePanel.m_OrgWidth;

	//float fScale = GetScale(nPanelLen, nPanelWidth, rcDrawArea.Width()-50, rcDrawArea.Height()-50);
	float fScale = GetDrawingScale(nPanelLen, nPanelWidth);
	CRect rcPanelArea(DRAW_PANEL_GAP/2, DRAW_PANEL_GAP/2, rcDrawArea.Width()-DRAW_PANEL_GAP/2, rcDrawArea.Height()-DRAW_PANEL_GAP/2);

	CRect rcPanel((rcDrawArea.Width()-nPanelLen*fScale)/2, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2, \
		(rcDrawArea.Width()-nPanelLen*fScale)/2 + nPanelLen*fScale, \
		(rcDrawArea.Height()-nPanelWidth*fScale)/2 + nPanelWidth*fScale);

	g.FillRectangle(&SolidBrush(Color(240, 255, 255)), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());

	vector<Component*> vAllComponent;
	FindAllComponentInPanel(thePanel, vAllComponent);

	for(int i = 0; i < vAllComponent.size(); i++)
	{
		Component& theComponent = *(vAllComponent[i]);
		CRect rcComponent(theComponent.m_x*fScale, (/*thePanel.m_RealWidth*/nPanelWidth - (theComponent.m_y + theComponent.m_RealWidth))*fScale, (theComponent.m_x + theComponent.m_RealLength)*fScale, (/*thePanel.m_RealWidth*/nPanelWidth - theComponent.m_y)*fScale);
		rcComponent.OffsetRect(rcPanel.left, rcPanel.top);

		//g.FillRectangle(&SolidBrush(Color(180, 255, 255, 224)), (INT)rcComponent.left, (INT)rcComponent.top, (INT)rcComponent.Width(), (INT)rcComponent.Height());

		Region regionComponent;
		GetComponentRegion(theComponent.m_vOutlinePoint, rcComponent, fScale, regionComponent);
		

		g.FillRegion(&SolidBrush(Color(180, 255, 255, 224)), &regionComponent);

		


		vector<PointInfo>& vOutlinePoint = theComponent.m_vOutlinePoint;
		for(int j = 1; j < vOutlinePoint.size(); j++)
		{
			PointInfo& vertexStart = vOutlinePoint[j - 1];
			PointInfo& vertexEnd = vOutlinePoint[j];

			DrawOneLine(g, rcComponent, fScale, vertexStart, vertexEnd);
		}

		DrawMachiningInfo(g, rcComponent, fScale, &theComponent);

		
		DrawDetail(g, rcComponent, fScale, &theComponent);
		

	}


	DrawRemainderCuting(g, rcPanel, fScale, thePanel);

	g.DrawRectangle(&Pen(Color::Black, 2), (INT)rcPanel.left, (INT)rcPanel.top, (INT)rcPanel.Width(), (INT)rcPanel.Height());
}

void CDlgResult::RefreshOptimizeResult()
{
	m_pDlgTotalResult->RefreshOptimizeResult();
}

void CDlgResult::DrawOneLine(Graphics& g, CRect rcPanel, float fScale, PointInfo& vertexStart, PointInfo& vertexEnd, Color color)
{
	if(vertexStart.group == OUTLINE_GROUP_TYPE_END)
		return;

	Pen pen(color, 1);
	pen.SetDashStyle(DashStyleDash);

	if(GetLineType(vertexStart.sign) == STRAIGHT_LINE)
	{

		INT p1_x = rcPanel.left + vertexStart.x * fScale;
		INT p1_y = rcPanel.top + (rcPanel.Height() - vertexStart.y * fScale);
		INT p2_x = rcPanel.left + vertexEnd.x * fScale;
		INT p2_y = rcPanel.top + (rcPanel.Height() - vertexEnd.y * fScale);
		g.DrawLine(&pen, p1_x, p1_y, p2_x, p2_y);
	}
	else if(GetLineType(vertexStart.sign) == CIRCULAR_ARC)
	{
		

		CString strTmp;
		

		int eArcDir = vertexEnd.dir;
		float fXa = vertexStart.x, fYa = vertexStart.y, fXb = vertexEnd.x, fYb = vertexEnd.y, fR = /*vertexStart.r*/vertexEnd.r;
		

		float fDx = fXa - fXb;
		

		float fDy = fYa - fYb;
		


		float fCenterX1;
		float fCenterX2;
		float fCenterY1;
		float fCenterY2;
		if(IsFloatEqual(fDy, 0, 0.1))
		{
			fCenterX1 = (fXa + fXb) / 2;
			fCenterX2 = (fXa + fXb) / 2;

			fCenterY1 = fYa - sqrt(pow(fR, 2) - pow((abs(fXa-fXb)/2), 2));
			fCenterY2 = fYa + sqrt(pow(fR, 2) - pow((abs(fXa-fXb)/2), 2));
		}
		else
		{
			float fD = fDx / fDy;
			

			float fa = 1 + fD*fD;
			float fb = -(fD*(fD*(fXa+fXb)+fDy) + 2*fXb);
			float fc = pow((0.5*(fD*(fXa+fXb)+fDy)), 2) - pow(fR, 2) + pow(fXb, 2);
			

			fCenterX1 = (-fb + sqrt(fb*fb - 4*fa*fc)) / (2 * fa);
			fCenterX2 = (-fb - sqrt(fb*fb - 4*fa*fc)) / (2 * fa);
			fCenterY1 = -fD*fCenterX1+((fDx*(fXa+fXb)+fDy*(fYa+fYb))/(2*fDy));
			fCenterY2 = -fD*fCenterX2+((fDx*(fXa+fXb)+fDy*(fYa+fYb))/(2*fDy));
		}
		


		


		PointF vectorStart(fXa-fCenterX1, fYa-fCenterY1);
		PointF vectorEnd(fXb-fCenterX1, fYb-fCenterY1);
		float fModStartVector = sqrt(pow(vectorStart.X,2) + pow(vectorStart.Y,2));
		float fModEndVector = sqrt(pow(vectorEnd.X,2) + pow(vectorEnd.Y,2));
		float fCos = (vectorStart.X*vectorEnd.X + vectorStart.Y*vectorEnd.Y) / (fModStartVector*fModEndVector);
		float fSin = (vectorStart.X*vectorEnd.Y - vectorEnd.X*vectorStart.Y) / (fModStartVector*fModEndVector);

		


		PointF ptTheCenter;
		if(((-fSin) >= 0 && /*vertexStart.dir*/eArcDir == ANTI_CLOCKWISE) || ((-fSin) < 0 && /*vertexStart.dir*/eArcDir == CLOCKWISE))
		{
			ptTheCenter.X = fCenterX2;
			ptTheCenter.Y = fCenterY2;
		}
		else
		{
			ptTheCenter.X = fCenterX1;
			ptTheCenter.Y = fCenterY1;
		}

		

		RectF rc(rcPanel.left, rcPanel.top, 100, 100);
		rc.X = rcPanel.left + (ptTheCenter.X - /*vertexStart.r*/fR)*fScale;
		rc.Y = rcPanel.top + (rcPanel.Height() - (ptTheCenter.Y + /*vertexStart.r*/fR)*fScale);
		rc.Width = (/*vertexStart.r*/fR * 2)*fScale;
		rc.Height = (/*vertexStart.r*/fR * 2)*fScale;
		float fArc = acos(fCos);
		float fAng = fArc*360/(PI*2);

		PointF vectorDrawingStart;
		if(/*vertexStart.dir*/eArcDir == CLOCKWISE)
		{
			vectorDrawingStart.X = vertexStart.x - ptTheCenter.X;
			vectorDrawingStart.Y = vertexStart.y - ptTheCenter.Y;
		}
		else
		{
			vectorDrawingStart.X = vertexEnd.x - ptTheCenter.X;
			vectorDrawingStart.Y = vertexEnd.y - ptTheCenter.Y;
		}

		float fCosDrawingStart = vectorDrawingStart.X/sqrt(pow(vectorDrawingStart.X,2)+pow(vectorDrawingStart.Y,2));
		float fSinDrawingStart = vectorDrawingStart.Y/sqrt(pow(vectorDrawingStart.X,2)+pow(vectorDrawingStart.Y,2));
		float fAngDrawingStart = (acos(fCosDrawingStart))*360/(PI*2);
		if(fSinDrawingStart >= 0)
			fAngDrawingStart = 360 - fAngDrawingStart;

		g.DrawArc(&pen, rc, fAngDrawingStart, fAng);

		

	}
}

void CDlgResult::DrawMachiningInfo(Graphics& g, CRect rcPanel, float fScale, Component* pComponent)
{
	SolidBrush brush(Color::Blue);//������ɫ
	SolidBrush brushLightBlue(Color::LightBlue);
	SolidBrush brushDisable(Color(100, 100, 100));
	SolidBrush brushDisable_Light(Color(190, 190, 190));

	for(vector<AllVec>::iterator itr = pComponent->m_vUpperFaceHole.begin(); itr != pComponent->m_vUpperFaceHole.end(); itr++)
	{
		float x = itr->_x * fScale;
		float y = (pComponent->m_RealWidth - itr->_y) * fScale;	
		g.FillEllipse(itr->m_bDisabled ? &brushDisable : &brush, rcPanel.left + (x - (itr->_r * fScale) / 2), rcPanel.top + (y - (itr->_r * fScale) / 2), itr->_r * fScale, itr->_r * fScale);
	}

	for(vector<AllSlot>::iterator itr = pComponent->m_vUpperFaceSlot.begin(); itr != pComponent->m_vUpperFaceSlot.end(); itr++)
	{
		CRect rcSlot;
		rcSlot.left = (itr->_x - (itr->_width / 2)) * fScale;
		rcSlot.top = (pComponent->m_RealWidth - (itr->_y + (itr->_height / 2))) * fScale;
		rcSlot.right = (itr->_x + (itr->_width / 2)) * fScale;
		rcSlot.bottom = (pComponent->m_RealWidth - (itr->_y - (itr->_height / 2))) * fScale;

		CPoint ptCenter((rcSlot.left+rcSlot.right)/2 + rcPanel.left, (rcSlot.top+rcSlot.bottom)/2 + rcPanel.top);
		g.TranslateTransform(ptCenter.x, ptCenter.y);
		g.RotateTransform(/*itr->m_angle*/itr->_dir == 0 ? 90 : 0);
		g.TranslateTransform(-ptCenter.x, -ptCenter.y);

		g.FillRectangle(itr->m_bDisabled ? &brushDisable : &brush, rcPanel.left + rcSlot.left, rcPanel.top + rcSlot.top, rcSlot.Width(), rcSlot.Height());
		g.ResetTransform();
	}

	for(vector<AllVec>::iterator itr = pComponent->m_vDownerFaceHole.begin(); itr != pComponent->m_vDownerFaceHole.end(); itr++)
	{
		float x = itr->_x * fScale;
		float y = (pComponent->m_RealWidth - itr->_y) * fScale;	
		g.FillEllipse(itr->m_bDisabled ? &brushDisable_Light : &brushLightBlue, rcPanel.left + (x - (itr->_r * fScale) / 2), rcPanel.top + (y - (itr->_r * fScale) / 2), itr->_r * fScale, itr->_r * fScale);
	}

	for(vector<AllSlot>::iterator itr = pComponent->m_vDownerFaceSlot.begin(); itr != pComponent->m_vDownerFaceSlot.end(); itr++)
	{
		CRect rcSlot;
		rcSlot.left = (itr->_x - (itr->_width / 2)) * fScale;
		rcSlot.top = (pComponent->m_RealWidth - (itr->_y + (itr->_height / 2))) * fScale;
		rcSlot.right = (itr->_x + (itr->_width / 2)) * fScale;
		rcSlot.bottom = (pComponent->m_RealWidth - (itr->_y - (itr->_height / 2))) * fScale;

		CPoint ptCenter((rcSlot.left+rcSlot.right)/2 + rcPanel.left, (rcSlot.top+rcSlot.bottom)/2 + rcPanel.top);
		g.TranslateTransform(ptCenter.x, ptCenter.y);
		g.RotateTransform(/*itr->m_angle*/itr->_dir == 0 ? 90 : 0);
		g.TranslateTransform(-ptCenter.x, -ptCenter.y);

		g.FillRectangle(itr->m_bDisabled ? &brushDisable_Light : &brushLightBlue, rcPanel.left + rcSlot.left, rcPanel.top + rcSlot.top, rcSlot.Width(), rcSlot.Height());
		g.ResetTransform();
	}

}

void CDlgResult::DrawDetail(Graphics& g, CRect rcComponent, float fScale, Component* pComponent)
{
	Gdiplus::Font font18(L"Segoe UI", 20, FontStyleRegular, UnitPixel);
	Gdiplus::Font font11(L"Segoe UI", 11, FontStyleRegular, UnitPixel);
	StringFormat sf;
	sf.SetLineAlignment(StringAlignmentCenter);
	SolidBrush brushRed(Color::Red);
	SolidBrush brushBlack(Color::Black);

	CString strOrder, strSize, strDetail;
	strOrder.Format("%d", pComponent->m_NumberInPanel);
	strSize.Format(_T("%s*%s"), GetFloatString(pComponent->m_RealLength, 1), GetFloatString(pComponent->m_RealWidth, 1));
	strDetail = strSize + _T("\n") + pComponent->m_BarCode;

	RectF rfMeasureDetail , rfMeasureOrder;	
	g.MeasureString(AnsiToUnicode(strDetail).c_str(), -1, &font11, PointF(0, 0), &rfMeasureDetail);
	g.MeasureString(AnsiToUnicode(strOrder).c_str(), -1, &font18, PointF(0, 0), &rfMeasureOrder);
	
	if(((pComponent->m_nRotatedAngle / 90) % 2) == 0)//���ƽ����ԭ����
	{
		RectF rfTotalText(rcComponent.left + (rcComponent.Width()-(rfMeasureOrder.Width+rfMeasureDetail.Width))/2, \
			rcComponent.top + (rcComponent.Height()-rfMeasureDetail.Height)/2, \
			rfMeasureOrder.Width+rfMeasureDetail.Width, \
			rfMeasureDetail.Height);

		RectF rfOrder, rfDetail;
		rfOrder = rfTotalText;
		g.DrawString(AnsiToUnicode(strOrder).c_str(), -1, &font18, rfOrder, &sf, &brushRed);
		rfDetail = rfTotalText;
		rfDetail.Offset(rfMeasureOrder.Width, 0);
		rfDetail.Width -= rfMeasureOrder.Width;
		
		if (pComponent->m_IndexInSameCpn == 0)
		{
			g.DrawString(AnsiToUnicode(strDetail).c_str(), -1, &font11, rfDetail, &sf, &brushBlack);
		}
	}
	else//�����ֱ��ԭ����
	{
		RectF rfTotalText(rcComponent.left + (rcComponent.Width()-(rfMeasureOrder.Width+rfMeasureDetail.Height))/2, \
			rcComponent.top + (rcComponent.Height()-rfMeasureDetail.Width)/2, \
			rfMeasureOrder.Width+rfMeasureDetail.Height, \
			rfMeasureDetail.Width);


		RectF rfOrder, rfDetail;
		rfOrder = rfTotalText;
		sf.SetLineAlignment(StringAlignmentCenter);
		g.DrawString(AnsiToUnicode(strOrder).c_str(), -1, &font18, rfOrder, &sf, &brushRed);
		rfDetail = rfTotalText;
		rfDetail.Offset(rfMeasureOrder.Width, 0);
		rfDetail.Width -= rfMeasureOrder.Width;

		POINT ptTargetCenter;
		ptTargetCenter.x = (rfDetail.GetLeft() + rfDetail.GetRight()) / 2;
		ptTargetCenter.y = (rfDetail.GetTop() + rfDetail.GetBottom()) / 2;

		g.TranslateTransform(ptTargetCenter.x, ptTargetCenter.y);
		g.RotateTransform(90);
		g.TranslateTransform(-ptTargetCenter.x, -ptTargetCenter.y);

		RectF rfStr(ptTargetCenter.x - rfDetail.Height/2, ptTargetCenter.y - rfDetail.Width/2, rfDetail.Height, rfDetail.Width);
		

		g.SetSmoothingMode(SmoothingModeHighQuality);

		if (pComponent->m_IndexInSameCpn == 0)
		{
			g.DrawString(AnsiToUnicode(strDetail).c_str(), -1, &font11, rfStr, &sf, &brushBlack);
		}

		g.ResetTransform();
	}
}

void CDlgResult::DrawRemainderCuting(Graphics& g, CRect rcPanel, float fScale, Panel& thePanel)
{
	SolidBrush brush(Color::Blue);
	for(int i = 0; i < thePanel.m_vCutPath.size(); i++)
	{
		CutPath& theCutPath = thePanel.m_vCutPath[i];
		float fXCenter = (theCutPath._x1 + theCutPath._x2) / 2;
		float fYCenter = (theCutPath._y1 + theCutPath._y2) / 2;
		float fLen = GetPointDistance(theCutPath._x1, theCutPath._y1, theCutPath._x2, theCutPath._y2);
		float fDia = GetKerf();

		CRect rcSlot;
		rcSlot.left = (fXCenter - (fLen / 2)) * fScale;
		rcSlot.top = (thePanel.m_OrgWidth - (fYCenter + (fDia / 2))) * fScale;
		rcSlot.right = (fXCenter + (fLen / 2)) * fScale;
		rcSlot.bottom = (thePanel.m_OrgWidth - (fYCenter - (fDia / 2))) * fScale;

		CPoint ptCenter((rcSlot.left+rcSlot.right)/2 + rcPanel.left, (rcSlot.top+rcSlot.bottom)/2 + rcPanel.top);
		g.TranslateTransform(ptCenter.x, ptCenter.y);
		g.RotateTransform(/*itr->_dir == 0 ? 90 : 0*/-GetLineAngle(theCutPath._x1, theCutPath._y1, theCutPath._x2, theCutPath._y2));
		g.TranslateTransform(-ptCenter.x, -ptCenter.y);

		g.FillRectangle(&brush, rcPanel.left + rcSlot.left, rcPanel.top + rcSlot.top, rcSlot.Width(), rcSlot.Height());
		g.ResetTransform();
	}

}

void CDlgResult::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel || !pParam->m_pSolution)
		return;
	Panel* pThePanel = pParam->m_pPanel;

	Component* pTheClickedComponent = NULL;
	vector<Component*> vAllComponent;
	FindAllComponentInPanel(*pThePanel, vAllComponent);
	for(int i = 0; i < vAllComponent.size(); i++)
	{
		Component& theComponent = *(vAllComponent[i]);
		CRect rcComponentUIRect = GetComponentUIRect(pThePanel->m_OrgLen, pThePanel->m_OrgWidth, theComponent.m_x, theComponent.m_y, theComponent.m_RealLength, theComponent.m_RealWidth);
		if(rcComponentUIRect.PtInRect(point))
		{

			float fXClickedInComponent, fYClickedInComponent;
			float fScale = GetDrawingScale(pThePanel->m_OrgLen, pThePanel->m_OrgWidth);
			fXClickedInComponent = (point.x - rcComponentUIRect.left) / fScale;
			fYClickedInComponent = (-(point.y - rcComponentUIRect.bottom)) / fScale;
			vector<PointInfo> vOutterOutlinePoint;
			GetOutterOutlineVertex(theComponent.m_vOutlinePoint, vOutterOutlinePoint);
			theComponent.m_nKnifeDownPos = GetKnifeDownPosByClickedPos(vOutterOutlinePoint, fXClickedInComponent, fYClickedInComponent);



			CRect rcPanelViewRect;
			rcPanelViewRect = GetPanelViewRect();
			//InvalidateRect(rcPanelViewRect);
			PostMessage(WM_REFRESH_PANEL_VIEW, (WPARAM)pParam/*0*/, 0);
			break;
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

CRect CDlgResult::GetComponentUIRect(float fPanelXLen, float fPanelYLen, float fComponentX, float fComponentY, float fComponentXLen, float fComponentYLen)
{
	CRect rcDrawArea;
	rcDrawArea = GetPanelViewRect();

	float fScale = GetDrawingScale(fPanelXLen, fPanelYLen);

	CRect rcPanelArea(DRAW_PANEL_GAP/2, DRAW_PANEL_GAP/2, rcDrawArea.Width()-DRAW_PANEL_GAP/2, rcDrawArea.Height()-DRAW_PANEL_GAP/2);

	CRect rcPanel((rcDrawArea.Width()-fPanelXLen*fScale)/2, \
		(rcDrawArea.Height()-fPanelYLen*fScale)/2, \
		(rcDrawArea.Width()-fPanelXLen*fScale)/2 + fPanelXLen*fScale, \
		(rcDrawArea.Height()-fPanelYLen*fScale)/2 + fPanelYLen*fScale);

	CRect rcComponentUI(\
		fComponentX*fScale, \
		(fPanelYLen - (fComponentY + fComponentYLen))*fScale, \
		(fComponentX + fComponentXLen)*fScale, \
		(fPanelYLen - fComponentY)*fScale);
	rcComponentUI.OffsetRect(rcPanel.left, rcPanel.top);


	return rcComponentUI;
}

float CDlgResult::GetDrawingScale(float fPanelXLen, float fPanelYLen)
{
	CRect rcDrawArea;
	rcDrawArea = GetPanelViewRect();

	float fScale = GetScale(fPanelXLen, fPanelYLen, rcDrawArea.Width()-50, rcDrawArea.Height()-50);

	return fScale;
}

vector<Panel*> CDlgResult::GetPanels(bool bChecked)
{
	return m_pDlgTotalResult->GetPanels(bChecked);
}

bool CDlgResult::IsPanelChecked(Panel* pPanel)
{
	return m_pDlgTotalResult->IsPanelChecked(pPanel);
}

void CDlgResult::OnMenuResetMachiningOrder()
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;
	Panel* pThePanel = pParam->m_pPanel;
	CDlgSetMachiningOrder dlg;
	dlg.SetPanel(pThePanel);
	if(dlg.DoModal() == IDOK)
	{
		ResetMachiningOrderByMap(pThePanel, dlg.GetResettedOrderMap());
		CalcKnifeDownPosInPanel(pThePanel);
		PostMessage(WM_REFRESH_PANEL_VIEW, (WPARAM)pParam, 0);
	}

}

void CDlgResult::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
#if 0


	if(m_pPastingComponent)
	{
		CancelPasting();
	}
	else if(m_pRemainderCutting)
	{
		StopRemainderCutting();
	}
	else
	{
		PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
		if(!pParam || !pParam->m_pPanel)
			return;

		if(GetPanelViewRect().PtInRect(point))
		{
			CMenu menu, *pPopup;  
			menu.LoadMenu(IDR_MENU_RESULT_DLG);  
			pPopup = menu.GetSubMenu(0);  
			CPoint myPoint;  
			

			GetCursorPos(&myPoint); 
			menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, myPoint.x, myPoint.y,this); 

			m_ptRClicked = myPoint;
			ScreenToClient(&m_ptRClicked);
		}
	}

	#endif

}

void CDlgResult::OnMenuCutComponent()
{
	


	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;
	Component* pTheComponent = GetClickedComponent(m_ptRClicked, GetPanelViewRect(), DRAW_PANEL_GAP, pParam->m_pPanel);
	if(pTheComponent == NULL)
		return;

	Component* pCopiedComponent = (Component*) pTheComponent->CopyNode();
	AddComponentToClipboard(pCopiedComponent);

	RemoveComponentFromPanel(pTheComponent, pParam->m_pPanel);
	PostMessage(WM_REFRESH_PANEL_VIEW, (WPARAM)pParam, 0);
	m_pDlgTotalResult->UpdateList();
}

void CDlgResult::OnMenuCopyComponent()
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;
	Component* pTheComponent = GetClickedComponent(m_ptRClicked, GetPanelViewRect(), DRAW_PANEL_GAP, pParam->m_pPanel);
	if(pTheComponent == NULL)
		return;

	Component* pCopiedComponent = (Component*) pTheComponent->CopyNode();

	AddComponentToClipboard(pCopiedComponent);
}

void CDlgResult::OnMenuPasteComponent()
{
	if(m_lbClipBoard.GetCurSel() == -1)
		return;


	m_pPastingComponent = (Component*)(m_lbClipBoard.GetComponent(m_lbClipBoard.GetCurSel())->CopyNode());




	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	PastingComponent_OnMouseMove(ptCursor);
}

void CDlgResult::OnMenuRemoveComponent()
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;
	Component* pTheComponent = GetClickedComponent(m_ptRClicked, GetPanelViewRect(), DRAW_PANEL_GAP, pParam->m_pPanel);
	if(pTheComponent == NULL)
		return;

	//AddComponentToClipboard(pTheComponent);
	if(MessageBox(_T("ȷ��ɾ�������"), NULL, MB_OKCANCEL) == IDOK)
	{
		RemoveComponentFromPanel(pTheComponent, pParam->m_pPanel);
		PostMessage(WM_REFRESH_PANEL_VIEW, (WPARAM)pParam, 0);
		m_pDlgTotalResult->UpdateList();
	}
}

void CDlgResult::AddComponentToClipboard(Component* pComponent)
{

	m_lbClipBoard.AddItem(pComponent);
	m_lbClipBoard.SetCurSel(m_lbClipBoard.GetCount()-1);
	m_lbClipBoard.SetItemHeight(m_lbClipBoard.GetCount()-1, CLIPBOARD_ITEM_HEIGHT);
	ShowOrHideClipboard();
}


void CDlgResult::ShowOrHideClipboard()
{
	if(m_lbClipBoard.GetCount() > 0 && !m_lbClipBoard.IsWindowVisible())
	{
		m_lbClipBoard.ShowWindow(SW_SHOW);
		Invalidate();
	}
	else if(m_lbClipBoard.GetCount() == 0 && m_lbClipBoard.IsWindowVisible())
	{
		m_lbClipBoard.ShowWindow(SW_HIDE);
		Invalidate();
	}
}

void CDlgResult::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;

	PastingComponent_OnMouseMove(point);

	if(m_pRemainderCutting)
	{
		if(m_bNeedToEraseOldRemainderCutting)
		{
			DrawRemainderCuttingToPos(*m_pRemainderCutting, pParam->m_pPanel);
		}

		PointFloat ptPosInPanel;
		if(!IsAttachMode())
			ptPosInPanel = ConvertUIPosToPanelPos(pParam->m_pPanel, point);
		else
		{
			if(m_nRemainderCuttingSettingState == CUTTING_SETTING_STATE_START)
				ptPosInPanel = GetAttachPanelPos_RemainderCutting_Start(point);
			else
			{
				PointFloat ptStartInPanel;
				ptStartInPanel.x = m_pRemainderCutting->_x1;
				ptStartInPanel.y = m_pRemainderCutting->_y1;
				ptPosInPanel = GetAttachPanelPos_RemainderCutting_End(point, ptStartInPanel);
			}
		}

		if(m_nRemainderCuttingSettingState == CUTTING_SETTING_STATE_START)
		{		
			m_pRemainderCutting->_x1 = m_pRemainderCutting->_x2 = ptPosInPanel.x;
			m_pRemainderCutting->_y1 = m_pRemainderCutting->_y2 = ptPosInPanel.y;
		}
		else
		{
			m_pRemainderCutting->_x2 = ptPosInPanel.x;
			m_pRemainderCutting->_y2 = ptPosInPanel.y;
		}
		
		DrawRemainderCuttingToPos(*m_pRemainderCutting, pParam->m_pPanel);
		m_bNeedToEraseOldRemainderCutting = true;
	}

	CDialog::OnMouseMove(nFlags, point);
}

//void CDlgResult::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//
//	CDialog::OnLButtonUp(nFlags, point);
//}

void CDlgResult::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_pRemainderCutting)
	{
		if(m_nRemainderCuttingSettingState == CUTTING_SETTING_STATE_START)
		{
			m_nRemainderCuttingSettingState = CUTTING_SETTING_STATE_END;
		}
		else
		{
			PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
			if(!pParam || !pParam->m_pPanel)
				return;

			pParam->m_pPanel->m_vCutPath.push_back(*m_pRemainderCutting);
			StopRemainderCutting();
		}
	}
	else if(m_pPastingComponent)
	{
		PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
		if(pParam && pParam->m_pPanel)
		{
			//if(IsPosCanPasteComponent(pParam->m_pPanel, m_pPastingComponent, point))
			if(pParam->m_pPanel->m_Material.CompareNoCase(m_pPastingComponent->m_Material) == 0 \
				&& IsFloatEqual(pParam->m_pPanel->m_Thickness, m_pPastingComponent->m_Thickness))
			{
				PointFloat ptComponentLeftTop;
				if(!IsAttachMode())
					ptComponentLeftTop = ConvertUIPosToPanelPos(pParam->m_pPanel, /*point*/m_ptOldPastingComponent);
				else
					ptComponentLeftTop = m_ptfAttachPosInPanel;
				PointFloat ptComponentLeftBottom;
				ptComponentLeftBottom.x = ptComponentLeftTop.x;
				ptComponentLeftBottom.y = ptComponentLeftTop.y - m_pPastingComponent->m_RealWidth;
				if(InsertComponentToPanelRoot(pParam->m_pPanel, m_pPastingComponent, ptComponentLeftBottom))
				{
					m_lbClipBoard.DeleteString(m_lbClipBoard.GetCurSel());
					m_lbClipBoard.SetCurSel(0);

					m_pPastingComponent = NULL;
					m_bNeedToEraseOldPastingComponent = false;

					ShowOrHideClipboard();
				//	CalcKnifeDownPosInPanel(pParam->m_pPanel);
					InvalidateRect(GetPanelViewRect());
					m_pDlgTotalResult->UpdateList();
				}
			}
			else
			{
				MessageBox(_T("���ʺ�Ȳ�һ�£��޷���������"));
			}
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgResult::DrawComponentToPos(Component* pComponent, CPoint ptComponent, bool bDrawOKIcon, Panel* pPanel)
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;

	float fScale = GetDrawingScale(pParam->m_pPanel->m_OrgLen, pParam->m_pPanel->m_OrgWidth);

	CRect rcComponent(ptComponent.x, ptComponent.y, ptComponent.x + pComponent->m_RealLength*fScale, ptComponent.y + pComponent->m_RealWidth*fScale);
	
	CDC* pDC = GetDC();

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(pDC, rcComponent.Width(), rcComponent.Height());
	dcMem.SelectObject(&bmpMem);

	dcMem.Rectangle(0, 0, rcComponent.Width(), rcComponent.Height());

	//float fXClickedInPanel, fYClickedInPanel;
	//CRect rcPanelUIRect = GetComponentUIRect(pParam->m_pPanel->m_OrgLen, pParam->m_pPanel->m_OrgWidth, 0, 0, pParam->m_pPanel->m_OrgLen, pParam->m_pPanel->m_OrgWidth);

	//fXClickedInPanel = (ptComponent.x - rcPanelUIRect.left) / fScale;
	//fYClickedInPanel = (-(ptComponent.y - rcPanelUIRect.bottom)) / fScale;
	//CPoint ptInPanel(fXClickedInPanel, fYClickedInPanel);
	//CSingleton* pSingleton = CSingleton::GetSingleton();
	//if(!IsComponentOverlap(pParam->m_pPanel, pComponent, ptInPanel, pSingleton->m_BaseInfo.m_SawKerfWidth) \
	//	&& IsComponentInPanelClipped(pParam->m_pPanel, pComponent, ptInPanel, pSingleton->m_BaseInfo.m_DeburringWidth))
	DrawingPanelParam param;
	param.m_nGap = 0;
	DrawComponent(&dcMem, pComponent, CRect(0, 0, rcComponent.Width(), rcComponent.Height()), param);

	Image* pImageFlag = NULL;
	if(/*IsPosCanPasteComponent(pParam->m_pPanel, pComponent, ptComponent)*/bDrawOKIcon)
		pImageFlag = LoadPngImgFromRes(IDB_PNG_FLAG_YES);
	else
		pImageFlag = LoadPngImgFromRes(/*IDB_PNG_FLAG_NO*/IDB_PNG_EXCLAMATION);
	Graphics g(dcMem.m_hDC);
	RectF rfImgCorrect(((float)rcComponent.Width() - pImageFlag->GetWidth())/2, ((float)rcComponent.Height()-pImageFlag->GetHeight())/2, pImageFlag->GetWidth(), pImageFlag->GetHeight());
	g.DrawImage(pImageFlag, rfImgCorrect, 0, 0, pImageFlag->GetWidth(), pImageFlag->GetHeight(), UnitPixel);
	delete pImageFlag;

	//int nOldMode = pDC->SetROP2(R2_XORPEN);
	pDC->BitBlt(ptComponent.x, ptComponent.y, rcComponent.Width(), rcComponent.Height(), &dcMem, 0, 0, SRCINVERT);
	//pDC->SetROP2(nOldMode);

	bmpMem.DeleteObject();
	dcMem.DeleteDC();

	this->ReleaseDC(pDC);
}

bool CDlgResult::IsPosCanPasteComponent(Panel* pPanel, Component* pComponent, PointFloat ptComponentUILeftTop)
{
	bool bRet = false;

	PointFloat ptInPanel = /*ConvertUIPosToPanelPos(pPanel, ptComponentUILeftTop)*/ptComponentUILeftTop;
	CSingleton* pSingleton = CSingleton::GetSingleton();
	if(pPanel->m_Material.CompareNoCase(pComponent->m_Material) == 0 \
		&& IsFloatEqual(pPanel->m_Thickness, pComponent->m_Thickness) \
		&&!IsComponentOverlap(pPanel, pComponent, ptInPanel, pSingleton->m_BaseInfo.m_SawKerfWidth) \
		&& IsComponentInPanelClipped(pPanel, pComponent, ptInPanel, pSingleton->m_BaseInfo.m_DeburringWidth))
	{
		return bRet = true;
	}

	return bRet;
}

PointFloat CDlgResult::ConvertUIPosToPanelPos(Panel* pPanel, CPoint ptInUI)
{
	float fScale = GetDrawingScale(pPanel->m_OrgLen, pPanel->m_OrgWidth);

	float fXClickedInPanel, fYClickedInPanel;
	CRect rcPanelUIRect = GetComponentUIRect(pPanel->m_OrgLen, pPanel->m_OrgWidth, 0, 0, pPanel->m_OrgLen, pPanel->m_OrgWidth);

	fXClickedInPanel = (ptInUI.x - rcPanelUIRect.left) / fScale;
	fYClickedInPanel = (-(ptInUI.y - rcPanelUIRect.bottom)) / fScale;
	PointFloat ptInPanel;
	ptInPanel.x = fXClickedInPanel;
	ptInPanel.y = fYClickedInPanel;

	return ptInPanel;
}



PanelViewingParam* CDlgResult::GetSelectedItemViewingParam()
{
	return m_pDlgTotalResult->GetSelectedItemViewingParam();
}

void CDlgResult::OnLbnSelchangeClipBoard()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CancelPasting();
}

void CDlgResult::CancelPasting()
{
	if(m_pPastingComponent)
	{
		PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
		if(pParam && pParam->m_pPanel)
		{
			if(m_bNeedToEraseOldPastingComponent)
			{
				DrawComponentToPos(m_pPastingComponent, m_ptOldPastingComponent, m_bOldPastingComponentOK, pParam->m_pPanel);
			}
		}

		delete m_pPastingComponent;
		m_pPastingComponent = NULL;
		m_bNeedToEraseOldPastingComponent = false;

	}
}

bool CDlgResult::IsAttachMode()
{
	return !IsKeyPressed(VK_CONTROL);
}

PointFloat CDlgResult::GetAttachPanelPos_Component(CPoint ptMouseInUI, float fComponentXLen, float fComponentYLen)
{
	//CPoint ptRet = ptMouseInUI;
	PointFloat ptfRet;

	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(pParam && pParam->m_pPanel)
	{
		CSingleton* pSingleton = CSingleton::GetSingleton();
		float fKerf = pSingleton->m_BaseInfo.m_SawKerfWidth;

		PointFloat ptMouseInPanel = ConvertUIPosToPanelPos(pParam->m_pPanel, ptMouseInUI);
		PointFloat ptAttachPosInPanel = GetAttachPos_Component(pParam->m_pPanel, ptMouseInPanel, fComponentXLen, fComponentYLen, fKerf/* + 1*/, ATTACH_DISTANCE);
		//ptRet = ConvertPanelPosToUIPos(pParam->m_pPanel, ptAttachPosInPanel);
		ptfRet = ptAttachPosInPanel;
	}

	return ptfRet;
}

CPoint CDlgResult::ConvertPanelPosToUIPos(Panel* pPanel, PointFloat ptInPanel)
{
	CPoint ptRet;

	float fPanelXLen = pPanel->m_OrgLen;
	float fPanelYLen = pPanel->m_OrgWidth;

	CRect rcDrawArea;
	rcDrawArea = GetPanelViewRect();

	float fScale = GetDrawingScale(fPanelXLen, fPanelYLen);

	CRect rcPanelArea(DRAW_PANEL_GAP/2, DRAW_PANEL_GAP/2, rcDrawArea.Width()-DRAW_PANEL_GAP/2, rcDrawArea.Height()-DRAW_PANEL_GAP/2);

	CRect rcPanel((rcDrawArea.Width()-fPanelXLen*fScale)/2, \
		(rcDrawArea.Height()-fPanelYLen*fScale)/2, \
		(rcDrawArea.Width()-fPanelXLen*fScale)/2 + fPanelXLen*fScale, \
		(rcDrawArea.Height()-fPanelYLen*fScale)/2 + fPanelYLen*fScale);

	ptRet.x = ptInPanel.x * fScale + rcPanel.left;
	ptRet.y = (fPanelYLen - ptInPanel.y)*fScale + rcPanel.top;

	return ptRet;
}

void CDlgResult::OnMenuRemainderCutting()
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;

	m_pRemainderCutting = new CutPath;
	m_nRemainderCuttingSettingState = CUTTING_SETTING_STATE_START;
	m_bNeedToEraseOldRemainderCutting = false;

	CPoint ptCursor;  
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	PointFloat fpCursorInPanel = ConvertUIPosToPanelPos(pParam->m_pPanel, ptCursor);
	m_pRemainderCutting->_x1 = m_pRemainderCutting->_x2 = fpCursorInPanel.x;
	m_pRemainderCutting->_y1 = m_pRemainderCutting->_y2 = fpCursorInPanel.y;
}

void CDlgResult::DrawRemainderCuttingToPos(CutPath& theRemainderCutting, Panel* pPanel)
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;

	PointFloat ptStartInPanel, ptEndInPanel;
	CPoint ptStartInUI, ptEndInUI;
	ptStartInPanel.x = theRemainderCutting._x1;
	ptStartInPanel.y = theRemainderCutting._y1;
	ptEndInPanel.x = theRemainderCutting._x2;
	ptEndInPanel.y = theRemainderCutting._y2;

	ptStartInUI = ConvertPanelPosToUIPos(pPanel, ptStartInPanel);
	ptEndInUI = ConvertPanelPosToUIPos(pPanel, ptEndInPanel);

	CDC* pDC = GetDC();
	CPen pen;
	pen.CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	CPen* pOldPen = pDC->SelectObject(&pen);
	int nOldMode = pDC->SetROP2(R2_XORPEN);
	//pDC->MoveTo(ptStartInUI);
	//pDC->LineTo(ptStartInUI.x+50 ,ptStartInUI.y+50);

	pDC->MoveTo(ptStartInUI.x - 5, ptStartInUI.y);
	pDC->LineTo(ptStartInUI.x + 5 ,ptStartInUI.y);
	pDC->MoveTo(ptStartInUI.x, ptStartInUI.y - 5);
	pDC->LineTo(ptStartInUI.x, ptStartInUI.y + 5);

	if(ptEndInUI != ptStartInUI)
	{
		pDC->MoveTo(ptStartInUI.x, ptStartInUI.y);
		pDC->LineTo(ptEndInUI.x, ptEndInUI.y);

		pDC->MoveTo(ptEndInUI.x - 5, ptEndInUI.y);
		pDC->LineTo(ptEndInUI.x + 5 ,ptEndInUI.y);
		pDC->MoveTo(ptEndInUI.x, ptEndInUI.y - 5);
		pDC->LineTo(ptEndInUI.x, ptEndInUI.y + 5);
	}
	
	pDC->SetROP2(nOldMode);
	pDC->SelectObject(pOldPen);
	
	pen.DeleteObject();

	this->ReleaseDC(pDC);
}

void CDlgResult::StopRemainderCutting()
{
	delete m_pRemainderCutting;
	m_pRemainderCutting = NULL;
	m_bNeedToEraseOldRemainderCutting = false;
	Invalidate();
}

PointFloat CDlgResult::GetAttachPanelPos_RemainderCutting_Start(CPoint ptMouseInUI)
{
	PointFloat ptRet;

	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(pParam && pParam->m_pPanel)
	{
		float fKerf = GetKerf();

		PointFloat ptMouseInPanel = ConvertUIPosToPanelPos(pParam->m_pPanel, ptMouseInUI);
		PointFloat ptAttachPosInPanel = GetAttachPos_RemainderCutting_Start(pParam->m_pPanel, ptMouseInPanel, fKerf/* + 1*/, ATTACH_DISTANCE);
		ptRet = ptAttachPosInPanel;
	}

	return ptRet;
}

PointFloat CDlgResult::GetAttachPanelPos_RemainderCutting_End(CPoint ptMouseInUI, PointFloat ptStartInPanel)
{
	PointFloat ptRet;

	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(pParam && pParam->m_pPanel)
	{
		float fKerf = GetKerf();

		PointFloat ptMouseInPanel = ConvertUIPosToPanelPos(pParam->m_pPanel, ptMouseInUI);
		PointFloat ptAttachPosInPanel = GetAttachPos_RemainderCutting_End(pParam->m_pPanel, ptMouseInPanel, fKerf/* + 1*/, ATTACH_DISTANCE, ptStartInPanel);
		ptRet = ptAttachPosInPanel;
	}

	return ptRet;
}

void CDlgResult::ResetPastingState()
{
	if(m_pPastingComponent)
		delete m_pPastingComponent;
	m_pPastingComponent = NULL;

	m_lbClipBoard.EmptyClipBoard();
	ShowOrHideClipboard();
}

void CDlgResult::ResetResultDlg()
{
	RefreshOptimizeResult();
	ResetPastingState();
}

LRESULT CDlgResult::OnShowOrHideClipboard(WPARAM wparam, LPARAM lparam)
{
	ShowOrHideClipboard();
	return 0;
}

BOOL CDlgResult::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)  
	{
		RotatePastingComponent();
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgResult::RotatePastingComponent()
{
	if(m_pPastingComponent)
	{
		CancelPasting();
		m_lbClipBoard.ItemRotate90(m_lbClipBoard.GetCurSel());
		PostMessage(WM_COMMAND,ID_MENU_PASTE_COMPONENT);
	}

}

void CDlgResult::PastingComponent_OnMouseMove(CPoint point)
{
	PanelViewingParam* pParam = m_pDlgTotalResult->GetSelectedItemViewingParam();
	if(!pParam || !pParam->m_pPanel)
		return;

	if(m_pPastingComponent)
	{
		if(m_bNeedToEraseOldPastingComponent)
		{
			DrawComponentToPos(m_pPastingComponent, m_ptOldPastingComponent, m_bOldPastingComponentOK, pParam->m_pPanel);
		}

		CPoint ptCur;
		if(!IsAttachMode())
			ptCur = point;
		else
		{
			//ptCur = GetAttachUIPos_Component(point, m_pPastingComponent->m_RealLength, m_pPastingComponent->m_RealWidth);
			m_ptfAttachPosInPanel = GetAttachPanelPos_Component(point, m_pPastingComponent->m_RealLength, m_pPastingComponent->m_RealWidth);
			ptCur = ConvertPanelPosToUIPos(pParam->m_pPanel, m_ptfAttachPosInPanel);
		}

		bool bPosOK = IsPosCanPasteComponent(pParam->m_pPanel, m_pPastingComponent, IsAttachMode() ? m_ptfAttachPosInPanel : ConvertUIPosToPanelPos(pParam->m_pPanel, ptCur));
		DrawComponentToPos(m_pPastingComponent, ptCur, bPosOK, pParam->m_pPanel);
		m_ptOldPastingComponent = ptCur;
		m_bOldPastingComponentOK = bPosOK;
		m_bNeedToEraseOldPastingComponent = true;
	}
}

void CDlgResult::OnMenuRotatePastingComponent()
{
	RotatePastingComponent();
}


void CDlgResult::ClearAllData()
{
	CSingleton* pSingleton = CSingleton::GetSingleton();

	pSingleton->ClearAllData();
	ResetResultDlg();
}



void CDlgResult::OnOpenSolution()
{
	// TODO: �ڴ���������������


	CSingleton* pSingleton = CSingleton::GetSingleton();

	CString strTmp;

	strTmp = _T("hgo");
	CString filter = "hgo �ļ�(*.hgo)|*.hgo|�����ļ� (*.*)|*.*||";
	CFileDialog fileDlg (true, _T("hgo"), _T("*.hgo"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);


//	CString strDefDir = SelectPathDlg().DefaultLoadPath();
//	fileDlg.m_ofn.lpstrInitialDir = strDefDir;
	if ( fileDlg.DoModal() == IDOK)
	{

		// �������һ�ε�����
		//pSingleton->ClearAllData();

		ClearAllData();
	


		m_vComponentInputItem.clear();

		CString strOpenedFile = fileDlg.GetPathName();
		//m_vOpenedFile.clear();
		//m_vOpenedFile.push_back(strOpenedFile);

		int Which = strOpenedFile.ReverseFind('.');  
		CString strExtName = strOpenedFile.Right(strOpenedFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

		if (strExtName.CompareNoCase(strTmp) == 0)
		{
			if(HgyReadWrite::ReadHgy(strOpenedFile) != true)
			{
				AfxMessageBox("�Ż��ļ�����!");
			}
			else
			{
				//GetDocument()->SetTitle(GetFileNameInPath(strOpenedFile));

				m_vComponentInputItem = pSingleton->m_vBackupComponentInputItem;

				for(int i = 0; i < m_vComponentInputItem.size(); i++)
					m_vComponentInputItem[i].m_uSerialID = CSingleton::GetSingleton()->m_uComponentInputItemSerialID++;

				// ������Ƿ��г����ߴ緶Χ�ģ�ɾ��
				//CheckComponentList(m_vComponentInputItem);
			}

		}

		// ����С��
		int nCpnNum = 0;
		for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
		{
			ComponentInputItem& item = *it;
			if (item.m_nCount > 0)
			{
				nCpnNum += item.m_nCount;
				it++;
			}
			else
			{
				it = m_vComponentInputItem.erase(it);
			}
		}

		// ���ô���
		ResetResultDlg();

		// ������������
		pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);
	}
}



/**  �����Ż����
	@param[in]		
	@param[out]		
	@return			void
	@warning		
	@note			
	@see            
	*/
void  CDlgResult::OnLayout()
{
	// ��ȡ��������
	UpdateData(TRUE);


	CSingleton* pSingleton = CSingleton::GetSingleton();

	// ����ԭʼ��Ϣ�������������顢ԭ�ϡ�����
	pSingleton->SetBackupComponentInputItem(m_vComponentInputItem);
	pSingleton->SetRawMaterialInfoList(m_vRawMaterialList);


	// �����������,׼���Ż�
	ClearAllData();

	int sln_index = 0;
	for (unsigned int raw_index = 0; raw_index < m_vRawMaterialList.size(); raw_index++)
	{
		RawMaterialInfo rm_info = m_vRawMaterialList.at(raw_index);

		if (CheckRawMaterialUsable(m_vComponentInputItem, rm_info, m_BaseInfo) == false)
		{
			continue;;
		}

		BaseInfo& info = pSingleton->m_BaseInfo;

		m_len = rm_info.m_PanelLength;
		m_width = rm_info.m_PanelWidth;


		info.m_PanelLength		=		m_len ;	

		if (m_width == 0.0)
		{
			info.m_WidthUnlimited	=		true;
			info.m_PanelWidth		=		DEFAULT_WIDTH;
		}
		else
		{
			info.m_PanelWidth		=		m_width;
		}


		info.m_x_space				=		m_x_space;			
		info.m_y_space				=		m_y_space;			
		info.m_left_offset			=		m_left_offset;		
		info.m_right_offset			=		m_right_offset;	
		info.m_top_offset			=		m_top_offset;		
		info.m_bottom_offset		=		m_bottom_offset;	

		info.m_LayoutOrg			=		m_arranging_origin;
		info.m_FirstSectionOPTimes =		m_BaseInfo.m_FirstSectionOPTimes;
		info.m_FirstSectionOPMethod =		m_BaseInfo.m_LayoutMethod;
		info.m_bCustomerFirst		=		m_BaseInfo.m_bCustomerFirst; 

		float offset = m_left_offset + m_right_offset;



		// ������Ҫ�Ż��İ������ֱ�Ӵ�����Ҫ�Ż��İ��������ԭʼ�����⵽�ƻ�
		vector<ComponentInputItem> vOptimizeComponent = m_vComponentInputItem;



		// �����Ż������Ϣ

		//pSingleton->ClearAllData();
		//((CMainFrame*)theApp.GetMainWnd())->GetAlgBaseInfo(pSingleton->m_BaseInfo);


		// ��һ�׶β���
		// 	pSingleton->m_BaseInfo.m_FirstSectionOPTimes = dlgOptimizeSetting.m_nEditStep1Count;
		// 	pSingleton->m_BaseInfo.m_FirstSectionOPMethod = dlgOptimizeSetting.m_nComboStep1Alg;
		// 	// �ڶ��׶β���
		// 	pSingleton->m_BaseInfo.m_SecondSectionOPTimes = dlgOptimizeSetting.m_nEditStep2Count;
		// 	// �����׶β���
		// 	pSingleton->m_BaseInfo.m_ThirdSectionOPTimes = dlgOptimizeSetting.m_nEditStep3Count;
		// 	pSingleton->m_BaseInfo.m_ThirdSectionOAccptableUtilization = dlgOptimizeSetting.m_fEditStep3AcceptableUti;
		// 
		// 	// ������Ϣ��������
		// 	pSingleton->m_BaseInfo.m_bDownerFaceFirst = dlgOptimizeSetting.m_valDownerInfoFirst;


		// ����ԭʼ�������
		pSingleton->SetBackupComponentInputItem(vOptimizeComponent);




		// ���������
		CheckAndDeleteOverSizeComponentList(vOptimizeComponent);



		// ����ԭ�㡢�Ż�����
		int Org = pSingleton->m_BaseInfo.m_LayoutOrg;
		ComponentList componentList;

		int nTotalCount = info.m_FirstSectionOPTimes;

		// �Ż�ѭ����ʼ
		for(int i_progress = 0; i_progress < nTotalCount; i_progress++)
		{



			// ��һ���Ż�
#if 1


			int i_first_op_times = i_progress;

			// �ͷŽ������ 
			pSingleton->ClearCurrentSolution();
			pSingleton->ClearRemainderManager();

			// �ͷ�С�����
			pSingleton->m_vComponentGroup.clear();

			// ����С�����
			ConvertInputInfoToComponentList(vOptimizeComponent, m_vPreCombineItem, componentList);

			// ���ڴ���������������������ʸ����������������Ż�ʱ���Ⱥ����Ƹ���һ��
			int text_index = i_progress%5;
			float rotate_limit = pSingleton->m_BaseInfo.m_PanelLength >  pSingleton->m_BaseInfo.m_PanelWidth ?  pSingleton->m_BaseInfo.m_PanelWidth :  pSingleton->m_BaseInfo.m_PanelLength ;

			rotate_limit -= offset /*2* pSingleton->m_BaseInfo.m_DeburringWidth*/;


			// 		if (text_index == 1)
			// 		{
			// 			for(int i_cpn = 0; i_cpn < componentList.size(); i_cpn++)
			// 			{
			// 				Component* pCpn = componentList.at(i_cpn);
			// 
			// 				// ȫ���ú�����һ��, ������ת�ĳ���
			// 				if (pCpn->m_Texture == TextureType_NO_TEXTURE &&
			// 					(pCpn->m_RealLength < rotate_limit && pCpn->m_RealWidth < rotate_limit))
			// 				{
			// 					pCpn->m_Texture = TextureType_H_TEXTURE;
			// 				}
			// 				else
			// 				{
			// 					int a = 0;
			// 				}
			// 			}
			// 
			// 		}
			// 		else if (text_index == 2)
			// 		{
			// 			for(int i_cpn = 0; i_cpn < componentList.size(); i_cpn++)
			// 			{
			// 				Component* pCpn = componentList.at(i_cpn);
			// 
			// 				// ȫ���ú�����һ��
			// 				if (pCpn->m_Texture == TextureType_NO_TEXTURE &&
			// 					(pCpn->m_RealLength < rotate_limit && pCpn->m_RealWidth < rotate_limit))
			// 				{
			// 					pCpn->m_Texture = TextureType_V_TEXTURE;
			// 				}
			// 				else
			// 				{
			// 					int a = 0;
			// 				}
			// 			}
			// 		}

			// ��ֵ����������Ż�ԭ��
			SplitComponentList(componentList, pSingleton->m_vComponentGroup);


			// �Ż�
			if (pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 0)			// ���������
			{
				pSingleton->New_Layout(0, CutDir_Horizon, Org);
			}
			else if ( pSingleton->m_BaseInfo.m_FirstSectionOPMethod == 1)	// ̰��
			{
				if (i_first_op_times == 2)
				{
					pSingleton->New_Layout(1, CutDir_Horizon, Org);
				}
				else if (i_first_op_times == 3)
				{
					pSingleton->New_Layout(1, CutDir_Vertical, Org);
				}
				else if (i_first_op_times == 4)
				{
					pSingleton->New_Layout(1, CutDir_Default, Org);
				}
				else
				{
					pSingleton->New_Layout(1, CutDir_Random, Org);
				}
			}
			else
			{
				// ��� ̰��+���������
				int flag = pSingleton->m_BaseInfo.m_FirstSectionOPTimes/2;

				if (i_first_op_times > flag) // ���
				{
					pSingleton->New_Layout(0, CutDir_Horizon, Org);
				}
				else
				{
					pSingleton->New_Layout(1, CutDir_Horizon, Org);
				}
			}


#endif


			// ���ݽϺõķ���
			pSingleton->BackupBetterSolution(sln_index);

		}

		// ����һ���ߴ��������
		sln_index++;




	}









	// ���½������ID�Ͱ��ID
	pSingleton->UpdateSlnNum();

	//���°�������㡢�ײ۵ȼӹ���Ϣ���Ű�����п����еİ������ת�ˣ������Ű�֮����ת���İ���������㡢�ײ�ҲҪ��֮��תһ�£�
	pSingleton->UpdateComponentMachiningInfo();
	pSingleton->UpdatePreCombinedComponent();

	ResetResultDlg();
}



/** �������С�Ƿ񳬳���ɾ���������
 *	@param[in]		
 *	@param[out]		
 *	@return			void
 *  @warning		
 *	@note			
 *	@see            
 */
void CDlgResult::CheckAndDeleteOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem)
{
	CSingleton* pSingleton = CSingleton::GetSingleton();
	BaseInfo base_info = pSingleton->m_BaseInfo;
	float panel_offset = base_info.m_left_offset + base_info.m_right_offset;
	vector<ComponentInputItem>::iterator it, it_begin, it_end;
	CString strMsg;

	for (it = vComponentInputItem.begin(); it != vComponentInputItem.end();)
	{
		ComponentInputItem& pCpn = *it;

		bool bOverSize = false;
		if (pCpn.m_strTexture == "������")
		{
			if(pCpn.m_fLength > base_info.m_PanelLength - panel_offset 
				|| pCpn.m_fWidth > base_info.m_PanelWidth - panel_offset
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// ��ת���ٴ��ж�
				if (pCpn.m_fLength >  base_info.m_PanelWidth - panel_offset 
					|| pCpn.m_fWidth > base_info.m_PanelLength - panel_offset
					|| pCpn.m_fLength <= 0
					|| pCpn.m_fWidth <= 0)
				{
					// ���ǳ�����ɾ��
					bOverSize = true;
				}
			}
		}
		else if(pCpn.m_strTexture == "����")
		{
			if (pCpn.m_fLength > base_info.m_PanelLength - panel_offset
				|| pCpn.m_fWidth > base_info.m_PanelWidth - panel_offset
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// ֱ��ɾ��
				bOverSize = true;
			}
		}
		else
		{
			if(pCpn.m_fLength >  base_info.m_PanelWidth - panel_offset 
				|| pCpn.m_fWidth > base_info.m_PanelLength - panel_offset
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// ֱ��ɾ��
				bOverSize = true;
			}
		}

		if(bOverSize)
		{
			// ����
			strMsg += "ɾ��������Χ���������ţ�" + pCpn.m_strBarcode + "\n";

			// ɾ��
			it = vComponentInputItem.erase(it);
		}
		else
		{
			it++;
		}
	}

	// ����Ϣ
	if (strMsg.IsEmpty() != true)
	{
		AfxMessageBox(strMsg);
	}


}

bool CDlgResult::CheckRawMaterialUsable(vector<ComponentInputItem>& vComponentInputItem, RawMaterialInfo rm_info, BaseInfo b_info)
{

	float panel_len = rm_info.m_PanelLength;
	float panel_width = rm_info.m_PanelWidth;
	float panel_offset = b_info.m_left_offset + b_info.m_right_offset;

	if (panel_width == 0.0)
	{
		panel_width = DEFAULT_WIDTH;

	}


	vector<ComponentInputItem>::iterator it, it_begin, it_end;
	CString strMsg;

	for (it = vComponentInputItem.begin(); it != vComponentInputItem.end();)
	{
		ComponentInputItem& pCpn = *it;

		bool bOverSize = false;
		if (pCpn.m_strTexture == "������")
		{
			if(pCpn.m_fLength > panel_len - panel_offset 
				|| pCpn.m_fWidth > panel_width - panel_offset
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// ��ת���ٴ��ж�
				if (pCpn.m_fLength >  panel_width - panel_offset
					|| pCpn.m_fWidth > panel_len - panel_offset
					|| pCpn.m_fLength <= 0
					|| pCpn.m_fWidth <= 0)
				{
					// ���ǳ�����ɾ��
					bOverSize = true;
				}
			}
		}
		else if(pCpn.m_strTexture == "����")
		{
			if (pCpn.m_fLength > panel_len - panel_offset 
				|| pCpn.m_fWidth > panel_width - panel_offset
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// ֱ��ɾ��
				bOverSize = true;
			}
		}
		else
		{
			if(pCpn.m_fLength >  panel_width - panel_offset 
				|| pCpn.m_fWidth > panel_len - panel_offset
				|| pCpn.m_fLength <= 0
				|| pCpn.m_fWidth <= 0)
			{
				// ֱ��ɾ��
				bOverSize = true;
			}
		}

		if(bOverSize)
		{
			// ����
			strMsg += "ɾ��������Χ���������ţ�" + pCpn.m_strBarcode + "\n";
			return false;
		}
		else
		{
			it++;
		}
	}

	// ����Ϣ
// 	if (strMsg.IsEmpty() != true)
// 	{
// 		AfxMessageBox(strMsg);
// 	}

	return true;
}


void CDlgResult::OnOptimize()
{
	// TODO: �ڴ���������������
	

}








#endif

bool  CDlgResult::CopyToClipboard(const char* pszData, const int nDataLen)
{
	if(::OpenClipboard(NULL))
	{
		::EmptyClipboard();
		HGLOBAL clipbuffer;
		char *buffer;
		clipbuffer = ::GlobalAlloc(GMEM_DDESHARE, nDataLen+1);
		buffer = (char *)::GlobalLock(clipbuffer);
		strcpy(buffer, pszData);
		::GlobalUnlock(clipbuffer);
		::SetClipboardData(CF_TEXT, clipbuffer);
		::CloseClipboard();
		return TRUE;
	}
	return FALSE;
}



void  CDlgResult::InputNormalString(string str)
{
	for(UINT i = 0; i < str.length(); i++)
	{
		char c = str.at(i);

		Sleep(SLEEP_1MS);

		if (c == '.')
		{
			keybd_event(VK_DECIMAL, 0, 0, 0);					// ����
			keybd_event(VK_DECIMAL, 0, KEYEVENTF_KEYUP, 0);		// ̧��
		}
		else
		{
			keybd_event(c, 0, 0, 0);					// ����
			keybd_event(c, 0, KEYEVENTF_KEYUP, 0);		// ̧��
		}
		
		
	}
}


// ���ñ༭����ֵ
void CDlgResult::setEditCtrlString(int pos_x, int pos_y, string str, int sleep_time)
{
	SetCursorPos(pos_x, pos_y);
	mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
	mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

	// ɾ����������
	keybd_event(VK_BACK, 0, 0, 0);
	keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);


	if (sleep_time > 0)
	{
		Sleep(sleep_time);
	}


	// ���õ����а�
	CopyToClipboard(str.c_str(), str.length());

	// ճ�� Ctrl+V
	keybd_event(VK_CONTROL, 0, 0, 0);				// ����ctrl
	keybd_event('V', 0, 0, 0);						// ����v
	keybd_event('V', 0, KEYEVENTF_KEYUP, 0);		// ̧��ctrl
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	// ̧��v
}




BOOL CALLBACK CDlgResult::EnumWindowsLikeProc(HWND hwnd,LPARAM lParam)
{
	CString* namelink = (CString*)lParam;
	TCHAR str[500];
	::GetWindowText(hwnd,str,sizeof(str));
	if(CString(str).Find(*namelink) != -1)
	{
		*namelink = CString(str);
		return 0;
	}
	return 1;
}



int CDlgResult::FindWindowLike(CString& namelink)
{
	EnumWindows(EnumWindowsLikeProc,(LPARAM)&namelink);
	return 0;
}



CWnd* CDlgResult::FindWndByCtrlID(CWnd* pWnd, UINT nCtrlID, LPCTSTR szClassName)
{
	CWnd* pFindingWnd = NULL;
	pFindingWnd = pWnd->GetWindow(GW_CHILD);
	while(pFindingWnd)
	{
		CString strTmp;
		TCHAR szCheckingClassName[MAX_PATH];
		GetClassName(pFindingWnd->GetSafeHwnd(), szCheckingClassName, MAX_PATH);
		strTmp.Format("ID=%d H=%x class=%s\n", pFindingWnd->GetDlgCtrlID(), pFindingWnd->GetSafeHwnd(), szCheckingClassName);
		OutputDebugString(strTmp);

		CString strFindingClassName = szClassName;
		if(pFindingWnd->GetDlgCtrlID() == nCtrlID && (strFindingClassName.IsEmpty() || strFindingClassName.CompareNoCase(szCheckingClassName) == 0))
		{
			return pFindingWnd;
		}

		CWnd* pRes = FindWndByCtrlID(pFindingWnd, nCtrlID, szClassName);
		if(pRes)
			return pRes;

		pFindingWnd = pFindingWnd->GetNextWindow();
	}

	return NULL;
}


void CDlgResult::EmptyCtrlContent(HWND hWnd)
{
	for(int j = 0; j < MAX_PATH; j++)
	{
		::SendMessage(hWnd, WM_KEYDOWN, VK_BACK, 0);
		::SendMessage(hWnd, WM_KEYUP, VK_BACK, 0);
	}
	for(int j = 0; j < MAX_PATH; j++)
	{
		::SendMessage(hWnd, WM_KEYDOWN, VK_DELETE, 0);
		::SendMessage(hWnd, WM_KEYUP, VK_DELETE, 0);
	}
}


void CDlgResult::SetCtrlText(CString title, int id,  CString ctrl_type, CString text)
{
	

	CString strMsg;

	strMsg.Format("Ĭ��ת������:%s", title);
	//AfxMessageBox(strMsg);
	CWnd* pDefTransWnd = NULL;

	CString strWndTitle = title;
	FindWindowLike(strWndTitle);
	pDefTransWnd = FindWindow(NULL, strWndTitle);


	if(pDefTransWnd && pDefTransWnd->GetSafeHwnd())
	{
		CString strBarcode = text;

		HWND hPopup = ::GetLastActivePopup(pDefTransWnd->GetSafeHwnd());

		CWnd wndPopup;
		wndPopup.Attach(hPopup);
		CWnd* pTargetCtrl = NULL;
		pTargetCtrl = FindWndByCtrlID(&wndPopup, id, ctrl_type);
		//CString strPrint = PrintAllCtrl(&wndPopup);
		//AfxMessageBox(strPrint);
		wndPopup.Detach();		

		if(pTargetCtrl && pTargetCtrl->GetSafeHwnd())
		{
			EmptyCtrlContent(pTargetCtrl->GetSafeHwnd());

			CString strTmp;
			TCHAR szClassName[MAX_PATH];
			GetClassName(pTargetCtrl->GetSafeHwnd(), szClassName, MAX_PATH);
			strTmp.Format("ID=%d H=%x class=%s\n", pTargetCtrl->GetDlgCtrlID(), pTargetCtrl->GetSafeHwnd(), szClassName);
			OutputDebugString(strTmp);
			for(int j = 0; j < strBarcode.GetLength(); j++)
			{
				::SendMessage(pTargetCtrl->GetSafeHwnd(), WM_CHAR, strBarcode.GetAt(j) & 0xFF, 0);
			}
			//::PostMessage(pTargetCtrl->GetSafeHwnd(), WM_KEYDOWN, VK_RETURN, 0);
		}
		else
		{
			strMsg.Format(_T("��Ĭ�ϴ������Ҳ����ؼ���ID=%d CtrlClassName=%s"), id, "Edit");
			AfxMessageBox(strMsg);
		}
	}
	else
	{
		strMsg.Format("�Ҳ���Ĭ�ϴ��ڣ�");
		AfxMessageBox(strMsg);
	}
}


void CDlgResult::SetBottomAction(CString title, int id, UINT action)
{
	CString strMsg;
	strMsg.Format("Ĭ��ת������:%s Ĭ��ת���ؼ�ID:%d", title, id);
	//AfxMessageBox(strMsg);
	CWnd* pDefTransWnd = NULL;

	CString strWndTitle = title;
	FindWindowLike(strWndTitle);
	pDefTransWnd = FindWindow(NULL, strWndTitle);
	

	if(pDefTransWnd && pDefTransWnd->GetSafeHwnd())
	{
		CWnd wndPopup;
		wndPopup.Attach(::GetLastActivePopup(pDefTransWnd->GetSafeHwnd()));

		CWnd* pDefTransCtrl = FindWndByCtrlID(/*pDefTransWnd*/&wndPopup, id, NULL);
		wndPopup.Detach();
		if(pDefTransCtrl && pDefTransCtrl->GetSafeHwnd())
		{
			strMsg.Format("�ҵ���Ĭ�ϴ��ڼ��ؼ���");
			//AfxMessageBox(strMsg);

			::SendMessage(pDefTransCtrl->GetSafeHwnd(), /*BM_CLICK*/action, 0, 0);

			//CWnd* pPostingWnd = pDefTransCtrl->GetParent();
			//while(pPostingWnd)
			//{
			//	::PostMessage(pDefTransCtrl->GetParent()->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(theAction.m_uCtrlID, BN_CLICKED), (LPARAM)(pDefTransCtrl->GetSafeHwnd()));
			//	pPostingWnd = pPostingWnd->GetParent();
			//}

			//::PostMessage(pDefTransCtrl->GetSafeHwnd(), WM_LBUTTONDOWN, 0, 0);
			//::PostMessage(pDefTransCtrl->GetSafeHwnd(), WM_LBUTTONUP, 0, 0);
		}
		else
		{
			strMsg.Format("�Ҳ���Ĭ�Ͽؼ���");
			AfxMessageBox(strMsg);
		}	
	}
	else
	{
		strMsg.Format("�Ҳ���Ĭ�ϴ��ڣ�");
		AfxMessageBox(strMsg);
	}
}














void CDlgResult::OnConnectMaintop()
{
	// ��ȡ��ǰ��������
	CSingleton* pSingleton = CSingleton::GetSingleton();
	
	int nSlnNum = pSingleton->m_BackupSolutionList.size();
	if (nSlnNum < 1)
	{
		return;
	}


	CSolution* pCurSln = pSingleton->m_BackupSolutionList.at(pSingleton->m_CurSlnIndex);

	int nPanelNum = pCurSln->GetPanelNum();
	if (nPanelNum < 1)
	{
		return;
	}

	Panel* pCurPanel = pCurSln->GetPanel(0);

	vector<Component*> cpn_list;
	pCurPanel->GetAllNeededComponent(cpn_list);


	// ��д�ļ���
	vector<string> file_list;
	vector<string> x_pos_list;
	vector<string> y_pos_list;


	//exe ·��
	string exe_path = m_BaseInfo.m_strMainTopPath;


	for (int i_cpn = 0; i_cpn < cpn_list.size(); i_cpn++)
	{
		Component* pCpn = cpn_list.at(i_cpn);

		string file_path = pCpn->m_strCabinetName;
		float x = pCpn->m_x;
		float y;
		if (pCpn->m_nRotatedAngle == 0)
		{

			y = pCurPanel->m_OrgWidth - pCpn->m_y - pCpn->m_RealWidth;	// ���Ͻ�Ϊԭ�㣬 yҪȡ��

		}
		else
		{
			y = pCurPanel->m_OrgWidth - pCpn->m_y;	// ���Ͻ�Ϊԭ�㣬 yҪȡ��


		}

		stringstream ss;

		ss << x;

		string str_x = ss.str();

		ss.clear();
		ss.str("");

		ss << y;

		string str_y = ss.str();


		file_list.push_back(file_path);
		x_pos_list.push_back(str_x);
		y_pos_list.push_back(str_y);

	}



#if 0
// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif����ͼƬ\\001.tif");
// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif����ͼƬ\\002.tif");
// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif����ͼƬ\\003.tif");
// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif����ͼƬ\\001.tif");
// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif����ͼƬ\\002.tif");
// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif����ͼƬ\\003.tif");
// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif����ͼƬ\\001.tif");
// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif����ͼƬ\\002.tif");
// 	file_list.push_back("C:\\Users\\admin\\Desktop\\tif����ͼƬ\\003.tif");


#else
// 	file_list.push_back("D:\\QQPCmgr\\Desktop\\tif����ͼƬ\\001.tif");
// 	file_list.push_back("D:\\QQPCmgr\\Desktop\\tif����ͼƬ\\002.tif");
// 	file_list.push_back("D:\\QQPCmgr\\Desktop\\tif����ͼƬ\\003.tif");
#endif







// 	x_pos_list.push_back("100");
// 	x_pos_list.push_back("200");
// 	x_pos_list.push_back("300");
	// 		x_pos_list.push_back("1500");
	// 		x_pos_list.push_back("1600");
	// 		x_pos_list.push_back("1700");
	// 		x_pos_list.push_back("3100");
	// 		x_pos_list.push_back("3200");
	// 		x_pos_list.push_back("3300");


// 	y_pos_list.push_back("10");
// 	y_pos_list.push_back("1800");
// 	y_pos_list.push_back("3600");
	// 		y_pos_list.push_back("10");
	// 		y_pos_list.push_back("1800");
	// 		y_pos_list.push_back("3600");
	// 		y_pos_list.push_back("10");
	// 		y_pos_list.push_back("1800");
	// 		y_pos_list.push_back("3600");




	CString exe_title;
	HWND exe_id ;

#if (CUR_VERSION == VERSION_NORMAL)

	exe_title = "��̩��ɫ���ӳ���ϵͳ V6.0(�ռ���)";

#else

	exe_title = "��̩��ɫ���ӳ���ϵͳ V6.0(רҵ��)";

#endif

		exe_id = ::FindWindow(NULL, exe_title);
		int find_exe_num = 0;
		int find_count = 0;

		if (exe_id == 0)
		{
			// ��������

			ShellExecute(NULL, "open", exe_path.c_str(), NULL, NULL, SW_SHOWNORMAL); 


			while(exe_id == 0)
			{
				Sleep(SLEEP_100MS);

				exe_id = ::FindWindow(NULL, exe_title);



				find_exe_num++;
				
				// 10��δ����
				if (find_exe_num >= FIND_TIMES)
				{
					AfxMessageBox("����20��δ�ҵ���̩���򴰿ڣ��˳���");
					return;
				}


			}

		}


		if (exe_id != NULL)
		{
			// �������Ƶ����
			::SendMessage(exe_id, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			::SetWindowPos(NULL, HWND_TOPMOST, 0,0, 1926,1446, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);

			Sleep(SLEEP_1000MS);

			bool show_coor_flag =false;																			 
			bool first_modify_coord_flag =false;																	 
			bool first_insert_flag =false;

			RECT exe_wnd_rect;
			::GetWindowRect(exe_id, &exe_wnd_rect);

			int x = exe_wnd_rect.left, y = exe_wnd_rect.top;


			// ���ڻ�ȡ����
			int x2 = (exe_wnd_rect.left + exe_wnd_rect.right)/2;
			int y2 = (exe_wnd_rect.top + exe_wnd_rect.bottom)/2;
			SetCursorPos(x2, y2);
			mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);


			/************************************************************************/
			/*                     �½�Ctrl+N                                      */
			/************************************************************************/ 
			
			keybd_event(VK_CONTROL, 0, 0, 0);				// ����ctrl
			keybd_event('N', 0, 0, 0);						// ����N
			keybd_event('N', 0, KEYEVENTF_KEYUP, 0);		// ̧��ctrl
			keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	// ̧��N

			// ����ֽ�Ŵ�С   ���ڡ��������ļ���
			find_count = 0;
			HWND new_file_dlg_id;
			CString new_file_dlg_title = "�������ļ�";
			while(!(new_file_dlg_id = ::FindWindow("#32770", new_file_dlg_title)))
			{
				Sleep(SLEEP_100MS);
				find_count++;

				// 10��δ����
				if (find_count >= FIND_TIMES)
				{
					AfxMessageBox("����20��δ�ҵ��������ļ����ڣ��˳���");
					return;
				}

			}


			if (new_file_dlg_id != NULL)
			{
				// ��������Ϊ100x100   ��220x100
				RECT new_file_dlg_rect;
				::GetWindowRect(new_file_dlg_id, &new_file_dlg_rect);

				int len_pos_x = new_file_dlg_rect.left + 100;
				int len_pos_y = new_file_dlg_rect.top + 100;


				int width_pos_x = new_file_dlg_rect.left + 220;
				int width_pos_y = new_file_dlg_rect.top + 100;

				stringstream ss ;

				ss << pCurPanel->m_OrgLen;

				string panel_len = ss.str();


				ss.clear();
				ss.str("");

				ss << pCurPanel->m_OrgWidth;

				string panel_width = ss.str();


#if 0
				// �� 10500 �� 10501
				int len_id = 10500, width_id = 10501;

				SetEditText(new_file_dlg_title, len_id, panel_len.c_str());

				SetEditText(new_file_dlg_title, width_id, panel_width.c_str());

				Sleep(SLEEP_3000MS);

#else
				Sleep(SLEEP_1000MS);
				setEditCtrlString(len_pos_x, len_pos_y, panel_len, SLEEP_10MS);
				Sleep(SLEEP_1000MS);

				setEditCtrlString(width_pos_x, width_pos_y, panel_width, SLEEP_10MS);
				//Sleep(SLEEP_1000MS);

#endif
				

				



			}
			else
			{
				AfxMessageBox("����10��δ�ҵ��������ļ����ڣ��˳���");
				return;
			}


			Sleep(SLEEP_500MS);


			// ����-ȷ�� �½��ļ�
			keybd_event(VK_RETURN, 0, 0, 0);
			keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);



			/************************************************************************/
			/*                     ѭ������ͼƬ                                     */
			/************************************************************************/ 
			string prev_file_path;
			RECT coor_dlg_rect;
			for (UINT i = 0; i < file_list.size();  i++)
			{
				Component* pCpn = cpn_list.at(i);
				string cur_file_path = file_list.at(i);
				string str_pos_x = x_pos_list.at(i);
				string str_pos_y = y_pos_list.at(i);



				// �Ƚ��ǲ���ͬһ���ļ�
				bool same_file_flag = false;
				if (prev_file_path == cur_file_path)
				{
					same_file_flag = true;
				}

				// ����һ�ļ������и�ֵ
				prev_file_path = cur_file_path;
			
				/************************************************************************/
				/*                     ͬһ��ͼƬ                                        */
				/************************************************************************/ 
				if (same_file_flag)
				{
					// ctrl+d ֱ�Ӹ���һ��
					keybd_event(VK_CONTROL, 0, 0,				0);
					keybd_event('D',		0, 0,				0);
					keybd_event('D',		0, KEYEVENTF_KEYUP, 0);
					keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);

					// ����λ��


					// ����x����
					POINT coor_x;
					coor_x.x = coor_dlg_rect.left + 35;
					coor_x.y = coor_dlg_rect.top + 8;

					SetCursorPos(coor_x.x, coor_x.y);
					mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
					mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

					if (pCpn->m_nRotatedAngle != 0)
					{
						// �ȵ����Ƕ�

						// ����table��
						keybd_event(VK_TAB, 0, 0, 0);
						keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);

						// ����table��
						keybd_event(VK_TAB, 0, 0, 0);
						keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);

						// ����table��
						keybd_event(VK_TAB, 0, 0, 0);
						keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);

						// ����table��
						keybd_event(VK_TAB, 0, 0, 0);
						keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);


						//Sleep(SLEEP_1000MS);
						// ����
						InputNormalString("90");
						Sleep(SLEEP_500MS);

						// ����-ȷ�� 
						keybd_event(VK_RETURN, 0, 0, 0);
						keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
						//Sleep(SLEEP_1000MS);

						SetCursorPos(coor_x.x, coor_x.y);
						mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
						mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
						//Sleep(SLEEP_1000MS);

					}









					// ����
					InputNormalString(str_pos_x);


					// ����table��
					keybd_event(VK_TAB, 0, 0, 0);
					keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);


					// ����
					InputNormalString(str_pos_y);


					// ����-ȷ�� 
					keybd_event(VK_RETURN, 0, 0, 0);
					keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);




				}
				else
				{

					/************************************************************************/
					/*                     ��ͬͼƬ                                        */
					/************************************************************************/ 

					int x = exe_wnd_rect.left + 80, y = exe_wnd_rect.top + 120;


					// ���ڻ�ȡ����

					SetCursorPos(x, y);
					mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);


					// ȡͼƬ�ļ����� Ctrl+I
					keybd_event(VK_CONTROL, 0, 0, 0);				// ����ctrl
					Sleep(SLEEP_10MS);
					keybd_event('I', 0, 0, 0);						// ����I
					Sleep(SLEEP_10MS);
					keybd_event('I', 0, KEYEVENTF_KEYUP, 0);		// ̧��ctrl
					Sleep(SLEEP_10MS);
					keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	// ̧��I



					HWND file_dlg_id;
					CString file_dlg_tile = "ȡͼƬ�ļ�";

					while(!(file_dlg_id = ::FindWindow("#32770", file_dlg_tile)))
					{
						Sleep(SLEEP_1000MS);
						find_count++;

						// 10��δ����
						if (find_count >= FIND_TIMES)
						{
							AfxMessageBox("����20��δ�ҵ�ȡͼƬ�ļ����ڣ��˳���");
							return;
						}

					}


					if (file_dlg_id != NULL)
					{
						RECT file_dlg_rect;
						::GetWindowRect(file_dlg_id, &file_dlg_rect);



#if 0
						int file_path_x = file_dlg_rect.left + 150;
						int file_path_y = file_dlg_rect.bottom - 65;

						SetCursorPos(file_path_x, file_path_y);
						mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
						mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

						// ɾ����������
						keybd_event(VK_BACK, 0, 0, 0);
						keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);

						if (first_insert_flag == false)
						{
							Sleep(SLEEP_1000MS);

							first_insert_flag = true;
						}

						// ���õ����а�
						CopyToClipboard(file_path.c_str(), file_path.length());

						// ճ�� Ctrl+V
						keybd_event(VK_CONTROL, 0, 0, 0);				// ����ctrl
						keybd_event('V', 0, 0, 0);						// ����v
						keybd_event('V', 0, KEYEVENTF_KEYUP, 0);		// ̧��ctrl
						keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	// ̧��v



#else

						int file_path_id = 1152;
						SetCtrlText(file_dlg_tile, file_path_id,"Edit", cur_file_path.c_str());


#endif




						Sleep(SLEEP_100MS);

						// ����-ȷ�� 

						keybd_event(VK_RETURN, 0, 0, 0);
						keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);


						Sleep(SLEEP_100MS);
						// ����-ȷ�� 
						keybd_event(VK_RETURN, 0, 0, 0);
						keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);


						Sleep(SLEEP_100MS);


					}
					else
					{
						AfxMessageBox("�ļ�����δ�ҵ���");
						return;
					}

					// ��ʾ��ע
					if (show_coor_flag == false)
					{
						Sleep(SLEEP_500MS);
						int show_coord_btn_x = exe_wnd_rect.left + 610;
						int show_coord_btn_y = exe_wnd_rect.top + 70;
						SetCursorPos(show_coord_btn_x, show_coord_btn_y);
						mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

						show_coor_flag = true;
					}


					// �޸ı�ע


					if (first_modify_coord_flag == false)
					{

						Sleep(SLEEP_500MS);
						first_modify_coord_flag = true;
					}


					//Sleep(SLEEP_1000MS);


					HWND parent_dlg_id;

					while(!(parent_dlg_id = ::FindWindowEx(exe_id, 0,"OGL_V30_Window", "")))
					{


						Sleep(SLEEP_100MS);
						find_count++;

						// 10��δ����
						if (find_count >= FIND_TIMES)
						{
							AfxMessageBox("����10��δ�ҵ���ͼƬ���길���ڡ����˳���");
							return;
						}

					}




					HWND coor_dlg_id;
					while(!(coor_dlg_id = ::FindWindowEx(parent_dlg_id, 0,"#32770", "")))
					{


						Sleep(SLEEP_100MS);
						find_count++;

						// 10��δ����
						if (find_count >= FIND_TIMES)
						{
							AfxMessageBox("����10��δ�ҵ���ͼƬ�����Ӵ��ڡ����˳���");
							return;
						}

					}



					if (coor_dlg_id != NULL)
					{
						::GetWindowRect(coor_dlg_id, &coor_dlg_rect);


						// ������Ҫ�ȶ�ͼƬ��������ԭ�гߴ硱�д���


#if 0
		
						// ������ƶ����������Ͻ�
						int img_attribule_x = exe_wnd_rect.left + 80, img_attribule_y = exe_wnd_rect.top + 120;
						SetCursorPos(img_attribule_x, img_attribule_y);

						mouse_event(MOUSEEVENTF_RIGHTDOWN|MOUSEEVENTF_RIGHTUP,0,0,0,0);
						Sleep(SLEEP_500MS);


						//ѡ���Ҽ��˵����������ԡ���A+Enter
						keybd_event('A', 0, 0, 0);						// ����v
						keybd_event('A', 0, KEYEVENTF_KEYUP, 0);		// ̧��ctrl

						Sleep(SLEEP_100MS);


						keybd_event(VK_RETURN, 0, 0, 0);
						keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
						Sleep(SLEEP_1MS);


#else

						// CTRL+A

						keybd_event(VK_CONTROL,	0, 0,				0);		// ����ctrl
						keybd_event('A',		0, 0,				0);		// ����A
						keybd_event('A',		0, KEYEVENTF_KEYUP, 0);		// ̧��A
						keybd_event(VK_CONTROL,	0, KEYEVENTF_KEYUP, 0);		// ̧��ctrl


#endif


						



						// ѡ�񴰿ڡ�ͼƬ�����ԡ� 


						HWND img_attribule_dlg_id;
						CString img_attribule_dlg_title = "ͼƬ������";

						while(!(img_attribule_dlg_id = ::FindWindow("#32770", img_attribule_dlg_title)))
						{


							Sleep(SLEEP_100MS);
							find_count++;

							// 10��δ����
							if (find_count >= FIND_TIMES)
							{
								AfxMessageBox("����10��δ�ҵ���ͼƬ�����ԡ����ڣ��˳���");
								return;
							}

						}


						if (img_attribule_dlg_id != NULL)
						{
							RECT img_attribule_dlg_rect;
							::GetWindowRect(img_attribule_dlg_id, &img_attribule_dlg_rect);


							// �������ͼƬԭ�ߴ� 75 290   ID=100

#if 0

							int keep_org_img_size_x = img_attribule_dlg_rect.left + 75, keep_org_img_size_y = img_attribule_dlg_rect.top + 290;

							SetCursorPos(keep_org_img_size_x, keep_org_img_size_y);

							mouse_event(MOUSEEVENTF_LEFTDOWN |MOUSEEVENTF_LEFTUP,0,0,0,0);

#else
							int keep_org_img_size_id = 100;

							SetBottomAction(img_attribule_dlg_title, keep_org_img_size_id, BM_CLICK);

#endif


							Sleep(SLEEP_100MS);

							// ���ȷ�� 270 290 ID = 1

#if 0

							int enter_x = img_attribule_dlg_rect.left + 270, enter_y = img_attribule_dlg_rect.top + 290;

							SetCursorPos(enter_x, enter_y);

							mouse_event(MOUSEEVENTF_LEFTDOWN |MOUSEEVENTF_LEFTUP,0,0,0,0);

#else

							int ok_id = 1;

							SetBottomAction(img_attribule_dlg_title, ok_id, BM_CLICK);



#endif


							Sleep(SLEEP_100MS);







#if 0

							//  x y w h r1 r2 id = 200~~205

							int x_id	= 200;
							int y_id	= 201;
							int w_id	= 202;
							int h_id	= 203;
							int r1_id	= 204;
							int r2_id	= 205;

							if (pCpn->m_nRotatedAngle != 0)
							{
								// �ȵ����Ƕ�



								//Sleep(SLEEP_1000MS);
								// ����
								SetCtrlText(exe_title, r1_id, "Edit", "90");
								Sleep(SLEEP_100MS);

								// ����-ȷ�� 
								keybd_event(VK_RETURN, 0, 0, 0);
								keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);

								Sleep(SLEEP_100MS);
							}


							SetCtrlText(exe_title, x_id, "Edit", str_pos_x.c_str());
							SetCtrlText(exe_title, y_id, "Edit", str_pos_y.c_str());

							Sleep(SLEEP_100MS);

#else

							// ����x����
							POINT coor_x;
							coor_x.x = coor_dlg_rect.left + 35;
							coor_x.y = coor_dlg_rect.top + 8;

							SetCursorPos(coor_x.x, coor_x.y);
							mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
							mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

							if (pCpn->m_nRotatedAngle != 0)
							{
								// �ȵ����Ƕ�

								// ����table��
								keybd_event(VK_TAB, 0, 0, 0);
								keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);

								// ����table��
								keybd_event(VK_TAB, 0, 0, 0);
								keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);

								// ����table��
								keybd_event(VK_TAB, 0, 0, 0);
								keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);

								// ����table��
								keybd_event(VK_TAB, 0, 0, 0);
								keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);


								//Sleep(SLEEP_1000MS);
								// ����
								InputNormalString("90");
								Sleep(SLEEP_500MS);

								// ����-ȷ�� 
								keybd_event(VK_RETURN, 0, 0, 0);
								keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
								//Sleep(SLEEP_1000MS);

								SetCursorPos(coor_x.x, coor_x.y);
								mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
								mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
								//Sleep(SLEEP_1000MS);

							}









							// ����
							InputNormalString(str_pos_x);

#if 0
							// ����y����
							// 			POINT coor_y;
							// 			coor_y.x = coor_dlg_rect.left + 35;
							// 			coor_y.y = coor_dlg_rect.top + 25;
							// 
							// 			SetCursorPos(coor_y.x, coor_y.y);
							// 			mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
							// 			mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
							//	
#endif

							


							// ����table��
							keybd_event(VK_TAB, 0, 0, 0);
							keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);




							// ����
							InputNormalString(str_pos_y);




#endif




							// ����-ȷ�� 
							keybd_event(VK_RETURN, 0, 0, 0);
							keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);

						}
					}
					else
					{
						AfxMessageBox("���괰��δ�ҵ���");
						return;
					}
				}



			}
		}
		else
		{
			AfxMessageBox("exe����δ�ҵ���");
			return;
		}

	










}











#define  INCH_TO_MM		(25.4)		// Ӣ��ת���� 

/**  ��ȡͼƬ�ļ�������
	@param[in]		
	@param[out]		
	@return			void
	@warning		
	@note			
	@see            
	*/
void CDlgResult::OnOpenSourcePicInfo()
{
	CString  m_strOpenedFile;
	CSingleton* pSingleton = CSingleton::GetSingleton();

// 	CString filter = "xml �ļ�(*.xml)|*.xml|�����ļ� (*.*)|*.*||";
// 	CFileDialog fileDlg (true, _T("xml"), _T("*.xml"), OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, filter, NULL);
// 	CString  m_strOpenedFile = fileDlg.GetPathName();
// 	if ( fileDlg.DoModal() == IDOK)


	m_strOpenedFile = m_strSrcFilePath.c_str();
	if (!m_strOpenedFile.IsEmpty())
	{

		// �������һ�ε�����
		ClearAllData();
		m_vComponentInputItem.clear();

		

		int Which = m_strOpenedFile.ReverseFind('.');  
		CString strExtName = m_strOpenedFile.Right(m_strOpenedFile.GetLength() - Which - 1);  
		strExtName.MakeLower();

		if (strExtName == "xml")
		{

			


			// �ĵ�
			TiXmlDocument* doc = new TiXmlDocument;
			doc->LoadFile(m_strOpenedFile.GetBuffer());

			// ���ڵ�
			TiXmlElement *pRootElement = doc->RootElement();
			if (pRootElement == NULL)
			{
				AfxMessageBox("���ļ�");
				open_rt = false;
				return ;
			}

			//ѭ����ȡͼƬ��Ϣ
			TiXmlElement* pPicSetElem = pRootElement->FirstChildElement("PictureSet"); 
			if (pPicSetElem != NULL)
			{
			
				for (TiXmlElement* pCurPic = pPicSetElem->FirstChildElement("Picture"); pCurPic != NULL; pCurPic = (TiXmlElement*)(pCurPic->NextSibling("Picture")))
				{
					string pic_path			= pCurPic->Attribute("path");
					string texture			= pCurPic->Attribute("Texture");
					string material			= pCurPic->Attribute("Material");
					string customer_info	= pCurPic->Attribute("Customer");
					int	num					= stoi(pCurPic->Attribute("Number"));
					float w_mm				= stof(pCurPic->Attribute("Length"));
					float h_mm				= stof(pCurPic->Attribute("Width"));

					// ����ͼƬ����

					// 						const wchar_t* pwc = HGCode::char_Gb2312_To_Unicode(pic_path.c_str());
					// 						Image tmp_img(pwc);
					// 
					// 
					// 						UINT w	= tmp_img.GetWidth();
					// 						UINT h	= tmp_img.GetHeight();
					// 						UINT hr = tmp_img.GetHorizontalResolution();		// dpi ÿӢ����ٸ����ص�
					// 						UINT vr = tmp_img.GetVerticalResolution();			// dpi
					// 
					// 						float w_inch = (1.0*w)/hr;
					// 						float h_inch = (1.0*h)/vr;
					// 
					// 						float w_mm = w_inch*INCH_TO_MM;
					// 						float h_mm = h_inch*INCH_TO_MM;


					// �γ�һ������
					ComponentInputItem componentInputItem;


					string file_name = pic_path.substr(pic_path.find_last_of('\\') + 1);

					componentInputItem.m_strBarcode			= file_name.c_str();
					componentInputItem.m_strCabinetName		= pic_path.c_str();
					componentInputItem.m_fLength			= w_mm;
					componentInputItem.m_fWidth				= h_mm;
					componentInputItem.m_nCount				= num;
					componentInputItem.m_strTexture			= texture.c_str();
					componentInputItem.m_strMaterial		= material.c_str();
					componentInputItem.m_strCustomerInfo	= customer_info.c_str();
					componentInputItem.m_strOtherShapeID	= "��0";
					componentInputItem.m_strAbnormalInfo	= OtherShapeHelper::GetPointInfoStringFromPointInfo(
						OtherShapeHelper::getPointInfoRectangle(
						string(GetFloatString((int)componentInputItem.m_eRotateType, 0)), \
						string(GetFloatString(componentInputItem.m_fLength, 1)), \
						string(GetFloatString(componentInputItem.m_fWidth, 1))) \
						).c_str();

					m_vComponentInputItem.push_back(componentInputItem);

				}
			}

			// ��ȡԭ����Ϣ
			TiXmlElement* pRMSetElem = pRootElement->FirstChildElement("RawMaterialSet"); 
			if (pRMSetElem != NULL)
			{
				for (TiXmlElement* pCurRM = pRMSetElem->FirstChildElement("RawMaterial"); pCurRM != NULL; pCurRM = (TiXmlElement*)(pCurRM->NextSibling("RawMaterial")))
				{
					string name = pCurRM->Attribute("Material");
					float len	=  stof(pCurRM->Attribute("Length"));
					float width	=  stof(pCurRM->Attribute("Width"));

					RawMaterialInfo rm_info;

					rm_info.m_Material		= name;
					rm_info.m_PanelLength	= len;
					rm_info.m_PanelWidth	= width;

					m_vRawMaterialList.push_back(rm_info);
				}
			}

			// ��ȡ��������
			TiXmlElement* pPrincipleSetElem = pRootElement->FirstChildElement("PrincipleSet"); ;
			if (pPrincipleSetElem != NULL)
			{

				TiXmlElement* pCurPrinciple = pPrincipleSetElem->FirstChildElement("Principle");

				if ( pCurPrinciple != NULL)
				{
					int		Method			=  stoi(pCurPrinciple->Attribute("Method"));
					int		OPTimes			=  stoi(pCurPrinciple->Attribute("OPTimes"));
					int		Origin			=  stoi(pCurPrinciple->Attribute("Origin"));
					float	XSpace			=  stof(pCurPrinciple->Attribute("XSpace"));
					float	YSpace			=  stof(pCurPrinciple->Attribute("YSpace"));
					float	LeftOffset		=  stof(pCurPrinciple->Attribute("LeftOffset"));
					float	RightOffset		=  stof(pCurPrinciple->Attribute("RightOffset"));
					float	TopOffset		=  stof(pCurPrinciple->Attribute("TopOffset"));
					float	BottomOffset	=  stof(pCurPrinciple->Attribute("BottomOffset"));	
					string	maintop_path	=  pCurPrinciple->Attribute("MainTopPath");	

					m_BaseInfo.m_LayoutMethod			=	Method		;
					m_BaseInfo.m_FirstSectionOPTimes	=	OPTimes		;
					m_BaseInfo.m_LayoutOrg				=	Origin		;
					m_BaseInfo.m_x_space				=	XSpace		;		
					m_BaseInfo.m_y_space				=	YSpace		;		
					m_BaseInfo.m_left_offset			=	LeftOffset	;	
					m_BaseInfo.m_right_offset			=	RightOffset	;	
					m_BaseInfo.m_top_offset				=	TopOffset	;	
					m_BaseInfo.m_bottom_offset			=	BottomOffset;
					m_BaseInfo.m_strMainTopPath			=	maintop_path;

					// 
					if (m_BaseInfo.m_LayoutMethod == 3)
					{
						m_BaseInfo.m_bCustomerFirst = true;
					}

				}
			}




			// ����С��
			int nCpnNum = 0;
			for(vector<ComponentInputItem>::iterator it = m_vComponentInputItem.begin(); it != m_vComponentInputItem.end(); )
			{
				ComponentInputItem& item = *it;
				if (item.m_nCount > 0)
				{
					nCpnNum += item.m_nCount;
					it++;
				}
				else
				{
					it = m_vComponentInputItem.erase(it);
				}
			}

			CString str;
			str.Format("%d", nCpnNum);
			str += "��С��";
			//AfxMessageBox(str);
		}
	}

	// ѡ����ʵ�ԭ��
	bool b_selected = false;
	RawMaterialInfo select_rm;
	for(vector<RawMaterialInfo>::iterator it = m_vRawMaterialList.begin(); it != m_vRawMaterialList.end(); it++)
	{
		if (CheckRawMaterialUsable(m_vComponentInputItem, *it, m_BaseInfo) == true)
		{
			b_selected = true;
			select_rm = *it;



			break;
		}
	}

	if (b_selected == true)
	{
		m_len				= select_rm.m_PanelLength			;	
		m_width				= select_rm.m_PanelWidth			;	
		m_x_space			= m_BaseInfo.m_x_space			;
		m_y_space			= m_BaseInfo.m_y_space			;
		m_left_offset		= m_BaseInfo.m_left_offset		;
		m_right_offset		= m_BaseInfo.m_right_offset	;
		m_top_offset		= m_BaseInfo.m_top_offset		;
		m_bottom_offset		= m_BaseInfo.m_bottom_offset	;
		m_arranging_origin	= m_BaseInfo.m_LayoutOrg		;

		UpdateData(FALSE);
		ResetResultDlg();
	}
	else
	{
		AfxMessageBox("û�к��ʵ�ԭ�ϣ��޷�����!");

		open_rt = false;
		return ;
	}


	open_rt = true;
}
