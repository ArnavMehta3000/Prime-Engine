#include "pch.h"
#include "GraphicsEngine.h"
#include "Prime/Core/ServiceLocator/Locator.h"
#include "Prime/Core/Graphics/ResizeHandler.h"

namespace Prime
{
	GraphicsEngine::GraphicsEngine()
	{
		m_d3d = std::make_unique<D3D>();
	}
	GraphicsEngine::~GraphicsEngine()
	{
	}



	void GraphicsEngine::Init(D3D_INIT_PARAMS d3dInit, bool isResize)
	{
		if (!isResize)
		{
			m_initParams = d3dInit;
			m_d3d->Init(d3dInit);
			Locator::RegisterService<GraphicsFactory>();
			Locator::RegisterService<GraphicsRenderer>();
			Locator::ResolveService<GraphicsFactory>()->Init(m_d3d->m_device.Get(), m_d3d->m_context.Get());
			Locator::ResolveService<GraphicsRenderer>()->Init(m_d3d->m_device.Get(), m_d3d->m_context.Get());

			ResizeHandler::RegisterFunction(PRIME_BIND_RESIZE_FN(GraphicsEngine::OnResize));
		}



		// Create depth buffer
		D3D11_TEXTURE2D_DESC dbDesc{};
		dbDesc.Width              = d3dInit.Window.Width;
		dbDesc.Height             = d3dInit.Window.Height;
		dbDesc.MipLevels          = 1;
		dbDesc.ArraySize          = 1;
		dbDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dbDesc.SampleDesc.Count   = d3dInit.Multisampling.Count;
		dbDesc.SampleDesc.Quality = d3dInit.Multisampling.Quality;
		dbDesc.Usage              = D3D11_USAGE_DEFAULT;
		dbDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
		dbDesc.CPUAccessFlags     = 0;
		dbDesc.MiscFlags          = 0;

		ID3D11Texture2D* m_depthBufferTexture;
		D3D::ThrowHr(m_d3d->m_device->CreateTexture2D(&dbDesc, NULL, &m_depthBufferTexture),
			"Failed to create depth buffer");


		// Create depth stencil state
		D3D11_DEPTH_STENCIL_DESC dsDesc{};
		dsDesc.DepthEnable                  = true;
		dsDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc                    = D3D11_COMPARISON_LESS;
		dsDesc.StencilReadMask              = 0xFF;
		dsDesc.StencilWriteMask             = 0xFF;
		dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_DECR;
		dsDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;
		D3D::ThrowHr(m_d3d->m_device->CreateDepthStencilState(&dsDesc, m_depthStencilState.GetAddressOf()),
			"Failed to create depth stencil state");

		// Bind depth stencil state
		m_d3d->m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

		// Create depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format             = dbDesc.Format;
		dsvDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;

		// Create the depth stencil view
		D3D::ThrowHr(m_d3d->m_device->CreateDepthStencilView(m_depthBufferTexture, &dsvDesc, m_depthStencilView.GetAddressOf()),
			"Failed to create depth stencil view");
		m_depthBufferTexture->Release();

		// Set render targets
		m_d3d->m_context->OMSetRenderTargets(1, m_d3d->m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());


		// Create solid and wireframe raster states
		D3D11_RASTERIZER_DESC solidRasterDesc{};
		ZeroMemory(&solidRasterDesc, sizeof(D3D11_RASTERIZER_DESC));
		solidRasterDesc.FillMode              = D3D11_FILL_SOLID;
		solidRasterDesc.CullMode              = D3D11_CULL_BACK;

		D3D11_RASTERIZER_DESC wireRasterDesc{};
		ZeroMemory(&wireRasterDesc, sizeof(D3D11_RASTERIZER_DESC));
		wireRasterDesc.FillMode              = D3D11_FILL_WIREFRAME;
		wireRasterDesc.CullMode              = D3D11_CULL_BACK;

		D3D::ThrowHr(m_d3d->m_device->CreateRasterizerState(&solidRasterDesc, m_rasterStateSolid.GetAddressOf()),
			"Failed to create solid rasterizer state");

		D3D::ThrowHr(m_d3d->m_device->CreateRasterizerState(&wireRasterDesc, m_rasterStateWireframe.GetAddressOf()),
			"Failed to create wireframe rasterizer state");

		if (!isResize)
			SetWireframe(false);
	}

	void GraphicsEngine::Shutdown()
	{
		m_d3d->Shutdown();
		m_d3d.release();
	}

	void GraphicsEngine::BeginFrame()
	{
		float color[4] = { 0.15f, 0.175f, 0.25f, 1.0f };
		m_d3d->m_context->ClearRenderTargetView(m_d3d->m_renderTargetView.Get(), color);
		m_d3d->m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);		
	}

	void GraphicsEngine::EndFrame()
	{
		if (m_d3d->m_initParams.VSync)
			m_d3d->m_swapChain->Present(1, 0);  // Lock present to refresh rate
		else
			m_d3d->m_swapChain->Present(0, 0);  // Present as fast as possible
	}

	void GraphicsEngine::SetWireframe(bool isWireframe)
	{
		if (isWireframe)
		{
			m_d3d->m_context->RSSetState(m_rasterStateWireframe.Get());
			TRACE("Set raster state: wireframe");
		}
		else
		{
			m_d3d->m_context->RSSetState(m_rasterStateSolid.Get());
			TRACE("Set raster state: solid");
		}
	}

	void GraphicsEngine::OnResize(int w, int h)
	{
		m_d3d->m_context->OMSetRenderTargets(0, 0, 0);
		m_d3d->m_renderTargetView->Release();

		HRESULT hr;
		hr = m_d3d->m_swapChain->ResizeBuffers(1, w, h, DXGI_FORMAT_UNKNOWN, 0);
		THROW_HR(hr, "Failed to resize swap chain");

		ID3D11Texture2D* pBuffer;
		hr = m_d3d->m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
		THROW_HR(hr, "Failed to get back buffer");

		hr = m_d3d->m_device->CreateRenderTargetView(pBuffer, NULL, &m_d3d->m_renderTargetView);
		THROW_HR(hr, "Failed to resize and create render target view");

		pBuffer->Release();

		// Set up the viewport.
		D3D11_VIEWPORT vp{};
		vp.Width = (float)w;
		vp.Height = (float)h;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		m_d3d->m_context->RSSetViewports(1, &vp);
		Init(m_initParams, true);
	}
}
