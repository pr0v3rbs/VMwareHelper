#pragma once

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

bool CreateFunction();
void DestroyFunction();
bool CreateServerSocket();
unsigned WINAPI ServerThread(void* parameter);