#include "pch.h"
#include <Prime.h>
#define SHADER_PATH std::wstring(L"../PrimeEngine/Shaders/Compiled/")

class TestApp : public Prime::PrimeApp
{
public:
	TestApp() : Prime::PrimeApp()
	{
	}

	~TestApp()
	{

	}


	virtual void OnStart() override
	{
		// Create vertex buffer
		const Prime::Vertex vertices[] =
		{
			{ 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
			{ 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
			{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		};
		m_vertexBuffer.reset(GetFactory()->CreateVertexBuffer(vertices, UINT(sizeof(Prime::Vertex)), ARRAYSIZE(vertices)));

		// Create index buffer
		DWORD indices[] =
		{
			1,2,3
		};
		m_indexBuffer.reset(GetFactory()->CreateIndexBuffer(indices, ARRAYSIZE(indices)));

		// Create vertex shader
		D3D11_INPUT_ELEMENT_DESC inputLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		m_vertexShader.reset(GetFactory()->CreateVertexShader((SHADER_PATH + L"DefaultVertex.cso").c_str(), inputLayout, ARRAYSIZE(inputLayout)));
		m_pixelShader.reset(GetFactory()->CreatePixelShader((SHADER_PATH + L"DefaultPixel.cso").c_str()));
		
		
		
		Prime::ConstantBuffer cb{};
		cb.xOffset = 0.5f;
		cb.yOffset = 0.5f;
		
		
		
		

		D3D11_BUFFER_DESC desc{};
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage               = D3D11_USAGE_DYNAMIC;
		desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags           = 0;
		desc.ByteWidth           = static_cast<UINT>(sizeof(Prime::ConstantBuffer)) + (16 - sizeof(Prime::ConstantBuffer) % 16);  // For 16 yte alignment
		desc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA csd{};
		csd.pSysMem = &cb;
		GetGraphicsEngine()->GetDevice()->CreateBuffer(&desc, &csd, m_constantbuffer.GetAddressOf());
		GetGraphicsEngine()->GetContext()->VSSetConstantBuffers(0u, 1u, m_constantbuffer.GetAddressOf());

		D3D11_MAPPED_SUBRESOURCE sub{};
		THROW_HR(
			GetGraphicsEngine()->GetContext()->Map(m_constantbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &sub),
			"Faied to map subresource data"
		);
		CopyMemory(sub.pData, &cb, sizeof(Prime::ConstantBuffer));
		GetGraphicsEngine()->GetContext()->Unmap(m_constantbuffer.Get(), 0);













		GetRenderer()->Bind(m_vertexBuffer);
		GetRenderer()->Bind(m_indexBuffer);
		GetRenderer()->Bind(m_vertexShader);
		GetRenderer()->Bind(m_pixelShader);
		GetRenderer()->Bind(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	virtual void OnUpdate(float dt) override
	{
		
	}
	virtual void OnRender(float dt) override
	{
		GetRenderer()->DrawIndexed(m_indexBuffer);
		GetRenderer()->Draw(3, 0);
	}

	virtual void OnClose() override
	{
		m_vertexBuffer->Release();
	}

private:
	std::shared_ptr<Prime::VertexShader>  m_vertexShader;
	std::shared_ptr<Prime::PixelShader>   m_pixelShader;
	std::shared_ptr<Prime::VertexBuffer>  m_vertexBuffer;
	std::shared_ptr<Prime::IndexBuffer>   m_indexBuffer;
	ComPtr<ID3D11Buffer>       m_constantbuffer;
};

Prime::PrimeApp* Prime::CreateApplication()
{
	return new TestApp();
}