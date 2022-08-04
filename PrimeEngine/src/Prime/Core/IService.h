#pragma once
#include "Prime/ApiDefines.h"

namespace Prime
{
	class PRIME_API IService
	{
	public:
		IService();
		~IService();

	private:
		int m_id = -999;
		static int s_currentId;
	};
}

