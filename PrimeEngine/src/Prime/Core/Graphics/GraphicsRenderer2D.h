#pragma once
#include "Prime/Core/Graphics/GraphicsRenderer.h"

namespace Prime
{
	struct PrimitivesFrameBufferVS;
	struct PrimitivesObjectBufferVS;

	class GraphicsRenderer2D : public GraphicsRenderer
	{
	public:
		GraphicsRenderer2D();
		~GraphicsRenderer2D();

		void Init2D(ID3D11Device* device, ID3D11DeviceContext* context);

		void BeginQuadBatch(const Matrix& view, const Matrix& proj);
		void DrawQuadBatch(const Matrix& world, const Color& col);

	private:
		void InitQuad();

	private:
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;

		ComPtr<ID3D11ShaderResourceView> m_whiteTexture;

		std::shared_ptr<VertexBuffer> m_quadVB;
		std::shared_ptr<IndexBuffer> m_quadIB;

		std::shared_ptr<VertexShader> m_primitiveVS;
		std::shared_ptr<PixelShader> m_primitivePS;

		std::shared_ptr<ConstantBuffer<PrimitivesFrameBufferVS>> cbFrame;
		std::shared_ptr<ConstantBuffer<PrimitivesObjectBufferVS>> cbObject;
	};
}

