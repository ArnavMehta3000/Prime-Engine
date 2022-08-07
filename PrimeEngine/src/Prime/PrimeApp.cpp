#include "pch.h"
#include "PrimeApp.h"
#include "Prime/Logger.h"
#include "Prime/Constants.h"
#include "Effects.h"
#include <DirectXHelpers.h>

Prime::GraphicsEngine* Prime::GetGraphicsEngine()
{
	return Locator::ResolveService<GraphicsEngine>();
}

namespace Prime
{
	PrimeApp::PrimeApp()
	{
		m_window = nullptr;
		CreateAndAttachConsole();		
	}
	
	PrimeApp::~PrimeApp()
	{
	}

	void PrimeApp::ShutDown()
	{
		Locator::ResolveService<GraphicsEngine>()->Shutdown();

		if (m_window)
		{
			delete m_window;
			m_window = nullptr;
		}
	}

	void PrimeApp::CreateAndAttachConsole()
	{
#ifdef _DEBUG
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		FILE* fp = nullptr;
		freopen_s(&fp, "CONIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);

		LOG("Successfully attached console");
#endif // _DEBUG
	}
	
	void Prime::PrimeApp::PreRunInit()
	{
		WARN("Initializing app");
		
		m_window = new PrimeWindow();
		Locator::RegisterService<GraphicsEngine>();

		D3D_INIT_PARAMS d3dInit{};
		d3dInit.Window.Width = WINDOW_WIDTH;
		d3dInit.Window.Height = WINDOW_HEIGHT;
		d3dInit.Window.Handle = m_window->GetHWND();
		d3dInit.VSync = false;


		auto gfx = Locator::ResolveService<GraphicsEngine>();
		gfx->Init(d3dInit);
		
		WARN("Pre-run Initialise complete");
	}

	void PrimeApp::Run()
	{
		PreRunInit();
		OnStart();
		auto gfx = Locator::ResolveService<GraphicsEngine>();

		struct Vertex
		{
			float x, y, z;
		};

		const Vertex vertices[] =
		{
			{ 0.0f, 0.5f, 0.0f },
			{ 0.5f, -0.5f, 0.0f },
			{ -0.5f, -0.5f, 0.0f },
		};
		
		D3D11_BUFFER_DESC vbd{};
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.CPUAccessFlags = 0u;
		vbd.MiscFlags = 0u;
		vbd.ByteWidth = sizeof(vertices);
		vbd.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA vsd{};
		vsd.pSysMem = vertices;

		THROW_HR(gfx->GetDevice()->CreateBuffer(&vbd, &vsd, m_vertexBuffer.GetAddressOf()), 
			"Failed to create vertex buffer");
		UINT stride = sizeof(Vertex);
		UINT offset = 0u;
		gfx->GetContext()->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &stride, &offset);


		ComPtr<ID3D10Blob> vertexBlob;
		THROW_HR(D3DReadFileToBlob((SHADER_PATH + L"DefaultVertex.cso").c_str(), vertexBlob.GetAddressOf()),
			"Failed to read vertex shader to blob");

		THROW_HR(gfx->GetDevice()->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), nullptr, &m_vertexShader),
			"Failed to create vertex shader object");

		
		while (m_window->ProcessMessages())
		{
			gfx->BeginFrame();
			// Input polling here
			OnUpdate(0.0f);
			OnRender(0.0f);
			gfx->EndFrame();
			Sleep(0);
		}

		ShutDown();
	}
}
