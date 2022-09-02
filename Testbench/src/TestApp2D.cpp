#include <pch.h>
#include "TestApp2D.h"

TestApp2D::TestApp2D()
	: m_orthoCam((float)Prime::PrimeWindow::s_clientWidth, (float)Prime::PrimeWindow::s_clientHeight)
{

}

void TestApp2D::OnStart()
{
	// Create constant buffers
	m_cameraCBuffer.reset(GetFactory()->CreateConstantBuffer<Prime::WVPBuffer>());
	m_pixelCBuffer.reset(GetFactory()->CreateConstantBuffer<Prime::ColorBuffer>());

	//m_texture.reset(GetFactory()->CreateTextureFromFile((ASSET_PATH + L"Test.png").c_str(), D3D11_USAGE_DEFAULT, D3D10_BIND_SHADER_RESOURCE, 0, 0, WIC_LOADER_DEFAULT));

	GetRenderer2D()->Bind(Prime::ShaderType::VertexShader, m_cameraCBuffer);
	GetRenderer2D()->Bind(Prime::ShaderType::PixelShader, m_pixelCBuffer);
	GetGraphicsEngine()->SetWireframe(true);
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

	GetRenderer2D()->UpdateConstantBuffer(m_cameraCBuffer);
}

void TestApp2D::OnRender(float dt)
{
	static float t;
	t += dt;

	GetRenderer2D()->SetPrimitivesColor(Color(1.0f, 0.0f, 1.0f));
	auto col = GetRenderer2D()->GetPrimitivesColor();
	m_pixelCBuffer->Data = { col.R(), col.G() , col.B() , col.A() };
	m_cameraCBuffer->Data.WorldMatrix = Matrix::CreateTranslation(Vector3(cos(t) * 2.0f, 0.0f, 0.0f)).Transpose();

	GetRenderer()->UpdateConstantBuffer(m_pixelCBuffer);
	GetRenderer2D()->UpdateConstantBuffer(m_cameraCBuffer);
	GetRenderer2D()->DrawQuad();
	


	/*GetRenderer2D()->SetPrimitivesColor(Color(0.0f, 1.0f, 1.0f));
	col = GetRenderer2D()->GetPrimitivesColor();
	m_pixelCBuffer->Data = { col.R(), col.G() , col.B() , col.A() };
	m_cameraCBuffer->Data.WorldMatrix = Matrix::CreateTranslation(Vector3(sin(t) * 2.0f, 0.0f, 0.0f)).Transpose();

	GetRenderer()->UpdateConstantBuffer(m_pixelCBuffer);
	GetRenderer2D()->UpdateConstantBuffer(m_cameraCBuffer);
	GetRenderer2D()->DrawQuad();*/
}

void TestApp2D::OnClose()
{

}