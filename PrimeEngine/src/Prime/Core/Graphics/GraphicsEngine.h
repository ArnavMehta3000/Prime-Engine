#pragma once
#include "D3D.h"
#include "Prime/Core/ServiceLocator/IService.h"
#include "Prime/Core/Graphics/Buffers.h"


namespace Prime
{

	class GraphicsEngine : public IService
	{
	public:
		class Factory
		{
		public:
			Factory();
			~Factory();

			void Init(ID3D11Device* device, ID3D11DeviceContext* context);
			
			void const CreateBuffer(DataBufferDesc desc, const void* data, UINT dataSize, UINT typeSize, std::unique_ptr<DataBuffer>& bufferPtr) const;
			void const CreateBuffer(DataBufferDesc desc, const void* data, UINT dataSize, UINT typeSize, std::shared_ptr<DataBuffer>& bufferPtr) const;


		private:
			ID3D11Device*        m_device;
			ID3D11DeviceContext* m_context;
		};

	public:
		GraphicsEngine();
		~GraphicsEngine();

		inline const ComPtr<ID3D11Device>        GetDevice()  const { return m_d3d->m_device; }
		inline const ComPtr<ID3D11DeviceContext> GetContext() const { return m_d3d->m_context; }
		inline const Factory*                    GetFactory() const { return m_factory.get(); }

		void Init(D3D_INIT_PARAMS d3dInit);
		void Shutdown();
		void BeginFrame();
		void EndFrame();

		void SetWireframe(bool isWireframe);

		

	private:
		std::unique_ptr<D3D> m_d3d;
		std::unique_ptr<GraphicsEngine::Factory> m_factory;

		ComPtr<ID3D11DepthStencilState> m_depthStencilState;
		ComPtr<ID3D11DepthStencilView>  m_depthStencilView;
		ComPtr<ID3D11RasterizerState>   m_rasterStateSolid;
		ComPtr<ID3D11RasterizerState>   m_rasterStateWireframe;
	};
}

