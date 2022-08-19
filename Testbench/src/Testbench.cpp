#include "pch.h"
#include <Prime.h>
#define SHADER_PATH std::wstring(L"../PrimeEngine/Shaders/Compiled/")

class TestApp : public Prime::PrimeApp
{
public:
	TestApp() 
		: m_camera(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, 0.1f, 1.0f)
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
			{ -1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
			{ -1.0f,-1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f },
			{ -1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
			{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
			{  1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
			{  1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
			{  1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f },
			{  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		};
		m_vertexBuffer.reset(GetFactory()->CreateVertexBuffer(vertices, UINT(sizeof(Prime::Vertex)), ARRAYSIZE(vertices)));

		// Create index buffer
		DWORD indices[] =
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
		m_constantbuffer.reset(
			GetFactory()->CreateConstantBuffer<Prime::CBuffer>());

		m_camera.SetPosition(Vector3(0.0f, 0.0f, -2.0f));

		
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

		TRACE("Position: " << x << ", " << y << ", " << z);

		m_camera.SetPosition(Vector3(x, y, z));
		m_camera.UpdateMatrices();

		static float t;
		t += dt;

		m_constantbuffer->Data.WorldMat         = (Matrix::CreateScale(0.5f) * Matrix::CreateTranslation(x, y, z) * Matrix::CreateRotationX(t) * Matrix::CreateRotationZ(t)).Transpose();
		m_constantbuffer->Data.ViewMatrix       = m_camera.GetViewMatrix().Transpose();
		m_constantbuffer->Data.ProjectionMatrix = m_camera.GetProjectionMatrix().Transpose();

		GetRenderer()->UpdateConstantBuffer(m_constantbuffer);
	}
	virtual void OnRender(float dt) override
	{
		GetRenderer()->DrawIndexed(m_indexBuffer);
		GetRenderer()->Draw(3*2*6, 0);
	}

	virtual void OnClose() override
	{
		m_vertexBuffer->Release();
		m_indexBuffer->Release();
		m_constantbuffer->Release();
		m_vertexShader->GetShader()->Release();
		m_pixelShader->GetShader()->Release();
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

	Prime::OrthographicCamera m_camera;
};

Prime::PrimeApp* Prime::CreateApplication()
{
	return new TestApp();
}