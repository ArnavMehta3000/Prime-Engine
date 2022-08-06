#include "pch.h"
#include "GraphicsEngine.h"

namespace Prime
{
	GraphicsEngine::GraphicsEngine()
	{
		m_d3d = std::make_unique<D3D>();
	}
	GraphicsEngine::~GraphicsEngine()
	{
	}



	PRIME_API void GraphicsEngine::Init(D3D_INIT_PARAMS d3dInit)
	{
		m_d3d->Init(d3dInit);
	}

	PRIME_API void GraphicsEngine::Shutdown()
	{
		m_d3d->Shutdown();
	}



	PRIME_API void GraphicsEngine::BeginFrame()
	{
	}

	PRIME_API void GraphicsEngine::EndFrame()
	{
	}
}
