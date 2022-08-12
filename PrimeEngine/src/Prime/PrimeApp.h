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

	private:
		PrimeWindow*                   m_window;
		std::unique_ptr<VertexShader>  m_vertexShader;
		std::unique_ptr<PixelShader>   m_pixelShader;
		std::unique_ptr<VertexBuffer>  m_vertexBuffer;
		std::unique_ptr<IndexBuffer>   m_indexBuffer;

	};

	PrimeApp* CreateApplication();

	GraphicsEngine* GetGraphicsEngine();
}

