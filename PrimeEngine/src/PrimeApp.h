#pragma once
#include "ApiDefines.h"

namespace Prime
{
	class PrimeApp
	{
	public:
		PRIME_API PrimeApp();
		PRIME_API virtual ~PrimeApp();

		PRIME_API void Run();
	};
}

