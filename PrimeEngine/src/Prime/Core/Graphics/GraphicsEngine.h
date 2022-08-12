#pragma once
#include "Prime/Core/Graphics/GraphicsFactory.h"
#include "Prime/Core/Graphics/GraphicsRenderer.h"

namespace Prime
{

	class GraphicsEngine : public IService
	{
	public:
		GraphicsEngine();
		~GraphicsEngine();

		inline const ComPtr<ID3D11Device>        GetDevice()  const { return m_d3d->m_device; }
		inline const ComPtr<ID3D11DeviceContext> GetContext() const { return m_d3d->m_context; }

		void Init(D3D_INIT_PARAMS d3dInit);
		void Shutdown();
		void BeginFrame();
		void EndFrame();

		void SetWireframe(bool isWireframe);

		

	private:
		std::unique_ptr<D3D> m_d3d;

		ComPtr<ID3D11DepthStencilState> m_depthStencilState;
		ComPtr<ID3D11DepthStencilView>  m_depthStencilView;
		ComPtr<ID3D11RasterizerState>   m_rasterStateSolid;
		ComPtr<ID3D11RasterizerState>   m_rasterStateWireframe;
	};
}

