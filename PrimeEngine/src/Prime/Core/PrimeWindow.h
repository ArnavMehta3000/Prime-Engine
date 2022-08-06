#pragma once

namespace Prime
{
	class PrimeWindow
	{
	public:
		PrimeWindow();
		PrimeWindow(const PrimeWindow&) = delete;
		PrimeWindow& operator=(const PrimeWindow&) = delete;
		~PrimeWindow();

		bool ProcessMessages();
		
		inline void SetTitle(LPCWSTR title) { SetWindowText(m_hWnd, title); }
		inline HWND GetHWND() { return m_hWnd; }

		
	private:
		const LPCWSTR CLASS_NAME = L"Prime Class";

		HINSTANCE m_hInstance;
		HWND m_hWnd;
	};
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);