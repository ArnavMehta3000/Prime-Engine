#pragma once
#include "ApiDefines.h"

namespace Prime
{
	class PRIME_API PrimeApp
	{
	public:
		PrimeApp();
		virtual ~PrimeApp();

		void Run();

	private:
		void CreateAndAttachConsole();
	};

	PrimeApp* CreateApplication();
}

