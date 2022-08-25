#include <pch.h>
#include "Texture2D.h"

namespace Prime
{
	Texture2D::Texture2D()
	{
		m_desc = D3D11_TEXTURE2D_DESC{};
	}

	Texture2D::~Texture2D() {}

	void Texture2D::Init(const ComPtr<ID3D11Resource>& resource)
	{
		THROW_HR(resource.As(&m_texture), "Failed to convert ID3D11Resource to ID3D11Texture2D [INIT FAILED]");
		m_texture->GetDesc(&m_desc);
	}

	ComPtr<ID3D11ShaderResourceView>& Texture2D::GetResourceView()
	{
		return m_resourceView;
	}
	ComPtr<ID3D11Texture2D>& Texture2D::GettextureObj()
	{
		return m_texture;
	}
	const D3D11_TEXTURE2D_DESC& Texture2D::GetDesc()
	{
		return m_desc;
	}

	void Texture2D::Release()
	{
		m_texture->Release();
		m_resourceView->Release();
	}
}