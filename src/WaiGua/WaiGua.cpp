// WaiGua.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#include <WinUser.h>
#include <stdio.h>
#include <stdlib.h>
#include <atltypes.h>

#pragma comment (lib, "User32.lib")

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
	HWND id = FindWindow(NULL, "��̩��ɫ���ӳ���ϵͳ V6.0(רҵ��)");
	if (id != NULL)
	{
		RECT exe_wnd_rect;
		GetWindowRect(id, &exe_wnd_rect);

		int x = exe_wnd_rect.left, y = exe_wnd_rect.top;


		GetCursorPos(&cur_pt);
		SetCursorPos(x+30, y+40);
		
		Sleep(1000);
		

		//�ļ�
		SetCursorPos(x+30, y+40);
		mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
		Sleep(1000);
		
		// �½�
		SetCursorPos(x+50, y+60);
		mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
		Sleep(1000);

		// ����-ȷ��
		keybd_event(VK_RETURN, 0, 0, 0);
		keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);


		GetCursorPos(&cur_pt);
		GetCursorPos(&cur_pt);
		GetCursorPos(&cur_pt);
		GetCursorPos(&cur_pt);
		GetCursorPos(&cur_pt);
		GetCursorPos(&cur_pt);
		//

	}
	

	
	system("pause");




	system("pause");

	return 0;
}

