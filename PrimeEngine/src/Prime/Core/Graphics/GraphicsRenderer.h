#pragma once
#include "Prime/Core/ServiceLocator/IService.h"
#include "Prime/Core/Graphics/Buffers.h"

namespace Prime
{
	// TODO: Forward declare all submitable classes
	class VertexShader;
	class PixelShader;
	class VertexBuffer;
	class IndexBuffer;
	
	enum class ShaderType
	{
		VertexShader, PixelShader, 
	};

	class GraphicsRenderer : public IService
	{
	public:
		GraphicsRenderer();
		~GraphicsRenderer();
		
		void Init(ID3D11DeviceContext* context);

		void Bind(D3D11_PRIMITIVE_TOPOLOGY topology);
	
		void Bind(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void Bind(const std::shared_ptr<IndexBuffer>& indexBuffer);

		void Bind(const std::shared_ptr<VertexShader>& vertexShader);
		void Bind(const std::shared_ptr<PixelShader>& pixelShader);

		template <typename T>
		void Bind(ShaderType shaderType, const std::shared_ptr<ConstantBuffer<T>>& constantBuffer)
		{
			switch (shaderType)
			{
			case Prime::ShaderType::VertexShader:
				m_context->VSSetConstantBuffers(0, 1, constantBuffer->GetCOM().GetAddressOf());
				return;
			case Prime::ShaderType::PixelShader:
				m_context->PSSetConstantBuffers(0, 1, constantBuffer->GetCOM().GetAddressOf());
				return;
			}
		}

		template <typename T>
		bool UpdateConstantBuffer(const std::shared_ptr<ConstantBuffer<T>>& cb)
		{
			D3D11_MAPPED_SUBRESOURCE map{};
			HRESULT hr = m_context->Map(cb->GetCOM().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
			if (FAILED(hr))
			{
				std::cerr << "Failed to map subresource data" << std::endl;
				return false;
			}
			
			CopyMemory(map.pData, &cb->Data, sizeof(T));
			m_context->Unmap(cb->GetCOM().Get(), 0);

			return true;
		}


		void Draw(UINT vertexCount, UINT startLocation);
		void DrawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer);
		

	private:
		ID3D11DeviceContext* m_context;
	};
}