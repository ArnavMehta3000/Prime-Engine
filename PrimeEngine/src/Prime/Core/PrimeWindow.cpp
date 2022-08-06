#include "pch.h"
#include "PrimeWindow.h"
#include "Prime/Constants.h"

namespace Prime
{
	PrimeWindow::PrimeWindow()
		: m_hInstance(GetModuleHandle(nullptr))
	{
		WNDCLASSEX wc    = { 0 };
		wc.cbSize        = sizeof(WNDCLASSEX);
		wc.style         = CS_OWNDC;
		wc.lpfnWndProc   = WindowProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = m_hInstance;
		wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = nullptr;
		wc.lpszMenuName  = nullptr;
		wc.lpszClassName = CLASS_NAME;

		RegisterClassEx(&wc);

		DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

		RECT rect{};
		rect.left = 0;
		rect.top = 0;
		rect.right = rect.left + WINDOW_WIDTH;
		rect.bottom = rect.top + WINDOW_HEIGHT;
		AdjustWindowRect(&rect, style, FALSE);
		
		m_hWnd = CreateWindowEx(
			0,
			CLASS_NAME,
			_T("Prime App"),
			style,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rect.right - rect.left,
			rect.bottom - rect.top,
			NULL,
			NULL,
			m_hInstance,
			NULL
		);

		if (!m_hWnd)
		{
			FATAL_ERROR("Failed to create window");
			return;
		}
		else
		{
			ShowWindow(m_hWnd, SW_SHOW);
			LOG("Successfully created window");
		}
	}

	PrimeWindow::~PrimeWindow()
	{
		UnregisterClass(CLASS_NAME, m_hInstance);
	}
	
	bool PrimeWindow::ProcessMessages()
	{
		MSG msg = {};
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{			
			if (msg.message == WM_QUIT)
				return false;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return true;
	}
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
