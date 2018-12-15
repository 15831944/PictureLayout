// DirSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptimizeDlg.h"

#include "../DataProcess/DataProcess.h"

#include "../../../include/DataManager/BaseDataType/CSingleton/CSingleton.h"
#include "../../../include/DataManager/BaseDataType/CSolution/CSolution.h"
#include "../../../include/DataManager/BaseDataType/Component/Component.h"


// ÿ�Ż�һ�ַ�һ�ν��
#define WM_CALCULATE			WM_USER + 10086
#define WM_UPDATE_PROGRESS_BAR	WM_USER + 10087
#define WM_UPDATE_REMAIN_TIME	WM_USER + 10088

int							OptimizeDlg::m_LastPanelNum = 0;
BOOL						OptimizeDlg::m_ThreadIsRunning = FALSE;
vector<ComponentInputItem>	OptimizeDlg::m_vComponentInputItem;
vector<PreCombineItem>		OptimizeDlg::m_vPreCombineItem;
vector<RemainderInputItem>  OptimizeDlg::m_vRemainderInputItem;







 BaseInfo					OptimizeDlg::m_BaseInfo;
 vector<RawMaterialInfo>	OptimizeDlg::m_vRawMaterialList;								 

 float	OptimizeDlg::m_len;				
 float	OptimizeDlg::m_width;	
 float	OptimizeDlg::m_x_space;			
 float	OptimizeDlg::m_y_space;			
 float	OptimizeDlg::m_left_offset;		
 float	OptimizeDlg::m_right_offset;		
 float	OptimizeDlg::m_top_offset;		
 float	OptimizeDlg::m_bottom_offset;		
 int	OptimizeDlg::m_arranging_origin;	

// CDirSetDlg dialog

IMPLEMENT_DYNAMIC(OptimizeDlg, CDialogEx)

OptimizeDlg::OptimizeDlg(CWnd* pParent /*=NULL*/)
			: CDialogEx(OptimizeDlg::IDD, pParent)
{
	// ���ݳ�ʼ��
	m_LastPanelNum = 0;
	m_ThreadIsRunning = FALSE;
	m_vComponentInputItem.clear();






}

OptimizeDlg::~OptimizeDlg()
{
}

void OptimizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_OP_MESSAGE, m_TxtOpMessage);
	DDX_Control(pDX, IDC_BTN_PROGRESS_BAR, m_ProgressBar);
	DDX_Control(pDX, IDC_STATIC_OP_MESSAGE2, m_TxtOpMessage2);
	DDX_Control(pDX, IDC_LIST_SOLUTION_INFO, m_ListCtrlSolutionInfo);
	DDX_Control(pDX, IDC_BTN_USE_NOW, m_BtnUseNow);
	DDX_Control(pDX, IDC_STATIC_OP_REMAIN_TIME, m_TxtRemainTime);
}


BEGIN_MESSAGE_MAP(OptimizeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &OptimizeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &OptimizeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_USE_NOW, &OptimizeDlg::OnBnClickedUseNow)

	ON_MESSAGE(WM_CALCULATE, &OptimizeDlg::OnWM_CALCULATE)								//��Ӧ
	ON_MESSAGE(WM_UPDATE_PROGRESS_BAR, &OptimizeDlg::OnWM_WM_UPDATE_PROGRESS_BAR)		//��Ӧ
	ON_MESSAGE(WM_UPDATE_REMAIN_TIME, &OptimizeDlg::OnWM_WM_UPDATE_REMAIN_TIME)			//��Ӧ
	



END_MESSAGE_MAP()


void OptimizeDlg::OnBnClickedOk()
{
	

	CDialogEx::OnOK();
}


void OptimizeDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}






/*---------------------------------------*/
//	����˵����
//		���ʹ�ã�ֹͣ�߳�
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void OptimizeDlg::OnBnClickedUseNow() 
{
	if (m_ThreadIsRunning == TRUE)
	{
		m_ThreadIsRunning = FALSE;
		DWORD dw = WaitForSingleObject(m_hdthread, INFINITE);	// �ȴ��߳̽���
		switch(dw)
		{
		case WAIT_OBJECT_0:	
			// The process terminated.
			// �߳��˳�
			CloseHandle(m_hdthread);
			CDialogEx::OnOK();
			break;

		case WAIT_TIMEOUT:
			// The process did not terminate within 5000 milliseconds.
			CloseHandle(m_hdthread);
			AfxMessageBox("�Ż��̳߳�ʱ������");
			CDialogEx::OnOK();
			break;

		case WAIT_FAILED:
			// Bad call to function (invalid handle?)
			CloseHandle(m_hdthread);
			AfxMessageBox("�Ż��߳��˳���Ч��");
			CDialogEx::OnOK();
			break;
		}
	}
	else
	{
		CDialogEx::OnOK();
	}

}

//���ڳ�ʼ��
BOOL OptimizeDlg::OnInitDialog()					
{
	CDialogEx::OnInitDialog();

	// �ؼ���ʼ��
	//CFont font;
	//font.CreatePointFont(100, "Arial");

	//m_ListCtrlSolutionInfo.SetFont(&font);
	HWND hwnd = ::GetDlgItem(m_ListCtrlSolutionInfo.m_hWnd, 0); 
	if(hwnd) 
	{ 
		CWnd* pHeader = CWnd::FromHandlePermanent(hwnd); 
		if(pHeader) 
		{ 
			if(!pHeader->IsKindOf(RUNTIME_CLASS(CSkinHeaderCtrl))) 
			{ 
				pHeader->Detach(); 
				m_hcSolutionInfo.SubclassWindow(hwnd); 
			} 
		} 
		else
		{
			m_hcSolutionInfo.SubclassWindow(hwnd);
		}
	}

	LONG dwStyle = GetWindowLong(m_ListCtrlSolutionInfo.m_hWnd, GWL_STYLE);
	SetWindowLong(m_ListCtrlSolutionInfo.m_hWnd, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK)|LVS_REPORT|LVS_OWNERDRAWFIXED);
	m_ListCtrlSolutionInfo.SetExtendedStyle((m_ListCtrlSolutionInfo.GetExtendedStyle() & ~LVS_EX_BORDERSELECT | LVS_EX_SUBITEMIMAGES| LVS_EX_FULLROWSELECT)/*& ~LVS_EX_INFOTIP*/);

	CRect rcListCtrl;
	m_ListCtrlSolutionInfo.GetWindowRect(rcListCtrl);
	ScreenToClient(rcListCtrl);
	rcListCtrl.right+=5;
	m_ListCtrlSolutionInfo.MoveWindow(rcListCtrl);

	m_ListCtrlSolutionInfo.InsertColumn(0,"���ϲ���",0,180);
	m_ListCtrlSolutionInfo.InsertColumn(1,"���",0,60);
	m_ListCtrlSolutionInfo.InsertColumn(2,"���",0,120);
	m_ListCtrlSolutionInfo.InsertColumn(3,"����",0,60);

	// ����Ӧ�ð���
	m_BtnUseNow.EnableWindow(FALSE);



	// ��ʼ�Ż��߳�
	StartOptimizeThread();

	return  TRUE;

}

/*---------------------------------------*/
//	����˵����
//		�����Ż��߳�
//
//
//	������
//
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
void OptimizeDlg::StartOptimizeThread()
{
	
	m_hdthread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)OptimizeThread, this, NULL, NULL);
	if (m_hdthread != INVALID_HANDLE_VALUE)
	{
		m_ThreadIsRunning = TRUE;	// ����
	}



}


bool OptimizeDlg::CheckRawMaterialUsable(vector<ComponentInputItem>& vComponentInputItem, RawMaterialInfo rm_info, BaseInfo b_info)
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

/** �������С�Ƿ񳬳���ɾ���������
 *	@param[in]		
 *	@param[out]		
 *	@return			void
 *  @warning		
 *	@note			
 *	@see            
 */
void OptimizeDlg::CheckAndDeleteOverSizeComponentList(vector<ComponentInputItem>& vComponentInputItem)
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

/*---------------------------------------*/
//	����˵����
//		�̺߳����������Ż�
//
//
//	������
//		PVOID lpThreadParameter		-- �Ż�����ָ��
//
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
DWORD OptimizeDlg::OptimizeThread( PVOID lpThreadParameter )
{
	OptimizeDlg* self = (OptimizeDlg*)( lpThreadParameter);

	// ʱ�����
	CTime last_time = CTime::GetCurrentTime();
	CTime cur_time;
	CTimeSpan  timeSpan;

	CSingleton* pSingleton = CSingleton::GetSingleton();





	int sln_index = 0;
	for (unsigned int raw_index = 0; raw_index < m_vRawMaterialList.size(); raw_index++)
	{
		RawMaterialInfo rm_info = m_vRawMaterialList.at(raw_index);

		if (CheckRawMaterialUsable(m_vComponentInputItem, rm_info, m_BaseInfo) == false)
		{
			continue;;
		}

		BaseInfo& singleton_info = pSingleton->m_BaseInfo;

		m_BaseInfo.m_PanelLength		= rm_info.m_PanelLength;
		m_BaseInfo.m_PanelWidth			= rm_info.m_PanelWidth;


		singleton_info.m_PanelLength		=		m_BaseInfo.m_PanelLength ;	

		if (m_BaseInfo.m_PanelWidth == 0.0)
		{
			singleton_info.m_WidthUnlimited	=		true;
			singleton_info.m_PanelWidth		=		DEFAULT_WIDTH;
		}
		else
		{
			singleton_info.m_PanelWidth		=		m_width;
		}


		singleton_info.m_x_space				=		m_BaseInfo.m_x_space;			
		singleton_info.m_y_space				=		m_BaseInfo.m_y_space;			
		singleton_info.m_left_offset			=		m_BaseInfo.m_left_offset;		
		singleton_info.m_right_offset			=		m_BaseInfo.m_right_offset;	
		singleton_info.m_top_offset				=		m_BaseInfo.m_top_offset;		
		singleton_info.m_bottom_offset			=		m_BaseInfo.m_bottom_offset;	

		singleton_info.m_LayoutOrg				=		m_BaseInfo.m_LayoutOrg;
		singleton_info.m_FirstSectionOPTimes	=		m_BaseInfo.m_FirstSectionOPTimes;
		singleton_info.m_FirstSectionOPMethod	=		m_BaseInfo.m_LayoutMethod;
		singleton_info.m_bCustomerFirst			=		m_BaseInfo.m_bCustomerFirst; 

		float offset = m_BaseInfo.m_left_offset + m_BaseInfo.m_right_offset;



		// ������Ҫ�Ż��İ������ֱ�Ӵ�����Ҫ�Ż��İ��������ԭʼ�����⵽�ƻ�
		vector<ComponentInputItem> vOptimizeComponent = m_vComponentInputItem;


		// ����ԭʼ�������
		pSingleton->SetBackupComponentInputItem(vOptimizeComponent);




		// ���������
		CheckAndDeleteOverSizeComponentList(vOptimizeComponent);



		// ����ԭ�㡢�Ż�����
		int Org = pSingleton->m_BaseInfo.m_LayoutOrg;
		ComponentList componentList;

		int nTotalCount = singleton_info.m_FirstSectionOPTimes;

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

			rotate_limit -= offset ;


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


			{
				// ÿ������һ�ַ�һ�μ��������Ϣ
				int nPanelCount = pSingleton->GetBackupSolutionPanelNum(); 


				cur_time = CTime::GetCurrentTime();
				timeSpan = cur_time - last_time;

				int seconds = timeSpan.GetTotalSeconds();


				// ������һ���򴰿ڷ���ˢ�����ֺͽ�������Ϣ
				if (i_progress%10 == 0)
				{
					::PostMessage(self->GetSafeHwnd(), WM_CALCULATE, 0, nPanelCount);
					::PostMessage(self->GetSafeHwnd(), WM_UPDATE_PROGRESS_BAR, nTotalCount, i_progress);
					::PostMessage(self->GetSafeHwnd(), WM_UPDATE_REMAIN_TIME, (nTotalCount<<16) |  i_progress,  seconds);
				}

				// �ж��߳��Ƿ��������
				if (m_ThreadIsRunning == FALSE)
				{
					return 0;
				}
			}






		}

		// ����һ���ߴ��������
		sln_index++;



		




	}





























	

	// �Ż�����
	::PostMessage(self->GetSafeHwnd(), WM_CLOSE, 0, 0);

	return 0;
}


//��ɹر�
LRESULT OptimizeDlg::OnWM_CALCULATE(WPARAM wParam, LPARAM lParam)
{
	int messageinfo = (int)wParam;
	int showmessage = (int)lParam;
	CString strMsg =  "Ŀǰ����:";

	
	switch(messageinfo)
	{
	case 0:

		if (m_LastPanelNum == 0)
		{
			m_LastPanelNum = showmessage;
		}
		else
		{
			if (showmessage < m_LastPanelNum)
			{
				m_LastPanelNum = showmessage;
				AfxMessageBox("�и��ŵķ������֣�");
			}
		}
		strMsg.Format("����һ�׽������: %d ���塣�����Ӧ�á���ť���ã�", showmessage);

		m_TxtOpMessage.SetWindowText(strMsg);
		break;
	default:
		break;
	}

	

	return 0;
}

/*---------------------------------------*/
//	����˵����
//	���½�����
//
//
//	������
//		WPARAM wParam	--	�ܴ���
//		LPARAM lParam	--	��ǰ����
//
//
//	����ֵ:
//
//
/*---------------------------------------*/
LRESULT OptimizeDlg::OnWM_WM_UPDATE_PROGRESS_BAR(WPARAM wParam, LPARAM lParam)
{
	int first_message = (int)wParam;
	int second_massage = (int)lParam;

	// ���½�����
	m_ProgressBar.SetRange32(0, first_message);
	second_massage = second_massage % first_message+1;
	m_ProgressBar.SetPos(second_massage);

	// �����ı�����
	float cur_time = lParam;
	float total_time = wParam;
	float progress = cur_time*100/total_time;
	CString strProgress;

	strProgress.Format("��ǰ�Ż����ȣ�%0.2f%%......", progress);
	m_TxtOpMessage2.SetWindowText(strProgress);




	// �����Ż���Ϣ

	CSingleton* pSingleton = CSingleton::GetSingleton();
	int nSlnCount = pSingleton->GetBackupSolutionNum();

	m_ListCtrlSolutionInfo.DeleteAllItems();
	for (int i = 0 ; i< nSlnCount ; i++)
	{
		m_ListCtrlSolutionInfo.InsertItem(i,"");
	}

	for (int iSln = 0; iSln < nSlnCount; iSln++)
	{
		CSolution* pSln = pSingleton->m_BackupSolutionList.at(iSln);

		CString str_thickness;
		CString str_panel_size;
		CString str_panel_num;

		str_thickness.Format("%0.0f", pSln->m_fThickness);
		str_panel_size.Format("%0.0f x %0.0f", pSln->m_BaseInfo.m_PanelLength, pSln->m_BaseInfo.m_PanelWidth);
		str_panel_num.Format("%d", pSln->GetPanelNum());

		m_ListCtrlSolutionInfo.SetItemText(iSln, 0, pSln->m_strMaterial);
		m_ListCtrlSolutionInfo.SetItemText(iSln, 1, str_thickness);
		m_ListCtrlSolutionInfo.SetItemText(iSln, 2, str_panel_size);
		m_ListCtrlSolutionInfo.SetItemText(iSln, 3, str_panel_num);
	}

	// ʹ��Ӧ�ð���
	m_BtnUseNow.EnableWindow(TRUE);




	return 0;
}


LRESULT OptimizeDlg::OnWM_WM_UPDATE_REMAIN_TIME(WPARAM wParam, LPARAM lParam)
{
	float total_count = wParam>>16;
	float cur_count = wParam & 0xffff;
	float seconds_cur_count = lParam;
	
	// 
	if (cur_count < 1)
	{
		cur_count = 1;
	}

	float remain_time = seconds_cur_count/cur_count*total_count - seconds_cur_count;

	CString strProgress;
	strProgress.Format("ʣ��ʱ�䣺%0.0f��......", remain_time);



	m_TxtRemainTime.SetWindowText(strProgress);

	return 0;
}

BOOL OptimizeDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)  
	{
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if(pMsg->wParam == VK_RETURN)
		{
			if(m_BtnUseNow.IsWindowEnabled())
			{
				PostMessage(WM_COMMAND, MAKELONG(IDC_BTN_USE_NOW, BN_CLICKED), (LPARAM)0/*(GetDlgItem(IDC_BTN_USE_NOW)->GetSafeHwnd())*/);  
			}
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}