#include "pch.h"
#include "GraphicsFactory.h"
#include "Prime/Logger.h"

namespace Prime
{
	GraphicsFactory::GraphicsFactory()
	{
		m_device = nullptr;
		m_context = nullptr;
	}

	void GraphicsFactory::Init(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		m_device = device;
		m_context = context;
	}

	void const GraphicsFactory::CreateBuffer(DataBufferDesc desc, const void* data, UINT dataSize, UINT typeSize, std::unique_ptr<DataBuffer>& bufferPtr) const
	{
		bufferPtr = std::make_unique<DataBuffer>();
		D3D11_BUFFER_DESC bufferDesc{};
		D3D11_SUBRESOURCE_DATA subresourceData{};

		bufferDesc.MiscFlags = 0u;
		bufferDesc.ByteWidth = dataSize;
		subresourceData.pSysMem = data;
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		// Set buffer bind type
		switch (desc.Type)
		{
		case DataBufferType::VertexBuffer:
			bufferPtr->m_bufferType = "Vertex Buffer";
			bufferDesc.StructureByteStride = typeSize;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			break;

		case DataBufferType::IndexBuffer:
			bufferPtr->m_bufferType = "Index Buffer";
			bufferDesc.StructureByteStride = 0;
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			break;

		case DataBufferType::ConstantBuffer:
			bufferPtr->m_bufferType = "Constant Buffer";
			bufferDesc.StructureByteStride = typeSize;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			break;
		}

		// Set buffer usage type
		switch (desc.Usage)
		{
		case DataBufferUsage::Normal:
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			break;

		case DataBufferUsage::Immutable:
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			break;

		case DataBufferUsage::Dynamic:
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			break;

		case DataBufferUsage::Staging:
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			break;
		}

		// Set buffer cpu access
		switch (desc.CPUAccess)
		{
		case DataBufferCPUAccess::CPURead:
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			break;

		case DataBufferCPUAccess::CPUWrite:
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			break;

		case DataBufferCPUAccess::None:
		default:
			bufferDesc.CPUAccessFlags = 0u;
			break;
		}




		*bufferPtr->m_stride = typeSize;
		*bufferPtr->m_offset = 0;

		THROW_HR(m_device->CreateBuffer(&bufferDesc, &subresourceData, bufferPtr->m_buffer.GetAddressOf()),
			"Failed to create data buffer: Type[" << bufferPtr->m_bufferType << "]");

		TRACE("Created buffer: Type[" << bufferPtr->m_bufferType << "]");
	}

}
