#pragma once
#include "Prime/Core/Graphics/D3D.h"
#include "Prime/Core/ServiceLocator/IService.h"
#include "Prime/Core/Graphics/Buffers.h"


namespace Prime
{
	class GraphicsFactory : public IService
	{
	public:
		GraphicsFactory();
		~GraphicsFactory() {}

		void Init(ID3D11Device* device, ID3D11DeviceContext* context);

		void const CreateBuffer(DataBufferDesc desc, const void* data, UINT dataSize, UINT typeSize, std::unique_ptr<DataBuffer>& bufferPtr) const;

	private:
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
	};
}

