#include "pch.h"
#include "PrimeApp.h"

namespace Prime
{
	PrimeApp::PrimeApp()
	{
		CreateAndAttachConsole();		
	}
	
	PrimeApp::~PrimeApp()
	{
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
#endif // _DEBUG
	}
	
	void PrimeApp::Run()
	{
		while (true)
		{
		
		}
	}
}
