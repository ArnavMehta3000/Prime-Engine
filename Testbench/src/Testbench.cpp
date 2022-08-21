#include "pch.h"
#include <Prime.h>
#define SHADER_PATH std::wstring(L"../PrimeEngine/Shaders/Compiled/")
#define ASSET_PATH  std::wstring(L"../PrimeEngine/Assets/")

class TestApp : public Prime::PrimeApp
{
public:
	TestApp() 
		: m_orthoCam((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT)
	{
	}

	virtual void OnStart() override
	{
		// Create vertex buffer
		const Prime::ColorVertex cubeVerts[] =
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

		const Prime::TexturedVertex quadVerts[] =
		{
			{ -1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
			{  1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
			{ -1.0f,-1.0f, 0.0f, 0.0f, 1.0f },
			{  1.0f,-1.0f, 0.0f, 1.0f, 1.0f },
		};

		m_cubeVB.reset(
			GetFactory()->CreateVertexBuffer(cubeVerts, UINT(sizeof(Prime::ColorVertex)), ARRAYSIZE(cubeVerts)));

		m_quadVB.reset(
			GetFactory()->CreateVertexBuffer(quadVerts, UINT(sizeof(Prime::TexturedVertex)), ARRAYSIZE(quadVerts)));

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

		DWORD quadIndices[] = { 0, 1, 2, 2, 1, 3};


		m_cubeIB.reset(
			GetFactory()->CreateIndexBuffer(cubeIndices, ARRAYSIZE(cubeIndices)));

		m_quadIB.reset(
			GetFactory()->CreateIndexBuffer(quadIndices, ARRAYSIZE(quadIndices)));

		// Create shaders
		D3D11_INPUT_ELEMENT_DESC colorInputLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		D3D11_INPUT_ELEMENT_DESC texturedInputLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		m_colorVS.reset(
			GetFactory()->CreateVertexShader((SHADER_PATH + L"DefaultVertex.cso").c_str(), colorInputLayout, ARRAYSIZE(colorInputLayout)));
		m_colorPS.reset(
			GetFactory()->CreatePixelShader((SHADER_PATH + L"DefaultPixel.cso").c_str()));	

		m_textureVS.reset(
			GetFactory()->CreateVertexShader((SHADER_PATH + L"TexturedVertex.cso").c_str(), texturedInputLayout, ARRAYSIZE(texturedInputLayout)));
		m_texturePS.reset(
			GetFactory()->CreatePixelShader((SHADER_PATH + L"TexturedPixel.cso").c_str()));

		m_cameraCBuffer.reset(
			GetFactory()->CreateConstantBuffer<Prime::WVPBuffer>());

		m_texture.reset(
			GetFactory()->CreateTextureFromFile((ASSET_PATH + L"Test.png").c_str(),	D3D11_USAGE_DEFAULT, D3D10_BIND_SHADER_RESOURCE, 0, 0, WIC_LOADER_DEFAULT));

				
		GetRenderer()->Bind(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GetRenderer()->Bind(Prime::ShaderType::VertexShader, m_cameraCBuffer);
		GetGraphicsEngine()->SetWireframe(true);
	}

	virtual void OnUpdate(float dt) override
	{
		if (GetAsyncKeyState(VK_LEFT))
			x += 1.0f * dt;
		if (GetAsyncKeyState(VK_RIGHT))
			x -= 1.01f * dt;
		if (GetAsyncKeyState(VK_UP))
			y -= 1.0f * dt;
		if (GetAsyncKeyState(VK_DOWN))
			y += 1.0f * dt;
		if (GetAsyncKeyState(VK_PRIOR))
			z += 1.0f * dt;
		if (GetAsyncKeyState(VK_NEXT))
			z -= 1.0f * dt;
		

		// Update camera
		Matrix world = Matrix::CreateRotationX(-y) * Matrix::CreateRotationY(x) * Matrix::CreateRotationZ(z);
		//m_orthoCam.SetPosition(Vector3(x, y, -5.0f));
		//m_orthoCam.SetRotation(z);
		m_cameraCBuffer->Data.WorldMatrix      = world.Transpose();
		m_cameraCBuffer->Data.ViewMatrix       = m_orthoCam.GetViewMatrix().Transpose();
		m_cameraCBuffer->Data.ProjectionMatrix = m_orthoCam.GetProjectionMatrix().Transpose();

		GetRenderer()->UpdateConstantBuffer(m_cameraCBuffer);
	}

	virtual void OnRender(float dt) override
	{
		// Bind buffers
		GetRenderer()->Bind(m_quadVB);
		GetRenderer()->Bind(m_quadIB);

		// Bind shaders
		GetRenderer()->Bind(m_textureVS);
		GetRenderer()->Bind(m_texturePS);

		GetRenderer()->DrawIndexed(m_quadIB);
	}

	virtual void OnClose() override
	{
		RELEASE(m_texture)
		
		RELEASE(m_cubeVB)
		RELEASE(m_quadVB)
		
		RELEASE(m_quadIB)
		RELEASE(m_cubeIB)
		
		RELEASE(m_colorVS->GetShader())
		RELEASE(m_colorVS->GetInputLayout())
		RELEASE(m_textureVS->GetShader())
		RELEASE(m_textureVS->GetInputLayout())
		
		RELEASE(m_colorPS->GetShader())
		RELEASE(m_texturePS->GetShader())
		
		RELEASE(m_cameraCBuffer)
	}

private:
	std::shared_ptr<Prime::VertexShader> m_colorVS;
	std::shared_ptr<Prime::VertexShader> m_textureVS;
										 
	std::shared_ptr<Prime::PixelShader>  m_colorPS;
	std::shared_ptr<Prime::PixelShader>  m_texturePS;
										 
	std::shared_ptr<Prime::VertexBuffer> m_cubeVB;
	std::shared_ptr<Prime::VertexBuffer> m_quadVB;
										 
	std::shared_ptr<Prime::IndexBuffer>  m_cubeIB;
	std::shared_ptr<Prime::IndexBuffer>  m_quadIB;

	std::shared_ptr<Prime::Texture2D>    m_texture;

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