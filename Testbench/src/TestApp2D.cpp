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
	z = m_orthoCam.GetOrthoScale();
	GetGraphicsEngine()->SetWireframe(0);
}

void TestApp2D::OnUpdate(float dt)
{
	const float cameraMoveSpeed = 5.0f;
	const float cameraRotSpeed = 20.0f;

	
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


	Vector3 camPos = m_orthoCam.GetPosition() + Vector3(x, y, 0.0f);
	m_orthoCam.SetPosition(camPos);
	m_orthoCam.SetOrthoScale(z);

	x = y = 0;

	m_cameraCBuffer->Data.WorldMatrix      = Matrix::Identity.Transpose();
	m_cameraCBuffer->Data.ViewMatrix       = m_orthoCam.GetViewMatrix().Transpose();
	m_cameraCBuffer->Data.ProjectionMatrix = m_orthoCam.GetProjectionMatrix().Transpose();

	GetRenderer2D()->UpdateConstantBuffer(m_cameraCBuffer);
}

void TestApp2D::OnRender(float dt)
{
	static float t;
	t += dt;\
	GetRenderer2D()->BeginQuadBatch(m_orthoCam.GetViewMatrix(), m_orthoCam.GetProjectionMatrix());
	int rows = 10;
	int cols = 10;
	Vector3 pos;
	/*for (int r = -rows; r < rows; r++)
	{
		for (int c = -cols; c < cols; c++)
		{
			Matrix world = Matrix::CreateScale(0.5f) * Matrix::CreateTranslation(Vector3((float)r, (float)c, 0.0f));
			float red = (float)r / (float)rows;
			float green = (float)c / (float)cols;
			Color color = Color(abs(red), abs(green), 0.0f, 1.0f);
			GetRenderer2D()->DrawQuadBatch(world, color);
		}
	}*/

	GetRenderer2D()->DrawQuadBatch(Matrix::Identity, Color(Colors::Tomato));
	
}

void TestApp2D::OnClose()
{

}

// NOTE: How to create a white texture on the gpu and save it there
/*
* How to create a complete ehite texture
* Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pWhiteTexture;

    static const uint32_t s_pixel = 0xffffffff;

    D3D11_SUBRESOURCE_DATA initData = { &s_pixel, sizeof(uint32_t), 0 };

    D3D11_TEXTURE2D_DESC desc;
    memset( &desc, 0, sizeof(desc) );
    desc.Width = desc.Height = desc.MipLevels = desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
    HRESULT hr = mDevice->CreateTexture2D( &desc, &initData, &tex );

    DX::ThrowIfFailed(hr);

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    memset( &SRVDesc, 0, sizeof( SRVDesc ) );
    SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = 1;

    hr = mDevice->CreateShaderResourceView( tex.Get(), &SRVDesc, &pWhiteTexture );

    DX::ThrowIfFailed(hr);
*/