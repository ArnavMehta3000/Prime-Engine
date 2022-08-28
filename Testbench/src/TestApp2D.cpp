#include <pch.h>
#include "TestApp2D.h"

TestApp2D::TestApp2D()
	: m_orthoCam((float)Prime::PrimeWindow::s_clientWidth, (float)Prime::PrimeWindow::s_clientHeight)
{

}

void TestApp2D::OnStart()
{
	// Create vertex buffer
	const Prime::TexturedVertex quadVerts[] =
	{
		{ -1.0f, 1.0f, 0.0f, 0.0f, 0.0f },
		{  1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
		{ -1.0f,-1.0f, 0.0f, 0.0f, 1.0f },
		{  1.0f,-1.0f, 0.0f, 1.0f, 1.0f },
	};
	m_quadVB.reset(GetFactory()->CreateVertexBuffer(quadVerts, UINT(sizeof(Prime::TexturedVertex)), ARRAYSIZE(quadVerts)));


	// CReate index buffer
	DWORD quadIndices[] = { 0, 1, 2, 2, 1, 3 };
	m_quadIB.reset(
		GetFactory()->CreateIndexBuffer(quadIndices, ARRAYSIZE(quadIndices)));


	D3D11_INPUT_ELEMENT_DESC texturedInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_textureVS.reset(GetFactory()->CreateVertexShader((SHADER_PATH + L"TexturedVertex.cso").c_str(), texturedInputLayout, ARRAYSIZE(texturedInputLayout)));
	m_texturePS.reset(GetFactory()->CreatePixelShader((SHADER_PATH + L"TexturedPixel.cso").c_str()));


	m_cameraCBuffer.reset(GetFactory()->CreateConstantBuffer<Prime::WVPBuffer>());

	m_texture.reset(GetFactory()->CreateTextureFromFile((ASSET_PATH + L"Test.png").c_str(), D3D11_USAGE_DEFAULT, D3D10_BIND_SHADER_RESOURCE, 0, 0, WIC_LOADER_DEFAULT));

	GetRenderer()->BindDefaults();
	GetRenderer()->Bind(m_quadVB);
	GetRenderer()->Bind(m_quadIB);
	GetRenderer()->Bind(Prime::ShaderType::PixelShader, m_texture);
	GetRenderer()->Bind(Prime::ShaderType::VertexShader, m_cameraCBuffer);
	GetGraphicsEngine()->SetWireframe(false);
}

void TestApp2D::OnUpdate(float dt)
{
	const float cameraMoveSpeed = 5.0f;
	const float cameraRotSpeed = 50.0f;
	
	if (GetAsyncKeyState(VK_LEFT))
		x += cameraMoveSpeed * dt;
	if (GetAsyncKeyState(VK_RIGHT))
		x -= cameraMoveSpeed * dt;
	if (GetAsyncKeyState(VK_UP))
		y -= cameraMoveSpeed * dt;
	if (GetAsyncKeyState(VK_DOWN))
		y += cameraMoveSpeed * dt;
	if (GetAsyncKeyState(VK_PRIOR))
		z += cameraRotSpeed * dt;
	if (GetAsyncKeyState(VK_NEXT))
		z -= cameraRotSpeed * dt;

	if (GetAsyncKeyState(VK_CONTROL))
		scaleCube += 1.0f * dt;
	if (GetAsyncKeyState(VK_SHIFT))
		scaleCube -= 1.0f * dt;


	Vector3 camPos = m_orthoCam.GetPosition() + Vector3(x, y, 0.0f);
	m_orthoCam.SetPosition(camPos);
	m_orthoCam.SetRotation(z);
	x = y = 0;


	m_cameraCBuffer->Data.WorldMatrix      = Matrix::Identity.Transpose();
	m_cameraCBuffer->Data.ViewMatrix       = m_orthoCam.GetViewMatrix().Transpose();
	m_cameraCBuffer->Data.ProjectionMatrix = m_orthoCam.GetProjectionMatrix().Transpose();

	GetRenderer()->UpdateConstantBuffer(m_cameraCBuffer);
}

void TestApp2D::OnRender(float dt)
{
	GetRenderer()->Bind(m_textureVS);
	GetRenderer()->Bind(m_texturePS);
	GetRenderer()->DrawIndexed(m_quadIB);
}

void TestApp2D::OnClose()
{

}