#include "pch.h"
#include "PrimeApp.h"
#include "Prime/Logger.h"
#include "Prime/Core/Graphics/ResizeHandler.h"

namespace Prime
{
	GraphicsEngine* PrimeApp::GetGraphicsEngine() { return Locator::ResolveService<GraphicsEngine>(); }
	GraphicsFactory*  PrimeApp::GetFactory()      { return Locator::ResolveService<GraphicsFactory>(); }
	GraphicsRenderer* PrimeApp::GetRenderer()     { return Locator::ResolveService<GraphicsRenderer>(); }
	GraphicsRenderer2D* PrimeApp::GetRenderer2D() { return Locator::ResolveService<GraphicsRenderer2D>(); }

	PrimeApp::PrimeApp() :
		m_cpuTimer(CpuTimer())
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

	void PrimeApp::PrintDebug()
	{
		TRACE("")
		TRACE("FPS:" << 1.0f / m_cpuTimer.DeltaTime());
		TRACE("Delta Time: " << m_cpuTimer.DeltaTime());
	}
	
	void Prime::PrimeApp::PreRunInit()
	{
		LOG_LOAD("Initializing app");
		
		m_window = new PrimeWindow(800, 600, true);
		Locator::RegisterService<GraphicsEngine>();

		D3D_INIT_PARAMS d3dInit{};
		d3dInit.Window.Width  = PrimeWindow::s_clientWidth;
		d3dInit.Window.Height = PrimeWindow::s_clientHeight;
		d3dInit.Window.Handle = m_window->GetHWND();
		d3dInit.VSync         = false;

		auto gfx = Locator::ResolveService<GraphicsEngine>();
		gfx->InitCore(d3dInit);

		ResizeHandler::Resize(PrimeWindow::s_clientWidth, PrimeWindow::s_clientHeight);
		

		LOG_LOAD("Pre-run Initialise complete");
	}

	void PrimeApp::Run()
	{
#ifdef _DEBUG
		const float printTime = 5.0f;
		float currTime = printTime;
#endif // _DEBUG


		PreRunInit();
		OnStart();

		m_cpuTimer.Reset();
		m_cpuTimer.Start();

		auto gfx = Locator::ResolveService<GraphicsEngine>();

		while (m_window->ProcessMessages())
		{
			m_cpuTimer.Tick();

			gfx->BeginFrame();
			OnUpdate(m_cpuTimer);
			OnRender(m_cpuTimer);
			gfx->EndFrame();

#ifdef _DEBUG
			currTime -= m_cpuTimer;
			if (currTime <= 0.0f)
			{
				PrintDebug();
				currTime = printTime;
			}
#endif // DEBUG

			Sleep(0);
		}

		OnClose();
		ShutDown();
	}
}
