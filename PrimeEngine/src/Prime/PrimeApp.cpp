#include "pch.h"
#include "PrimeApp.h"
#include "Prime/Logger.h"
#include "Prime/Constants.h"

namespace Prime
{
	GraphicsEngine* PrimeApp::GetGraphicsEngine() { return Locator::ResolveService<GraphicsEngine>(); }
	GraphicsFactory*  PrimeApp::GetFactory()  { return Locator::ResolveService<GraphicsFactory>(); }
	GraphicsRenderer* PrimeApp::GetRenderer() { return Locator::ResolveService<GraphicsRenderer>(); }

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
		
		m_appTimer = std::make_unique<Timer>();

		LOG_LOAD("Pre-run Initialise complete");
	}

	void PrimeApp::Run()
	{
		PreRunInit();
		OnStart();
		auto gfx = Locator::ResolveService<GraphicsEngine>();
		
		while (m_window->ProcessMessages())
		{
			float deltaTime = m_appTimer->Mark();
			
			gfx->BeginFrame();
			// Input polling here
			OnUpdate(deltaTime);
			OnRender(deltaTime);
			gfx->EndFrame();
			Sleep(0);
		}
		OnClose();
		ShutDown();
	}
}
