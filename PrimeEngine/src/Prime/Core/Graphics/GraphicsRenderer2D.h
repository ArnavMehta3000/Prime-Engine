#pragma once
#include "Prime/Core/Graphics/GraphicsRenderer.h"

namespace Prime
{
	class GraphicsRenderer2D : public GraphicsRenderer
	{
	public:
		GraphicsRenderer2D();
		~GraphicsRenderer2D();

		void Init2D(ID3D11Device* device, ID3D11DeviceContext* context);

		inline Color GetPrimitivesColor() { return m_primitiveColor; }
		inline void SetPrimitivesColor(const Color& col) { m_primitiveColor = col; }

		void DrawQuad();

	private:
		void InitQuad();

	private:
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;

		std::shared_ptr<VertexBuffer> m_quadVB;
		std::shared_ptr<IndexBuffer> m_quadIB;

		std::shared_ptr<VertexShader> m_primitivesVS;
		std::shared_ptr<PixelShader> m_primitivesPS;

		Color m_primitiveColor;
	};
}

