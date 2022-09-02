#include "pch.h"
#include "GraphicsRenderer2D.h"
#include <filesystem>
#include "Prime/Types/VertexBufferTypes.h"
#include "Prime/Core/Graphics/GraphicsFactory.h"


namespace Prime
{
	Prime::GraphicsRenderer2D::GraphicsRenderer2D()
	{
		m_device = nullptr;
		m_context = nullptr;
	}

	Prime::GraphicsRenderer2D::~GraphicsRenderer2D()
	{
	}

	void Prime::GraphicsRenderer2D::Init2D(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		m_device = device;
		m_context = context;

		this->Init(device, context);
		InitQuad();

		BindDefaults();
	}

	void GraphicsRenderer2D::InitQuad()
	{
		auto factory = Locator::ResolveService<GraphicsFactory>();
		LOG("Directory:" << std::filesystem::current_path());
		const SimpleVertex quadVerts[] =
		{
			{ -1.0f, 1.0f, 0.0f },
			{  1.0f, 1.0f, 0.0f },
			{ -1.0f,-1.0f, 0.0f },
			{  1.0f,-1.0f, 0.0f },
		};
		m_quadVB.reset(factory->CreateVertexBuffer(quadVerts, UINT(sizeof(SimpleVertex)), ARRAYSIZE(quadVerts)));

		DWORD quadIndices[] = { 0, 1, 2, 2, 1, 3 };
		m_quadIB.reset(factory->CreateIndexBuffer(quadIndices, ARRAYSIZE(quadIndices)));

		D3D11_INPUT_ELEMENT_DESC inputLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		D3D11_INPUT_ELEMENT_DESC instanceLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SV_InstanceID", 0, DXGI_FORMAT_R32_UINT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 0},
		};
		//m_instanceVS.reset(factory->CreateVertexShader((SHADER_PATH + L"PrimitiveVertex.cso").c_str(), instanceLayout, ARRAYSIZE(instanceLayout)));
		
		m_primitivesVS.reset(factory->CreateVertexShader(L"Shaders/SimpleVertex.cso", inputLayout, ARRAYSIZE(inputLayout)));
		m_primitivesPS.reset(factory->CreatePixelShader(L"Shaders/SimplePixel.cso"));
		
		m_primitiveColor = Color(1.0f, 0.0f, 0.0f, 1.0f);
	}


	void GraphicsRenderer2D::DrawQuad()
	{
		Bind(m_quadIB);
		Bind(m_quadVB);
		Bind(m_primitivesVS);
		Bind(m_primitivesPS);
		//m_context->DrawIndexedInstanced(m_quadIB->GetCount(), 25u, 0u, 0u, 0u);
		DrawIndexed(m_quadIB);
	}
}
