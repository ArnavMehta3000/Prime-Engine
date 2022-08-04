#pragma once
#include "Prime/ApiDefines.h"

namespace Prime
{
	class PRIME_API IService
	{
	public:
		IService();
		~IService();

		inline int GetServiceId() { return m_id; }

	private:
		int m_id = -999;
		static int s_currentId;
	};
}

