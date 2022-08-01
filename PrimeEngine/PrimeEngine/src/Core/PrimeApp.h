#pragma once
#include "ApiDefines.h"

namespace Prime
{
	class PrimeApp
	{
	public:
		PrimeApp();
		virtual ~PrimeApp();

		PRIME_API void Run();
	};
}

