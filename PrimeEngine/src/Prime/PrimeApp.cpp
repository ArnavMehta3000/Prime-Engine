#include "pch.h"
#include "PrimeApp.h"
#include "Logger.h"

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
		if (m_d3d)
		{
			//m_d3d->ShutDown();
			delete m_d3d;
			m_d3d = nullptr;
		}

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
		TRACE("Initializing app");
		
		m_window = new PrimeWindow();
		m_d3d    = new D3D();

		TRACE("Pre-run Initialise complete")
	}

	void PrimeApp::Run()
	{
		PreRunInit();
		
		while (m_window->ProcessMessages())
		{
		
		}

		ShutDown();
	}
}
