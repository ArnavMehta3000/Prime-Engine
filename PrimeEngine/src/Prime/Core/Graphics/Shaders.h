#pragma once

namespace Prime
{
	struct ShaderCompileDesc
	{
		LPCWSTR SourceFile;
		LPCSTR EntryPoint;
		LPCSTR Profile;
	};

	//------------------- Vertex Shader -------------------
	class VertexShader
	{
	public:
		struct VSCompileDesc : public ShaderCompileDesc
		{
			D3D11_INPUT_ELEMENT_DESC* InputDesc;
			UINT NumElements;
		};

	public:
		VertexShader() {}
		~VertexShader() {}

		void Release()
		{
			m_blob->Release();
			m_shader->Release();
			m_inputLayout->Release();
		}

		ComPtr<ID3D11VertexShader>& GetShader();
		ComPtr<ID3D10Blob>& GetBlob();
		ComPtr<ID3D11InputLayout>& GetInputLayout();
		

	private:
		ComPtr<ID3D11VertexShader> m_shader;
		ComPtr<ID3D10Blob>         m_blob;
		ComPtr<ID3D11InputLayout>  m_inputLayout;
	};
	//------------------- Vertex Shader -------------------



	//------------------- Pixel Shader --------------------
	class PixelShader
	{
	public:
		PixelShader() {}
		~PixelShader() {}

		void Release()
		{
			m_blob->Release();
			m_shader->Release();
		}
		
		ComPtr<ID3D11PixelShader>& GetShader();
		ComPtr<ID3D10Blob>& GetBlob();
		
	private:
		ComPtr<ID3D11PixelShader> m_shader;
		ComPtr<ID3D10Blob>        m_blob;
	};
	//------------------- Pixel Shader --------------------
}