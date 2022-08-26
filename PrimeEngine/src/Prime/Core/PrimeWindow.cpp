#include "pch.h"
#include "PrimeWindow.h"
#include "Prime/Constants.h"
#include "Prime/Core/Graphics/ResizeHandler.h"

namespace Prime
{
	PrimeWindow::PrimeWindow()
		: m_hInstance(GetModuleHandle(nullptr))
	{
		WNDCLASSEX wc    = { 0 };
		wc.cbSize        = sizeof(WNDCLASSEX);
		wc.style         = CS_OWNDC;
		wc.lpfnWndProc   = MessageRouter;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = m_hInstance;
		wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = nullptr;
		wc.lpszMenuName  = nullptr;
		wc.lpszClassName = CLASS_NAME;

		RegisterClassEx(&wc);

		DWORD style = WS_OVERLAPPEDWINDOW;

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
			this
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

	LRESULT CALLBACK PrimeWindow::MessageRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Prime::PrimeWindow* window;

		if (msg == WM_CREATE)
		{
			window = (Prime::PrimeWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
		}
		else
		{
			window = (Prime::PrimeWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		}

		return window->PrimeWndProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK PrimeWindow::PrimeWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;

		case WM_SIZE:
			ResizeHandler::Resize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}