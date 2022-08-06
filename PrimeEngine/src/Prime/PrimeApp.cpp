#include "pch.h"
#include "PrimeApp.h"
#include "Prime/Logger.h"
#include "Prime/Constants.h"

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
		WARN("Initializing app");
		
		m_window = new PrimeWindow();
		Locator::RegisterService<GraphicsEngine>();

		D3D_INIT_PARAMS d3dInit{};
		d3dInit.Window.Width = WINDOW_WIDTH;
		d3dInit.Window.Height = WINDOW_HEIGHT;
		d3dInit.Window.Handle = m_window->GetHWND();


		auto gfx = Locator::ResolveService<GraphicsEngine>();
		gfx->Init(d3dInit);
		
		WARN("Pre-run Initialise complete")
	}

	void PrimeApp::Run()
	{
		PreRunInit();
		OnStart();
		
		auto gfx = Locator::ResolveService<GraphicsEngine>();
		while (m_window->ProcessMessages())
		{
			gfx->BeginFrame();
			// Input polling here
			OnUpdate(0.0f);
			OnRender(0.0f);
			gfx->EndFrame();
		}

		ShutDown();
	}
}
