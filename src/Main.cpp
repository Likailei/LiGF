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
	windowClass.lpszClassName = L"LiGF";
	RegisterClassEx(&windowClass);

	m_hwnd = CreateWindow(windowClass.lpszClassName,
		L"LiGF", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
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
		pGame->OnInput(lParam);
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