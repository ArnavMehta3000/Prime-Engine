#pragma once
#include "D3D.h"
#include "Prime/Core/IService.h"


namespace Prime
{

	class GraphicsEngine : public IService
	{
	public:
		GraphicsEngine();
		~GraphicsEngine();

		PRIME_API void Init(D3D_INIT_PARAMS d3dInit);
		PRIME_API void Shutdown();
		PRIME_API void BeginFrame();
		PRIME_API void EndFrame();

		PRIME_API void SetWireframe(bool isWireframe);

	private:
		std::unique_ptr<D3D> m_d3d;

		ComPtr<ID3D11DepthStencilState> m_depthStencilState;
		ComPtr<ID3D11DepthStencilView>  m_depthStencilView;
		ComPtr<ID3D11RasterizerState>   m_rasterStateSolid;
		ComPtr<ID3D11RasterizerState>   m_rasterStateWireframe;
	};
}

