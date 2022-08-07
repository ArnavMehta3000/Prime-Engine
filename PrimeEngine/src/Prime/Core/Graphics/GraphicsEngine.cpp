#include "pch.h"
#include "GraphicsEngine.h"

namespace Prime
{
	GraphicsEngine::GraphicsEngine()
	{
		m_d3d = std::make_unique<D3D>();
	}
	GraphicsEngine::~GraphicsEngine()
	{
	}



	void GraphicsEngine::Init(D3D_INIT_PARAMS d3dInit)
	{
		m_d3d->Init(d3dInit);

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
		dsDesc.StencilEnable                = true;
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
		LOG("Successfully created depth stencil state")

		// Bind depth stencil state
		m_d3d->m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
		TRACE("Bound depth stencil state");

		// Create depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format             = dbDesc.Format;
		dsvDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;

		// Create the depth stencil view
		D3D::ThrowHr(m_d3d->m_device->CreateDepthStencilView(m_depthBufferTexture, &dsvDesc, m_depthStencilView.GetAddressOf()),
			"Failed to create depth stencil view");
		LOG("Successfully created depth stencil view");
		m_depthBufferTexture->Release();

		// Set render targets
		m_d3d->m_context->OMSetRenderTargets(1, m_d3d->m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
		TRACE("Bound depth stencil view and render target");


		// Create solid and wireframe raster states
		D3D11_RASTERIZER_DESC solidRasterDesc{};
		solidRasterDesc.FillMode              = D3D11_FILL_SOLID;
		solidRasterDesc.CullMode              = D3D11_CULL_BACK;
		solidRasterDesc.FrontCounterClockwise = false;
		solidRasterDesc.DepthBias             = 0;
		solidRasterDesc.DepthBiasClamp        = 0.0f;
		solidRasterDesc.DepthClipEnable       = true;
		solidRasterDesc.MultisampleEnable     = false;
		solidRasterDesc.ScissorEnable         = false;
		solidRasterDesc.SlopeScaledDepthBias  = 0.0f;

		D3D11_RASTERIZER_DESC wireRasterDesc{};
		wireRasterDesc.FillMode              = D3D11_FILL_SOLID;
		wireRasterDesc.CullMode              = D3D11_CULL_BACK;
		wireRasterDesc.FrontCounterClockwise = false;
		wireRasterDesc.DepthBias             = 0;
		wireRasterDesc.DepthBiasClamp        = 0.0f;
		wireRasterDesc.DepthClipEnable       = true;
		wireRasterDesc.MultisampleEnable     = false;
		wireRasterDesc.ScissorEnable         = false;
		wireRasterDesc.SlopeScaledDepthBias  = 0.0f;

		D3D::ThrowHr(m_d3d->m_device->CreateRasterizerState(&solidRasterDesc, m_rasterStateSolid.GetAddressOf()),
			"Failed to create solid rasterizer state");

		D3D::ThrowHr(m_d3d->m_device->CreateRasterizerState(&solidRasterDesc, m_rasterStateSolid.GetAddressOf()),
			"Failed to create solid rasterizer state");
		
		SetWireframe(false);

		// Setup the viewport for rendering.
		D3D11_VIEWPORT viewport{};
		viewport.Width    = static_cast<float>(d3dInit.Window.Width);
		viewport.Height   = static_cast<float>(d3dInit.Window.Height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		// Create the viewport.
		m_d3d->m_context->RSSetViewports(1, &viewport);
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
	

}
