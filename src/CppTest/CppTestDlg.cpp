
// CppTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CppTest.h"
#include "CppTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "../NewUI/ExportAPI.h"


// �ж��Ƿ��ڲ��Ի�����
#ifdef DEBUG 

#pragma comment(lib, "../../lib/NewUId.lib")

#pragma  message(".......................................��ǰΪdebug�汾")

#else

#pragma comment(lib, "../../lib/NewUI.lib")

#pragma  message(".......................................��ǰΪrelease�汾")

#endif







// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCppTestDlg �Ի���




CCppTestDlg::CCppTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCppTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCppTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCppTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCppTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCppTestDlg ��Ϣ�������

BOOL CCppTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCppTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCppTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCppTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CALLBACK EnumWindowsLikeProc(HWND hwnd,LPARAM lParam)
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



int FindWindowLike(CString& namelink)
{
	EnumWindows(EnumWindowsLikeProc,(LPARAM)&namelink);
	return 0;
}



CWnd* FindWndByCtrlID(CWnd* pWnd, UINT nCtrlID, LPCTSTR szClassName)
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


void EmptyCtrlContent(HWND hWnd)
{
	for(int j = 0; j < MAX_PATH; j++)
	{
		::PostMessage(hWnd, WM_KEYDOWN, VK_BACK, 0);
		::PostMessage(hWnd, WM_KEYUP, VK_BACK, 0);
	}
	for(int j = 0; j < MAX_PATH; j++)
	{
		::PostMessage(hWnd, WM_KEYDOWN, VK_DELETE, 0);
		::PostMessage(hWnd, WM_KEYUP, VK_DELETE, 0);
	}
}


void CCppTestDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	

	//Test();
#if 0

	JT_Layouts("F:\\PictureLayout\\��ʵ����.xml");
#else

	JT_Layouts("E:\\Ԭ���������ļ���\\����\\PictureLayout\\��ʵ����_�������.xml");	
#endif


#if 0

		
		CString title = "��̩��ɫ���ӳ���ϵͳ V6.0(�ռ���)";
		int id = 200;

		CString strMsg;
// 		strMsg.Format("Ĭ��ת������:%s Ĭ��ת���ؼ�ID:%d", title, id);
// 		//AfxMessageBox(strMsg);
// 		CWnd* pDefTransWnd = NULL;
// 
// 		CString strWndTitle = title;
// 		FindWindowLike(strWndTitle);
// 		pDefTransWnd = FindWindow(NULL, strWndTitle);
// 
// 
// 		if(pDefTransWnd && pDefTransWnd->GetSafeHwnd())
// 		{
// 			CWnd wndPopup;
// 			wndPopup.Attach(::GetLastActivePopup(pDefTransWnd->GetSafeHwnd()));
// 
// 			CWnd* pDefTransCtrl = FindWndByCtrlID(/*pDefTransWnd*/&wndPopup, id, NULL);
// 			wndPopup.Detach();
// 			if(pDefTransCtrl && pDefTransCtrl->GetSafeHwnd())
// 			{
// 				strMsg.Format("�ҵ���Ĭ�ϴ��ڼ��ؼ���");
// 				//AfxMessageBox(strMsg);
// 
// 				::PostMessage(pDefTransCtrl->GetSafeHwnd(), BM_CLICK, 0, 0);
// 
// 				//CWnd* pPostingWnd = pDefTransCtrl->GetParent();
// 				//while(pPostingWnd)
// 				//{
// 				//	::PostMessage(pDefTransCtrl->GetParent()->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(theAction.m_uCtrlID, BN_CLICKED), (LPARAM)(pDefTransCtrl->GetSafeHwnd()));
// 				//	pPostingWnd = pPostingWnd->GetParent();
// 				//}
// 
// 				//::PostMessage(pDefTransCtrl->GetSafeHwnd(), WM_LBUTTONDOWN, 0, 0);
// 				//::PostMessage(pDefTransCtrl->GetSafeHwnd(), WM_LBUTTONUP, 0, 0);
// 			}
// 			else
// 			{
// 				strMsg.Format("�Ҳ���Ĭ�Ͽؼ���");
// 				AfxMessageBox(strMsg);
// 			}	
// 		}
// 		else
// 		{
// 			strMsg.Format("�Ҳ���Ĭ�ϴ��ڣ�");
// 			AfxMessageBox(strMsg);
// 		}



			
				strMsg.Format("Ĭ��ת������:%s", title);
				//AfxMessageBox(strMsg);
				CWnd* pDefTransWnd = NULL;

				CString strWndTitle = title;
				FindWindowLike(strWndTitle);
				pDefTransWnd = FindWindow(NULL, strWndTitle);


				if(pDefTransWnd && pDefTransWnd->GetSafeHwnd())
				{
					CString strBarcode = "100";
// 					((CEdit*)GetDlgItem(IDC_EDIT_BARCODE))->GetWindowText(strBarcode);
// 					strBarcode += m_config.m_strAddSuffix;
					HWND hPopup = ::GetLastActivePopup(pDefTransWnd->GetSafeHwnd());

					CWnd wndPopup;
					wndPopup.Attach(hPopup);
					CWnd* pTargetCtrl = NULL;
					pTargetCtrl = FindWndByCtrlID(&wndPopup, id, "Edit");
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
							::PostMessage(pTargetCtrl->GetSafeHwnd(), WM_CHAR, strBarcode.GetAt(j), 0);
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






#endif







	//CDialogEx::OnOK();
}
