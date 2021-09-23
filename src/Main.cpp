#include "stdafx.h"
#include <iostream>
#include "Game.h"

HWND m_hwnd;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	Game* myGame = reinterpret_cast<Game*>(GetWindowLongPtr(m_hwnd, GWLP_USERDATA));
	
	switch (message)
	{
	case WM_CREATE:
		{
			
		}
		return 0;
	case WM_PAINT:
		if (myGame) {
			myGame->OnUpdate();
			myGame->OnRender();
		}
		return 0;

	case WM_KEYDOWN:
		{
		if (wParam == 0x54) {
			//wchar_t msg[100];
			
			
			//swprintf_s(msg, L"%d", formatID.Data4[7]);
			//SetWindowTextW(m_hwnd, msg);
			//OutputDebugStringW(msg);
		}
		}
		return 0;

	case WM_DESTROY:				
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = 0;
	windowClass.lpfnWndProc = MainWndProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = L"LiGameFramework";
	RegisterClassEx(&windowClass);

	m_hwnd = CreateWindow(windowClass.lpszClassName,
		L"LiGameFramework", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);
	

	Game* myGame = new Game(1280, 760, L"LiGF", m_hwnd);
	myGame->OnInit();

	
	/*wchar_t filename[10];
	swprintf_s(filename, L"%c.png", ch);
	myGame->m_imageMgr->SaveBufferAsPng(fbb, filename);*/

	SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(myGame));
	ShowWindow(m_hwnd, nCmdShow);

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	myGame->OnDestroy();
	return static_cast<char>(msg.wParam);
}