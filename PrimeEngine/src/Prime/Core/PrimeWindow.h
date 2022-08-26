#pragma once

namespace Prime
{
	class PrimeWindow
	{
	public:
		PrimeWindow(UINT width, UINT height, bool maximise = false);
		PrimeWindow(const PrimeWindow&) = delete;
		PrimeWindow& operator=(const PrimeWindow&) = delete;
		~PrimeWindow();

		bool ProcessMessages();
		
		inline void SetTitle(LPCWSTR title) { SetWindowText(m_hWnd, title); }
		inline HWND GetHWND() { return m_hWnd; }

		static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK PrimeWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		
	private:
		const LPCWSTR CLASS_NAME = L"Prime Class";

		HINSTANCE m_hInstance;
		HWND m_hWnd;

	public:
		static UINT s_clientWidth;
		static UINT s_clientHeight;
	};
}

