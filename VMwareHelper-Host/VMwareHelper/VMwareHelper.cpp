#include "stdafx.h"
#include "VMwareHelper.h"
#include "VirtualDesktopManager.h"
#include <process.h>

HWND gWndHandle;
VirtualDesktopManager gVDManager;
bool gIsTerminate = false;
HANDLE gThreadHandle = NULL;
SOCKET gServerSocket;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	MSG message;
	WNDCLASS wndclass;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon =  LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance =  hInstance;
	wndclass.lpfnWndProc = (WNDPROC)WndProc;
	wndclass.lpszClassName =  L"VMwareHelper";
	wndclass.lpszMenuName = NULL;
	wndclass.style = NULL;

	RegisterClass(&wndclass);

	gWndHandle = CreateWindow(L"VMwareHelper", L"VMwareHelper", WS_OVERLAPPEDWINDOW, 200, 200, 500, 500, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(gWndHandle, SW_NORMAL);

	while(GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		case WM_CREATE:
			if (CreateFunction() == false)
			{
				PostQuitMessage(0);
			}
			return 0;

		case WM_DESTROY:
			DestroyFunction();
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc( hWnd, iMessage, wParam, lParam );
}

bool CreateFunction()
{
	bool result = false;

	if (__argc == 3)
	{
		if (SUCCEEDED(gVDManager.Init()))
		{
			if (CreateServerSocket())
			{
				DWORD threadID;
				gThreadHandle = (HANDLE)_beginthreadex(NULL, 0, ServerThread, NULL, 0, (unsigned*)&threadID);
				if (gThreadHandle != 0)
				{
					result = true;
				}
				else
				{
					MessageBox(gWndHandle, L"_beginthreadex fail!", L"error", MB_ICONERROR);
					closesocket(gServerSocket);
					gVDManager.Uninit();
					PostQuitMessage(0);
				}
			}
			else
			{
				MessageBox(gWndHandle, L"CreateServerSocket fail!", L"error", MB_ICONERROR);
				gVDManager.Uninit();
				PostQuitMessage(0);
			}
		}
		else
		{
			MessageBox(gWndHandle, L"gVDManager.Init() fail!", L"error", MB_ICONERROR);
			PostQuitMessage(0);
		}
	}
	else
	{
		MessageBox(gWndHandle, L"argument check fail!", L"error", MB_ICONERROR);
		PostQuitMessage(0);
	}

	return result;
}

void DestroyFunction()
{
	gIsTerminate = true;
	closesocket(gServerSocket);
	WaitForSingleObject(gThreadHandle, INFINITE);
	gVDManager.Uninit();
}

bool CreateServerSocket()
{
	bool result = false;
	WSADATA wsaData;
	SOCKADDR_IN serverAddr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0)
	{
		gServerSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (gServerSocket != INVALID_SOCKET)
		{
			memset(&serverAddr, 0, sizeof(serverAddr));
			serverAddr.sin_family = AF_INET;
			inet_pton(AF_INET, __argv[1], &serverAddr.sin_addr);
			serverAddr.sin_port = htons(atoi(__argv[2]));

			if (bind(gServerSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) != SOCKET_ERROR)
			{
				if (listen(gServerSocket, 5) != SOCKET_ERROR)
				{
					result = true;
				}
				else
				{
					MessageBox(gWndHandle, L"listen fail!", L"error", MB_ICONERROR);
				}
			}
			else
			{
				MessageBox(gWndHandle, L"bind fail!", L"error", MB_ICONERROR);
			}
		}
		else
		{
			MessageBox(gWndHandle, L"make server socket fail!", L"error", MB_ICONERROR);
		}
	}
	else
	{
		MessageBox(gWndHandle, L"WSAStartup fail!", L"error", MB_ICONERROR);
	}

	return result;
}

unsigned WINAPI ServerThread(void* parameter)
{
	SOCKET clientSocket;
	SOCKADDR_IN clientAddr;
	int clientAddrSize;
	clientAddrSize = sizeof(clientAddr);
	char cmd;

	Sleep(1);
	ShowWindow(gWndHandle, SW_HIDE);

	while (gIsTerminate == false)
	{
		clientSocket = accept(gServerSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);
		if (recv(clientSocket, &cmd, 1, NULL) == 1)
		{
			switch (cmd)
			{
			case 'L':
				gVDManager.SwitchLeft();
				break;
			case 'R':
				gVDManager.SwitchRight();
				break;
			case 'U':
				keybd_event(VK_LCONTROL, 0, KEYEVENTF_EXTENDEDKEY, 0);
				keybd_event(VK_LMENU,    0, KEYEVENTF_EXTENDEDKEY, 0);
				keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(VK_LMENU ,   0, KEYEVENTF_KEYUP, 0);
			default:
				break;
			}
		}
	}

	return 0;
}