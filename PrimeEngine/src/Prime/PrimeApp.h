#pragma once
#include "Prime/Core/PrimeWindow.h"
#include "Prime/Core/Graphics/GraphicsEngine.h"
#include "Prime/Core/Service Locator/Locator.h"

namespace Prime
{
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
		PrimeWindow*               m_window;
		ComPtr<ID3D11VertexShader> m_vertexShader;
		ComPtr<ID3D11Buffer>       m_vertexBuffer;
		ComPtr<ID3D11InputLayout>  m_inputLayout;

	};

	PrimeApp* CreateApplication();

	GraphicsEngine* GetGraphicsEngine();
}

