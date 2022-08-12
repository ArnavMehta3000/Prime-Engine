#include "pch.h"
#include "PrimeApp.h"
#include "Prime/Logger.h"
#include "Prime/Constants.h"
#include "Effects.h"
#include <DirectXHelpers.h>

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
		g_gfxFactory.reset(Locator::ResolveService<GraphicsFactory>());
		g_gfxrenderer.reset(Locator::ResolveService<GraphicsRenderer>());
		

		
		// Create vertex buffer
		const Vertex vertices[] =
		{
			{ 0.0f, 0.5f, 0.0f },
			{ 0.5f, -0.5f, 0.0f },
			{ -0.5f, -0.5f, 0.0f },
		};
		m_vertexBuffer.reset(g_gfxFactory->CreateVertexBuffer(vertices, UINT(sizeof(Vertex)), ARRAYSIZE(vertices)));

		// Create index buffer
		DWORD indices[] =
		{
			1,2,3
		};
		m_indexBuffer.reset(g_gfxFactory->CreateIndexBuffer(indices, ARRAYSIZE(indices)));

		// Create vertex shader
		D3D11_INPUT_ELEMENT_DESC inputLayout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};		
		
		m_vertexShader.reset(g_gfxFactory->CreateVertexShader((SHADER_PATH + L"DefaultVertex.cso").c_str(), inputLayout, ARRAYSIZE(inputLayout)));
		m_pixelShader.reset(g_gfxFactory->CreatePixelShader((SHADER_PATH + L"DefaultPixel.cso").c_str()));



		
		g_gfxrenderer->Bind(m_vertexBuffer);
		g_gfxrenderer->Bind(m_indexBuffer);
		g_gfxrenderer->Bind(m_vertexShader);
		g_gfxrenderer->Bind(m_pixelShader);
		g_gfxrenderer->Bind(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);




		

		


		while (m_window->ProcessMessages())
		{
			gfx->BeginFrame();
			// Input polling here
			OnUpdate(0.0f);
			OnRender(0.0f);

			//gfx->GetContext()->DrawIndexed(m_indexBuffer->GetCount(), 0u, 0u);
			g_gfxrenderer->Draw(3, 0);
			
			
			gfx->EndFrame();
			Sleep(0);
		}

		ShutDown();
	}
}
