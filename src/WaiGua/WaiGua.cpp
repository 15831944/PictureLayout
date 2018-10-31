// WaiGua.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#include <WinUser.h>
#include <stdio.h>
#include <stdlib.h>
#include <atltypes.h>
#include <string>
#include <vector>
#include <cstdio>
#include <iostream>



using namespace std;

#pragma comment (lib, "User32.lib")



#define  SLEEP_1MS			(1)
#define  SLEEP_10MS			(10)
#define  SLEEP_100MS		(10)
#define  SLEEP_1000MS		(1000)
#define  SLEEP_2000MS		(2000)
#define  SLEEP_3000MS		(3000)
#define  SLEEP_4000MS		(4000)
#define  SLEEP_5000MS		(5000)

BOOL CopyToClipboard(const char* pszData, const int nDataLen)
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







void InputNormalString(string str)
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








int _tmain(int argc, _TCHAR* argv[])
{
	// ��ȡ��ǰ��������
	


	// ��д�ļ���
	vector<string> file_list;
	vector<string> x_pos_list;
	vector<string> y_pos_list;


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

	file_list.push_back("D:\\QQPCmgr\\Desktop\\tif����ͼƬ\\001.tif");
	file_list.push_back("D:\\QQPCmgr\\Desktop\\tif����ͼƬ\\002.tif");
	file_list.push_back("D:\\QQPCmgr\\Desktop\\tif����ͼƬ\\003.tif");

#endif







	x_pos_list.push_back("100");
	x_pos_list.push_back("200");
	x_pos_list.push_back("300");
	// 		x_pos_list.push_back("1500");
	// 		x_pos_list.push_back("1600");
	// 		x_pos_list.push_back("1700");
	// 		x_pos_list.push_back("3100");
	// 		x_pos_list.push_back("3200");
	// 		x_pos_list.push_back("3300");


	y_pos_list.push_back("10");
	y_pos_list.push_back("1800");
	y_pos_list.push_back("3600");
	// 		y_pos_list.push_back("10");
	// 		y_pos_list.push_back("1800");
	// 		y_pos_list.push_back("3600");
	// 		y_pos_list.push_back("10");
	// 		y_pos_list.push_back("1800");
	// 		y_pos_list.push_back("3600");








	//HWND exe_id = FindWindow(NULL, "��̩��ɫ���ӳ���ϵͳ V6.0(רҵ��)");
	HWND exe_id = ::FindWindow(NULL, "��̩��ɫ���ӳ���ϵͳ V6.0(�ռ���)");

	int find_exe_num = 0;

	if (exe_id == 0)
	{
		// ��������
#if 0

		//ShellExecute(NULL, "open", "E:\\Ԭ���������ļ���\\����\\MainTop\\DTP\\dtpw.exe", NULL, NULL, SW_SHOWNORMAL); 

#else

		ShellExecute(NULL, "open", "F:\\MainTop\\DTP\\dtpw.exe", NULL, NULL, SW_SHOWNORMAL); 

#endif




		while(exe_id == 0)
		{
			Sleep(100);
			exe_id = ::FindWindow(NULL, "��̩��ɫ���ӳ���ϵͳ V6.0(�ռ���)");
			find_exe_num++;

		}

	}


	if (exe_id != NULL)
	{
		// �������Ƶ����
		::SendMessage(exe_id, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		::SetWindowPos(NULL, HWND_TOPMOST, 0,0, 1926,1446, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);

		Sleep(SLEEP_1000MS);

		bool show_coor_flag =false;																			 
		bool insert_flag =false;
		RECT exe_wnd_rect;
		::GetWindowRect(exe_id, &exe_wnd_rect);

		int x = exe_wnd_rect.left, y = exe_wnd_rect.top;


		// ���ڻ�ȡ����
		int x2 = (exe_wnd_rect.left + exe_wnd_rect.right)/2;
		int y2 = (exe_wnd_rect.top + exe_wnd_rect.bottom)/2;
		SetCursorPos(x2, y2);
		mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);


		// �½�Ctrl+N
		keybd_event(VK_CONTROL, 0, 0, 0);				// ����ctrl
		keybd_event('N', 0, 0, 0);						// ����N
		keybd_event('N', 0, KEYEVENTF_KEYUP, 0);		// ̧��ctrl
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	// ̧��N


		// ����-ȷ�� �½��ļ�
		keybd_event(VK_RETURN, 0, 0, 0);
		keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);





		for (UINT i = 0; i < file_list.size();  i++)
		{
			// ���ý���
			// 			HWND MainTopWnd_id = FindWindow("MainTop_TpfWindow", "");
			// 			if (MainTopWnd_id != NULL)
			// 			{
			//				RECT maintop_wnd_rect;
			//				GetWindowRect(MainTopWnd_id, &maintop_wnd_rect);

			int x = exe_wnd_rect.left + 80, y = exe_wnd_rect.top + 120;


			// ���ڻ�ȡ����

			SetCursorPos(x, y);
			mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);




			//			}




			string file_path = file_list.at(i);
			string str_pos_x = x_pos_list.at(i);
			string str_pos_y = y_pos_list.at(i);



			// ȡͼƬ�ļ����� Ctrl+I
			keybd_event(VK_CONTROL, 0, 0, 0);				// ����ctrl
			Sleep(SLEEP_10MS);
			keybd_event('I', 0, 0, 0);						// ����I
			Sleep(SLEEP_10MS);
			keybd_event('I', 0, KEYEVENTF_KEYUP, 0);		// ̧��ctrl
			Sleep(SLEEP_10MS);
			keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	// ̧��IC:\Users\admin\Desktop\tif����ͼƬ\001.tif


			Sleep(SLEEP_1000MS);






			int  find_count = 0;
			HWND file_dlg_id;/* = ::FindWindow("#32770", "ȡͼƬ�ļ�");*/

			while(!(file_dlg_id = ::FindWindow("#32770", "ȡͼƬ�ļ�")))
			{
				Sleep(SLEEP_1000MS);
				find_count++;
			}


			if (file_dlg_id != NULL)
			{
				RECT file_dlg_rect;
				::GetWindowRect(file_dlg_id, &file_dlg_rect);

				int file_path_x = file_dlg_rect.left + 150;
				int file_path_y = file_dlg_rect.bottom - 65;

				SetCursorPos(file_path_x, file_path_y);
				mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
				mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

				// ɾ����������
				keybd_event(VK_BACK, 0, 0, 0);
				keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);




				// ���õ����а�
				CopyToClipboard(file_path.c_str(), file_path.length());

				// ճ�� Ctrl+V
				keybd_event(VK_CONTROL, 0, 0, 0);				// ����ctrl
				keybd_event('V', 0, 0, 0);						// ����D:\QQPCmgr\Desktop\tif����ͼƬ\test_image.tif
				keybd_event('V', 0, KEYEVENTF_KEYUP, 0);		// ̧��ctrl
				keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	// ̧��v




				// ����-ȷ�� 

				keybd_event(VK_RETURN, 0, 0, 0);
				keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);


				// ����-ȷ�� 
				keybd_event(VK_RETURN, 0, 0, 0);
				keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);




			}
			else
			{
				cout << "�ļ�����δ�ҵ���" << std::endl;
				return 0;
			}

			// ��ʾ��ע
			if (show_coor_flag == false)
			{
				int show_coord_btn_x = exe_wnd_rect.left + 610;
				int show_coord_btn_y = exe_wnd_rect.top + 70;
				SetCursorPos(show_coord_btn_x, show_coord_btn_y);
				mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);

				show_coor_flag = true;
			}


			// �޸ı�ע


			if (insert_flag == false)
			{

				Sleep(SLEEP_2000MS);
				insert_flag = true;
			}




			HWND parent_dlg_id = ::FindWindowEx(exe_id, 0,"OGL_V30_Window", "");

			HWND coor_dlg_id = ::FindWindowEx(parent_dlg_id, 0,"#32770", "");
			if (coor_dlg_id != NULL)
			{
				RECT coor_dlg_rect;
				::GetWindowRect(coor_dlg_id, &coor_dlg_rect);



				// ����x����
				POINT coor_x;
				coor_x.x = coor_dlg_rect.left + 35;
				coor_x.y = coor_dlg_rect.top + 8;

				SetCursorPos(coor_x.x, coor_x.y);
				mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
				mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);


				// ����
				InputNormalString(str_pos_x);


				// ����y����
				// 			POINT coor_y;
				// 			coor_y.x = coor_dlg_rect.left + 35;
				// 			coor_y.y = coor_dlg_rect.top + 25;
				// 
				// 			SetCursorPos(coor_y.x, coor_y.y);
				// 			mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
				// 			mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
				//	
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
				cout << "���괰��δ�ҵ���" << endl;
				return 0;
			}
		}
	}
	else
	{
		cout << "exe����δ�ҵ���" << endl;
		return 0;
	}









	

	system("pause");

	return 0;
}
