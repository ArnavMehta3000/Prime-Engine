#include <pch.h>
#include "GraphicsRenderer.h"
#include "Prime/Core/Graphics/Buffers.h"
#include "Prime/Core/Graphics/Shaders.h"

namespace Prime
{
	GraphicsRenderer::GraphicsRenderer()
	{
		m_context = nullptr;
		
	}
	GraphicsRenderer::~GraphicsRenderer()
	{
	}




	void GraphicsRenderer::Init(ID3D11DeviceContext* context)
	{
		m_context = context;
		
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

	
	
	void GraphicsRenderer::Draw(UINT vertexCount, UINT startLocation)
	{
		m_context->Draw(vertexCount, startLocation);
	}
	void GraphicsRenderer::DrawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		m_context->DrawIndexed(indexBuffer->GetCount(), 0u, 0u);
	}
}