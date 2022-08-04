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
}