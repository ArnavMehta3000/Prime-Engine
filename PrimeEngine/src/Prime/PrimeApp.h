#pragma once
#include "Prime/Core/PrimeWindow.h"
#include "Prime/Core/Graphics/GraphicsEngine.h"
#include "Prime/Core/ServiceLocator/Locator.h"
#include "Prime/Core/Timer.h"
#include "Prime/Types/VertexBufferTypes.h"
#include "Prime/Types/ConstantbufferTypes.h"

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
		virtual void OnClose() = 0;

	private:
		void PreRunInit();
		void ShutDown();
		void CreateAndAttachConsole();

	protected:
		GraphicsEngine* GetGraphicsEngine();
		GraphicsFactory* GetFactory();
		GraphicsRenderer* GetRenderer();

	protected:
		PrimeWindow* m_window;

	private:
		std::unique_ptr<Timer> m_appTimer;
	};

	PrimeApp* CreateApplication();
}

