// WaiGua.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#include <WinUser.h>
#include <stdio.h>
#include <stdlib.h>
#include <atltypes.h>
#include <string>



using namespace std;

#pragma comment (lib, "User32.lib")



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


void  InputNormalString(string str)
{
	for(int i = 0; i < str.length(); i++)
	{
		char c = str.at(i);
		keybd_event(c, 0, 0, 0);						// ����
		keybd_event(c, 0, KEYEVENTF_KEYUP, 0);		// ̧��
	}
}







int _tmain(int argc, _TCHAR* argv[])
{
// 	for (int x = 100; x < 1000; x++)
// 	{
// 		for (int y = 100; y < 1000; y++)
// 		{
// // 			SetCursorPos(x, y);
// // 			Sleep(1000);
// 		}
// 	}

	POINT cur_pt;

	//HWND id = FindWindow(NULL, "��̩��ɫ���ӳ���ϵͳ V6.0(רҵ��)");
	HWND exe_id = FindWindow(NULL, "��̩��ɫ���ӳ���ϵͳ V6.0(�ռ���)");

	if (exe_id != NULL)
	{
		bool show_coor_flag =false;
		RECT exe_wnd_rect;
		GetWindowRect(exe_id, &exe_wnd_rect);

		int x = exe_wnd_rect.left, y = exe_wnd_rect.top;


// 		GetCursorPos(&cur_pt);
// 		SetCursorPos(x+30, y+40);
// 		
// 		Sleep(1000);
		

		//�ļ�
// 		SetCursorPos(x+30, y+40);
// 		mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
// 		Sleep(1000);
// 		
// 		// �½�
// 		SetCursorPos(x+50, y+60);
// 		mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
// 		Sleep(1000);
// 
// 		// ����-ȷ��
// 		keybd_event(VK_RETURN, 0, 0, 0);
// 		keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);

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


		// ȡͼƬ�ļ����� Ctrl+I
		keybd_event(VK_CONTROL, 0, 0, 0);				// ����ctrl
		keybd_event('I', 0, 0, 0);						// ����I
		keybd_event('I', 0, KEYEVENTF_KEYUP, 0);		// ̧��ctrl
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	// ̧��I

		Sleep(500);

		// ��д�ļ���
		string file_path ="D:\\QQPCmgr\\Desktop\\tif����ͼƬ\\test_image.tif";
		HWND file_dlg_id = FindWindow("#32770", "ȡͼƬ�ļ�");
		if (file_dlg_id != NULL)
		{
			RECT file_dlg_rect;
			GetWindowRect(file_dlg_id, &file_dlg_rect);
				
			int file_path_x = file_dlg_rect.left + 150;
			int file_path_y = file_dlg_rect.top + 200;
			
			SetCursorPos(file_path_x, file_path_y);
			mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);


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

		Sleep(500);
		HWND parent_dlg_id = FindWindowEx(exe_id, 0,"OGL_V30_Window", "");

		HWND coor_dlg_id = FindWindowEx(parent_dlg_id, 0,"#32770", "");
		if (coor_dlg_id != NULL)
		{
			RECT coor_dlg_rect;
			GetWindowRect(coor_dlg_id, &coor_dlg_rect);
			

			string str_pos_x ="100";
			string str_pos_y ="200";

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

	}
	

	system("pause");

	return 0;
}

