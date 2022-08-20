#include "pch.h"
#include <Prime.h>
#define SHADER_PATH std::wstring(L"../PrimeEngine/Shaders/Compiled/")

class TestApp : public Prime::PrimeApp
{
public:
	TestApp() 
		: m_orthoCam((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT)
	{
	}

	~TestApp()
	{

	}

	virtual void OnStart() override
	{
		// Create vertex buffer
		const Prime::SimpleColorVertex cubeVerts[] =
		{
			{ -1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
			{ -1.0f,-1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f },
			{ -1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
			{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{  1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
			{  1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
			{  1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
			{  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		};
		m_vertexBuffer.reset(
			GetFactory()->CreateVertexBuffer(cubeVerts, UINT(sizeof(Prime::SimpleColorVertex)), ARRAYSIZE(cubeVerts)));

		// Create index buffer
		DWORD cubeIndices[] =
		{
			// Face 1
			0,1,2, // -x
			1,3,2,

			// Face 2
			4,6,5, // +x
			5,6,7,

			// Face 3
			0,5,1, // -y
			0,4,5,

			// Face 4
			2,7,6, // +y
			2,3,7,

			// Face 5
			0,6,4, // -z
			0,2,6,

			// Face 6
			1,7,3, // +z
			1,5,7,
		};
		m_indexBuffer.reset(
			GetFactory()->CreateIndexBuffer(cubeIndices, ARRAYSIZE(cubeIndices)));

		// Create shaders
		D3D11_INPUT_ELEMENT_DESC inputLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		m_vertexShader.reset(
			GetFactory()->CreateVertexShader((SHADER_PATH + L"DefaultVertex.cso").c_str(), inputLayout, ARRAYSIZE(inputLayout)));
		m_pixelShader.reset(
			GetFactory()->CreatePixelShader((SHADER_PATH + L"TexturedPixel.cso").c_str()));			
		m_cameraCBuffer.reset(
			GetFactory()->CreateConstantBuffer<Prime::WVPBuffer>());
		
		GetRenderer()->Bind(m_vertexBuffer);
		GetRenderer()->Bind(m_indexBuffer);
		GetRenderer()->Bind(m_vertexShader);
		GetRenderer()->Bind(m_pixelShader);
		GetRenderer()->Bind(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GetRenderer()->Bind(Prime::ShaderType::VertexShader, m_cameraCBuffer);
	}

	virtual void OnUpdate(float dt) override
	{
		if (GetAsyncKeyState(VK_LEFT))
			x += 0.01f;
		if (GetAsyncKeyState(VK_RIGHT))
			x -= 0.01f;
		if (GetAsyncKeyState(VK_UP))
			y -= 0.01f;
		if (GetAsyncKeyState(VK_DOWN))
			y += 0.01f;
		if (GetAsyncKeyState(VK_PRIOR))
			z += 0.1f;
		if (GetAsyncKeyState(VK_NEXT))
			z -= 0.1f;


		// Update camera
		Matrix world = Matrix::Identity;
		m_orthoCam.SetPosition(Vector3(x, y, -5.0f));
		m_orthoCam.SetRotation(z);
		m_cameraCBuffer->Data.WorldMatrix      = world.Transpose();
		m_cameraCBuffer->Data.ViewMatrix       = m_orthoCam.GetViewMatrix().Transpose();
		m_cameraCBuffer->Data.ProjectionMatrix = m_orthoCam.GetProjectionMatrix().Transpose();

		GetRenderer()->UpdateConstantBuffer(m_cameraCBuffer);
	}

	virtual void OnRender(float dt) override
	{
		GetRenderer()->DrawIndexed(m_indexBuffer);
	}

	virtual void OnClose() override
	{
		m_vertexBuffer->Release();
		m_indexBuffer->Release();
		m_cameraCBuffer->Release();
		m_vertexShader->GetShader()->Release();
		m_pixelShader->GetShader()->Release();
	}

private:
	std::shared_ptr<Prime::VertexShader>                     m_vertexShader;
	std::shared_ptr<Prime::PixelShader>                      m_pixelShader;
	std::shared_ptr<Prime::VertexBuffer>                     m_vertexBuffer;
	std::shared_ptr<Prime::IndexBuffer>                      m_indexBuffer;
	std::shared_ptr<Prime::ConstantBuffer<Prime::WVPBuffer>> m_cameraCBuffer;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	Prime::OrthographicCamera m_orthoCam;
};

Prime::PrimeApp* Prime::CreateApplication()
{
	return new TestApp();
}