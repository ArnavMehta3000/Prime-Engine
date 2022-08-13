#include "pch.h"
#include "Locator.h"

namespace Prime
{
	Locator* Locator::s_instance = nullptr;

	Locator* Locator::Instance()
	{
		if (s_instance == nullptr)
			s_instance = new Locator();

		return s_instance;
	}

	void Locator::Shutdown()
	{
		Instance()->ShutdownImpl();
	}
	void Locator::ShutdownImpl()
	{
		for (const auto& [key, value] : m_services)
		{
			m_services.erase(key);
		}
	}
}