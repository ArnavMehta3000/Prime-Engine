#pragma once

namespace Prime
{
	class IService
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

