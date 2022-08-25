#include <pch.h>
#include "GraphicsRenderer.h"
#include "Prime/Core/Graphics/Buffers.h"
#include "Prime/Core/Graphics/Shaders.h"
#include "Prime/Core/Texture2D.h"

namespace Prime
{
	ComPtr<ID3D11SamplerState> GraphicsRenderer::s_samplerLinearWrap = nullptr;

	GraphicsRenderer::GraphicsRenderer()
	{
		m_context = nullptr;
		m_device = nullptr;
	}
	GraphicsRenderer::~GraphicsRenderer()
	{
	}




	void GraphicsRenderer::Init(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		m_context = context;
		m_device = device;

		// Create default samplers
		D3D11_SAMPLER_DESC sampDesc{};
		ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		THROW_HR(m_device->CreateSamplerState(&sampDesc, s_samplerLinearWrap.GetAddressOf()),
			"Failed to create sampler state");
	}
	
	void GraphicsRenderer::Bind(D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		m_context->IASetPrimitiveTopology(topology);
	}


	
	void GraphicsRenderer::Bind(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		UINT offset = 0u;
		m_context->IASetVertexBuffers(0u, 1u, vertexBuffer->GetCOM().GetAddressOf(), vertexBuffer->StridePtr(), &offset);
	}

	void GraphicsRenderer::Bind(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		UINT offset = 0u;
		m_context->IASetIndexBuffer(indexBuffer->GetCOM().Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	

	void GraphicsRenderer::Bind(const std::shared_ptr<VertexShader>& vertexShader)
	{
		m_context->VSSetShader(vertexShader->GetShader().Get(), nullptr, 0u);
		m_context->IASetInputLayout(vertexShader->GetInputLayout().Get());
	}

	void GraphicsRenderer::Bind(const std::shared_ptr<PixelShader>& pixelShader)
	{
		m_context->PSSetShader(pixelShader->GetShader().Get(), nullptr, 0u);
	}

	
	void GraphicsRenderer::Bind(ShaderType shaderType, const std::shared_ptr<Texture2D>& texture)
	{
		switch (shaderType)
		{
		case Prime::ShaderType::VertexShader:
			m_context->VSSetShaderResources(0, 1, texture->GetResourceView().GetAddressOf());
			return;
		case Prime::ShaderType::PixelShader:
			m_context->PSSetShaderResources(0, 1, texture->GetResourceView().GetAddressOf());
			return;
		}
	}

	void GraphicsRenderer::Bind(ShaderType shaderType, const ComPtr<ID3D11SamplerState> sampler)
	{
		switch (shaderType)
		{
		case Prime::ShaderType::VertexShader:
			m_context->VSSetSamplers(0, 1, sampler.GetAddressOf());
			return;
		case Prime::ShaderType::PixelShader:
			m_context->PSSetSamplers(0, 1, sampler.GetAddressOf());
			return;
		}
	}

	
	void GraphicsRenderer::Draw(UINT vertexCount, UINT startLocation)
	{
		m_context->Draw(vertexCount, startLocation);
	}
	void GraphicsRenderer::DrawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		m_context->DrawIndexed(indexBuffer->GetCount(), 0, 0);
	}
}