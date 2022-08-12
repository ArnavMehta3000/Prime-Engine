#pragma once
#include "Prime/Core/ServiceLocator/IService.h"

namespace Prime
{
	// TODO: Forward declare all submitable classes
	class VertexShader;
	class PixelShader;
	class VertexBuffer;
	class IndexBuffer;
	
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


		void Draw(UINT vertexCount, UINT startLocation);
		void DrawIndexed(const std::shared_ptr<IndexBuffer>& indexBuffer);
		

	private:
		ID3D11DeviceContext* m_context;
	};
}