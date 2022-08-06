#pragma once
#include "Prime/ApiDefines.h"

namespace Prime
{
	struct D3D_INIT_PARAMS
	{

		struct
		{
			HWND Handle;
			UINT Width;
			UINT Height;
		}Window;

		struct
		{
			UINT Count = 1;
			UINT Quality = 0;
		}Multisampling;

		bool VSync = true;
		bool Fullscreen = false;

		UINT BackBufferCount = 1;
		DXGI_FORMAT SCBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	};

	class D3D
	{
	public:
		D3D();
		~D3D();

		PRIME_API bool Init(D3D_INIT_PARAMS p);
		PRIME_API void Shutdown();

		//friend GraphicsEngine;

	private:
		PRIME_API void ThrowHr(HRESULT hr, const char* msg);

	private:
		int m_videoCardMemory;

		ComPtr<ID3D11Device>           m_device;
		ComPtr<ID3D11DeviceContext>    m_context;
		ComPtr<IDXGISwapChain>         m_swapChain;
		ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	};
}

