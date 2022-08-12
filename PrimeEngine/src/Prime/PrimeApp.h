#pragma once
#include "Prime/Core/PrimeWindow.h"
#include "Prime/Core/Graphics/GraphicsEngine.h"
#include "Prime/Core/ServiceLocator/Locator.h"

namespace Prime
{
	struct Vertex
	{
		float x, y, z;
	};

	class PrimeApp
	{
	public:
		PrimeApp();
		virtual ~PrimeApp();

		void Run();

		virtual void OnStart() = 0;
		virtual void OnUpdate(float dt) = 0;
		virtual void OnRender(float dt) = 0;

	private:
		void PreRunInit();
		void ShutDown();
		void CreateAndAttachConsole();

	protected:
		std::unique_ptr<Prime::GraphicsFactory> g_gfxFactory;
		std::unique_ptr<Prime::GraphicsRenderer> g_gfxrenderer;
		

	private:
		PrimeWindow*                   m_window;
		std::shared_ptr<VertexShader>  m_vertexShader;
		std::shared_ptr<PixelShader>   m_pixelShader;
		std::shared_ptr<VertexBuffer>  m_vertexBuffer;
		std::shared_ptr<IndexBuffer>   m_indexBuffer;

	};

	PrimeApp* CreateApplication();
}

