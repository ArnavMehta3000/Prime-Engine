#include <pch.h>
#include "IService.h"

namespace Prime
{
	int Prime::IService::s_currentId = -1;

	IService::IService()
		: m_id(++s_currentId)
	{
	}

	IService::~IService()
	{
		m_id = -999;
		--s_currentId;
	}
}