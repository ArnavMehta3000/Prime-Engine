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
			{  0.0f, 0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
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

		m_vertexShader.reset(
			GetFactory()->CreateVertexShader((SHADER_PATH + L"DefaultVertex.cso").c_str(), inputLayout, ARRAYSIZE(inputLayout)));
		m_pixelShader.reset(
			GetFactory()->CreatePixelShader((SHADER_PATH + L"DefaultPixel.cso").c_str()));	

		using namespace SimpleMath;
		Matrix world = XMMatrixTranslation(0, 0, 0);
		Matrix view  = XMMatrixLookAtLH(Vector3(0.0f, 0.0f, -2.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
		Matrix proj  = XMMatrixPerspectiveLH(WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 100.0f);

		m_constantbuffer.reset(GetFactory()->CreateConstantBuffer<Prime::CBuffer>());
		m_constantbuffer->Data.WorldMat         = world.Transpose();
		m_constantbuffer->Data.ViewMatrix       = view.Transpose();
		m_constantbuffer->Data.ProjectionMatrix = proj.Transpose();
		

		
		GetRenderer()->Bind(m_vertexBuffer);
		GetRenderer()->Bind(m_indexBuffer);
		GetRenderer()->Bind(m_vertexShader);
		GetRenderer()->Bind(m_pixelShader);
		GetRenderer()->Bind(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GetRenderer()->Bind(Prime::ShaderType::VertexShader, m_constantbuffer);
	}

	virtual void OnUpdate(float dt) override
	{
		if (GetAsyncKeyState(VK_LEFT))
			x -= 0.001f;
		if (GetAsyncKeyState(VK_RIGHT))
			x += 0.001f;
		if (GetAsyncKeyState(VK_UP))
			y += 0.001f;
		if (GetAsyncKeyState(VK_DOWN))
			y -= 0.001f;
		if (GetAsyncKeyState(VK_PRIOR))
			z += 0.001f;
		if (GetAsyncKeyState(VK_NEXT))
			z -= 0.001f;

		Matrix world = XMMatrixTranslation(x, y, z);
		m_constantbuffer->Data.WorldMat = world.Transpose();

		GetRenderer()->UpdateConstantBuffer(m_constantbuffer);
	}
	virtual void OnRender(float dt) override
	{
		GetRenderer()->DrawIndexed(m_indexBuffer);
	}

	virtual void OnClose() override
	{
		m_vertexBuffer->Release();
	}

private:
	std::shared_ptr<Prime::VertexShader>                         m_vertexShader;
	std::shared_ptr<Prime::PixelShader>                          m_pixelShader;
	std::shared_ptr<Prime::VertexBuffer>                         m_vertexBuffer;
	std::shared_ptr<Prime::IndexBuffer>                          m_indexBuffer;
	std::shared_ptr<Prime::ConstantBuffer<Prime::CBuffer>>       m_constantbuffer;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

Prime::PrimeApp* Prime::CreateApplication()
{
	return new TestApp();
}