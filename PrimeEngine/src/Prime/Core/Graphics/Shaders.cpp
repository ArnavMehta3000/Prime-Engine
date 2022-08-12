#include "pch.h"
#include "Shaders.h"

namespace Prime
{
	//------------------------ Vertex Shader ------------------------
	ComPtr<ID3D11VertexShader>& VertexShader::GetShader()
	{
		return m_shader;
	}
	
	ComPtr<ID3D10Blob>& VertexShader::GetBlob()
	{
		return m_blob;
	}
	
	ComPtr<ID3D11InputLayout>& VertexShader::GetInputLayout()
	{
		return m_inputLayout;
	}
	//------------------------ Vertex Shader ------------------------


	
	//------------------------ Pixel Shader ------------------------
	ComPtr<ID3D11PixelShader>& PixelShader::GetShader()
	{
		return m_shader;
	}
	ComPtr<ID3D10Blob>& PixelShader::GetBlob()
	{
		return m_blob;
	}
	//------------------------ Pixel Shader ------------------------
}
