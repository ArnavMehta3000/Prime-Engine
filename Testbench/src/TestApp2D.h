#pragma once
#include <Prime.h>

class TestApp2D : public Prime::PrimeApp
{
public:
	TestApp2D();

	virtual void OnStart() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnRender(float dt) override;
	virtual void OnClose() override;

private:
	std::shared_ptr<Prime::VertexShader> m_textureVS;
	std::shared_ptr<Prime::PixelShader>  m_texturePS;

	std::shared_ptr<Prime::VertexBuffer> m_quadVB;
	std::shared_ptr<Prime::IndexBuffer>  m_quadIB;

	std::shared_ptr<Prime::Texture2D>    m_texture;

	std::shared_ptr<Prime::ConstantBuffer<Prime::WVPBuffer>> m_cameraCBuffer;
	std::shared_ptr<Prime::ConstantBuffer<Prime::ColorBuffer>> m_pixelCBuffer;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float scaleCube = 0.5f;

	Prime::OrthographicCamera m_orthoCam;
};