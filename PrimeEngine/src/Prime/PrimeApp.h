#pragma once
#include "ApiDefines.h"
#include "Prime/Core/PrimeWindow.h"
#include "Prime/Core/D3D.h"
#include "Prime/Core/Locator.h"

namespace Prime
{
	class PRIME_API PrimeApp
	{
	public:
		PrimeApp();
		virtual ~PrimeApp();

		void Run();

	private:
		void PreRunInit();
		void ShutDown();
		void CreateAndAttachConsole();

	private:
		PrimeWindow* m_window;
	};

	PrimeApp* CreateApplication();
}

