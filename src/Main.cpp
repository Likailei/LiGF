#include "stdafx.h"
#include <iostream>
#include "Game.h"

HWND m_hwnd;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

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
	

	Game* pGame = new Game(1280, 760, L"LiGF", m_hwnd);
	pGame->OnInit();
	SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pGame));

	ShowWindow(m_hwnd, nCmdShow);

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	pGame->OnDestroy();
	return static_cast<char>(msg.wParam);
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	Game* pGame = reinterpret_cast<Game*>(GetWindowLongPtr(m_hwnd, GWLP_USERDATA));
	
	switch (message)
	{
	case WM_CREATE:
		{
			
		}
		return 0;
	case WM_INPUT:
		{
		char str[1024];
		pGame->m_inputMgr->ParseInputData(lParam);
		RAWINPUT* raw = pGame->m_inputMgr->GetRawInput();
		switch (raw->header.dwType)
		{
		case RIM_TYPEKEYBOARD:
			sprintf_s(str, " Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n",
				raw->data.keyboard.MakeCode,
				raw->data.keyboard.Flags,
				raw->data.keyboard.Reserved,
				raw->data.keyboard.ExtraInformation,
				raw->data.keyboard.Message,
				raw->data.keyboard.VKey);			
			OutputDebugStringA(str);
			//if(raw->data.keyboard.Flags == RI_KEY_BREAK) 
			pGame->OnKeyUp(raw->data.keyboard.VKey);
			break;
			
		/*case RIM_TYPEMOUSE:
			sprintf_s(str, "Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%d ulRawButtons=%04x lLastX=%ld lLastY=%ld ulExtraInformation=%04x\r\n",
				raw->data.mouse.usFlags,
				raw->data.mouse.ulButtons,
				raw->data.mouse.usButtonFlags,
				(short)raw->data.mouse.usButtonData,
				raw->data.mouse.ulRawButtons,
				(long)raw->data.mouse.lLastX,
				(long)raw->data.mouse.lLastY,
				raw->data.mouse.ulExtraInformation);
			OutputDebugStringA(str);
			break;*/
		default: break;
		}
		}
		return 0;
	case WM_PAINT:
		pGame->OnUpdate();
		pGame->OnRender();
		return 0;

	case WM_DESTROY:				
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}