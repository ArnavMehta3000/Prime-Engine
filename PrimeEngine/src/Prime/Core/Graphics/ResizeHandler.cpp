#include <pch.h>
#include "ResizeHandler.h"

namespace Prime
{
	std::vector<std::function<void(int, int)>> ResizeHandler::s_resizeFunctions;
	bool ResizeHandler::m_isResizeOrMoving = false;

	void ResizeHandler::RegisterFunction(std::function<void(int, int)> func)
	{
		s_resizeFunctions.push_back(func);
		TRACE("Registered Resize functions count: " << s_resizeFunctions.size());
	}

	void ResizeHandler::Resize(int w, int h)
	{
		for (auto& func : s_resizeFunctions)
		{
			func(w, h);
		}
	}
}