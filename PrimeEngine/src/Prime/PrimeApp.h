#pragma once
#include "Prime/Core/PrimeWindow.h"
#include "Prime/Core/Graphics/GraphicsEngine.h"
#include "Prime/Types/VertexBufferTypes.h"
#include "Prime/Types/ConstantbufferTypes.h"
#include "Prime/Core/CpuTimer.h"


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

		void PrintDebug();

	protected:
		GraphicsEngine*   GetGraphicsEngine();
		GraphicsFactory*  GetFactory();
		GraphicsRenderer* GetRenderer();
		GraphicsRenderer2D* GetRenderer2D();

	protected:
		PrimeWindow* m_window;

	private:
		CpuTimer m_cpuTimer;
	};

	PrimeApp* CreateApplication();
}

