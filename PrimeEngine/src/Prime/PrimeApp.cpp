#include "pch.h"
#include "PrimeApp.h"
#include "Prime/Logger.h"
#include "Prime/Constants.h"
#include "Effects.h"
#include <DirectXHelpers.h>

Prime::GraphicsEngine* Prime::GetGraphicsEngine()
{
	return Locator::ResolveService<GraphicsEngine>();
}

namespace Prime
{
	PrimeApp::PrimeApp()
	{
		m_window = nullptr;
		CreateAndAttachConsole();		
	}
	
	PrimeApp::~PrimeApp()
	{
	}

	void PrimeApp::ShutDown()
	{
		Locator::ResolveService<GraphicsEngine>()->Shutdown();

		if (m_window)
		{
			delete m_window;
			m_window = nullptr;
		}
	}

	void PrimeApp::CreateAndAttachConsole()
	{
#ifdef _DEBUG
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		FILE* fp = nullptr;
		freopen_s(&fp, "CONIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);

		LOG("Successfully attached console");
#endif // _DEBUG
	}
	
	void Prime::PrimeApp::PreRunInit()
	{
		LOG_LOAD("Initializing app");
		
		m_window = new PrimeWindow();
		Locator::RegisterService<GraphicsEngine>();

		D3D_INIT_PARAMS d3dInit{};
		d3dInit.Window.Width  = WINDOW_WIDTH;
		d3dInit.Window.Height = WINDOW_HEIGHT;
		d3dInit.Window.Handle = m_window->GetHWND();
		d3dInit.VSync         = false;

		auto gfx = Locator::ResolveService<GraphicsEngine>();
		gfx->Init(d3dInit);
		
		LOG_LOAD("Pre-run Initialise complete");
	}

	void PrimeApp::Run()
	{
		PreRunInit();
		OnStart();
		auto gfx = Locator::ResolveService<GraphicsEngine>();
		auto factory = Locator::ResolveService<GraphicsFactory>();

		

		const Vertex vertices[] =
		{
			{ 0.0f, 0.5f, 0.0f },
			{ 0.5f, -0.5f, 0.0f },
			{ -0.5f, -0.5f, 0.0f },
		};

		DataBufferDesc vBufferDesc{};
		vBufferDesc.Usage     = DataBufferUsage::Normal;
		vBufferDesc.CPUAccess = DataBufferCPUAccess::None;
		vBufferDesc.Type      = DataBufferType::VertexBuffer;
		factory->CreateBuffer(vBufferDesc, vertices, UINT(sizeof(vertices)), UINT(sizeof(Vertex)), m_vertexBuffer);
		gfx->GetContext()->IASetVertexBuffers(0u, 1u, m_vertexBuffer->GetCOM().GetAddressOf(), m_vertexBuffer->GetStride(), m_vertexBuffer->GetOffset());


		UINT indices[] =
		{
			1,2,3
		};

		DataBufferDesc iBufferDesc{};
		iBufferDesc.Type = DataBufferType::IndexBuffer;
		iBufferDesc.CPUAccess = DataBufferCPUAccess::None;
		iBufferDesc.Usage = DataBufferUsage::Normal;
		factory->CreateBuffer(iBufferDesc, indices, UINT(sizeof(UINT) * ARRAYSIZE(indices)), UINT(sizeof(UINT)), m_indexBuffer);
		gfx->GetContext()->IASetIndexBuffer(m_indexBuffer->GetCOM().Get(), DXGI_FORMAT_R32_UINT, 0);


		ComPtr<ID3D10Blob> vertexBlob;
		THROW_HR(D3DReadFileToBlob((SHADER_PATH + L"DefaultVertex.cso").c_str(), vertexBlob.GetAddressOf()),
			"Failed to read vertex shader to blob");
		THROW_HR(gfx->GetDevice()->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), nullptr, &m_vertexShader),
			"Failed to create vertex shader object");
		TRACE("Loaded and created vertex shader");


		ComPtr<ID3D10Blob> pixelBlob;
		THROW_HR(D3DReadFileToBlob((SHADER_PATH + L"DefaultPixel.cso").c_str(), pixelBlob.GetAddressOf()),
			"Failed to read pixel shader to blob");
		THROW_HR(gfx->GetDevice()->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), nullptr, &m_pixelShader),
			"Failed to create pixel shader object");
		TRACE("Loaded and created pixel shader");

		gfx->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		const D3D11_INPUT_ELEMENT_DESC inputLayout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		
		THROW_HR(gfx->GetDevice()->CreateInputLayout(inputLayout,
													 int(std::size(inputLayout)),
													 vertexBlob->GetBufferPointer(),
													 vertexBlob->GetBufferSize(),
													 m_inputLayout.GetAddressOf()),
			"Failed to create input layout");

		gfx->GetContext()->IASetInputLayout(m_inputLayout.Get());



		

		


		while (m_window->ProcessMessages())
		{
			gfx->BeginFrame();
			// Input polling here
			OnUpdate(0.0f);
			OnRender(0.0f);

			gfx->GetContext()->VSSetShader(m_vertexShader.Get(), nullptr, 0u);
			gfx->GetContext()->PSSetShader(m_pixelShader.Get(), nullptr, 0u);
			//gfx->GetContext()->DrawIndexed(3u, 0u, 0u);
			gfx->GetContext()->Draw(3, 0);
			
			
			gfx->EndFrame();
			Sleep(0);
		}

		ShutDown();
	}
}
