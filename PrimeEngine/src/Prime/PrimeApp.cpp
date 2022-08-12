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

		
		// Create vertex buffer
		const Vertex vertices[] =
		{
			{ 0.0f, 0.5f, 0.0f },
			{ 0.5f, -0.5f, 0.0f },
			{ -0.5f, -0.5f, 0.0f },
		};
		m_vertexBuffer.reset(
			factory->CreateVertexBuffer(vertices, UINT(sizeof(Vertex)), ARRAYSIZE(vertices)));
		UINT offset = 0u;
		gfx->GetContext()->IASetVertexBuffers(0u, 1u, m_vertexBuffer->GetAddressOf(), m_vertexBuffer->StridePtr(), &offset);


		// Create index buffer
		DWORD indices[] =
		{
			1,2,3
		};
		m_indexBuffer.reset(
			factory->CreateIndexBuffer(indices, ARRAYSIZE(indices)));
		gfx->GetContext()->IASetIndexBuffer(m_indexBuffer->Get(), DXGI_FORMAT_R32_UINT, 0);

		// Create vertex shader
		D3D11_INPUT_ELEMENT_DESC inputLayout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};		
		m_vertexShader.reset(
			factory->CreateVertexShader((SHADER_PATH + L"DefaultVertex.cso").c_str(), inputLayout, ARRAYSIZE(inputLayout)));

		m_pixelShader.reset(
			factory->CreatePixelShader((SHADER_PATH + L"DefaultPixel.cso").c_str()));

		gfx->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		gfx->GetContext()->IASetInputLayout(m_vertexShader->GetInputLayout().Get());



		

		


		while (m_window->ProcessMessages())
		{
			gfx->BeginFrame();
			// Input polling here
			OnUpdate(0.0f);
			OnRender(0.0f);

			gfx->GetContext()->VSSetShader(m_vertexShader->GetShader().Get(), nullptr, 0u);
			gfx->GetContext()->PSSetShader(m_pixelShader->GetShader().Get(), nullptr, 0u);
			//gfx->GetContext()->DrawIndexed(m_indexBuffer->GetCount(), 0u, 0u);
			gfx->GetContext()->Draw(3, 0);
			
			
			gfx->EndFrame();
			Sleep(0);
		}

		ShutDown();
	}
}
