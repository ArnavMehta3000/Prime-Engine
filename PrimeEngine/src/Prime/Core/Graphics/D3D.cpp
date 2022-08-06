#include "pch.h"
#include "D3D.h"

namespace Prime
{
	D3D::D3D()
	{
		m_videoCardMemory = 0;
	}

	D3D::~D3D()
	{
	}

	bool D3D::Init(D3D_INIT_PARAMS p)
	{
		// Create DirectX graphics interface factory
		IDXGIFactory* factory;
		ThrowHr(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory), "Failed to create IDXGI Factory");
		LOG("Successfully created IDXGI Factory");

		// Create adapter for primary GPU using factory
		IDXGIAdapter* adapter;
		ThrowHr(factory->EnumAdapters(0, &adapter), "Failed to create IDXGI Adapter");
		LOG("Successfully created IDXGI Adapter");

		// Enumerate primary adapter output (monitor)
		IDXGIOutput* adapterOutput;
		ThrowHr(adapter->EnumOutputs(0, &adapterOutput), "Failed to create IDXGI Output");
		LOG("Successfully created IDXGI Output");

		// Get number of modes that match the given display format
		UINT numModes = 0;
		DXGI_MODE_DESC* displayModeList;
		ThrowHr(adapterOutput->GetDisplayModeList(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_ENUM_MODES_INTERLACED,
			&numModes,
			NULL), "Failed to get number of display modes");

		displayModeList = new DXGI_MODE_DESC[numModes];

		ThrowHr(adapterOutput->GetDisplayModeList(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_ENUM_MODES_INTERLACED,
			&numModes,
			displayModeList), "Failed to get display mode list");

		// Create a list of all the possible display modes fo this monitor GPU combination
		if (!displayModeList)
		{
			FATAL("Failed to get display mode list");
		}
		else
		{
			LOG("Successfully retrived display mode list");
			TRACE("Mode list count: " + std::to_string(numModes))
		}
			
		// Cycle through all display modes in list, find one that matches with screen dimenaions
		// When match is found store the numerator and denominator of the refresh rate of the monitor
		UINT numerator = 0, denominator = 1;
		UINT num = 0;
		for (UINT i = 0; i < numModes; i++)
		{

			if (displayModeList[i].Width == (unsigned int)p.Window.Width)
			{
				if (displayModeList[i].Height == (unsigned int)p.Window.Height)
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
					num = i;
				}
			}
		}
		TRACE("Monitor Width: " + std::to_string(displayModeList[num].Width));
		TRACE("Monitor Height: " + std::to_string(displayModeList[num].Height));
		TRACE("Refresh rate numerator: " + std::to_string(numerator));
		TRACE("Refresh rate denominator: " + std::to_string(denominator));

		// Get adapter description
		DXGI_ADAPTER_DESC adapterDesc;
		ThrowHr(adapter->GetDesc(&adapterDesc), "Failed to get adapter description");
		LOG("Successfully retrived adapter descripton");
		
		// Store video card memory in mb
		m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
		TRACE("Dedicated video card memory: " + std::to_string(m_videoCardMemory) + "MB");


		delete[] displayModeList;
		displayModeList = nullptr;

		adapterOutput->Release();
		adapter->Release();
		factory->Release();




		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapChainDesc.BufferCount = p.BackBufferCount;



		return true;
	}

	void D3D::Shutdown()
	{
		m_device->Release();
		m_context->Release();
		m_swapChain->Release();
		m_renderTargetView->Release();
	}

	void D3D::ThrowHr(HRESULT hr, const char* msg)
	{
		if (FAILED(hr))
			FATAL(msg)
	}
}
