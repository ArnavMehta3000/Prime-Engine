#pragma once

namespace Prime
{
	class Texture2D
	{
		friend class GraphicsFactory;
	public:
		Texture2D();
		~Texture2D();

		void Init(const ComPtr<ID3D11Resource>& resource);
		void Release();

		ComPtr<ID3D11ShaderResourceView>& GetResourceView();
		ComPtr<ID3D11Texture2D>& GettextureObj();
		const D3D11_TEXTURE2D_DESC& GetDesc();

	private:
		ComPtr<ID3D11ShaderResourceView> m_resourceView;
		ComPtr<ID3D11Texture2D> m_texture;
		D3D11_TEXTURE2D_DESC m_desc;
	};	
}