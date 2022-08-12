#include "pch.h"
#include "GraphicsFactory.h"
#include "Prime/Logger.h"

namespace Prime
{
	GraphicsFactory::GraphicsFactory()
	{
		m_device = nullptr;
	}

	void GraphicsFactory::Init(ID3D11Device* device)
	{
		m_device = device;
	}

	

	VertexBuffer* GraphicsFactory::CreateVertexBuffer(const void* data, UINT dataTypeSize, UINT numVertices)
	{
		VertexBuffer* buffer = new VertexBuffer;
		buffer->m_bufferSize = numVertices;
		buffer->m_stride = std::make_unique<UINT>(dataTypeSize);

		D3D11_BUFFER_DESC vbd{};
		ZeroMemory(&vbd, sizeof(D3D11_BUFFER_DESC));
		vbd.Usage          = D3D11_USAGE_DEFAULT;
		vbd.ByteWidth      = dataTypeSize * numVertices;
		vbd.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA vsd{};
		ZeroMemory(&vsd, sizeof(D3D11_SUBRESOURCE_DATA));
		vsd.pSysMem = data;
		
		THROW_HR(m_device->CreateBuffer(&vbd, &vsd, buffer->m_buffer.GetAddressOf()), "Failed to create Vertex Buffer");

		TRACE("Created Vertex Buffer");
		return buffer;
	}

	IndexBuffer* GraphicsFactory::CreateIndexBuffer(const DWORD* data, UINT numIndices)
	{
		IndexBuffer* buffer = new IndexBuffer;
		buffer->m_indexCount = numIndices;
		
		D3D11_BUFFER_DESC ibd{};
		ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
		ibd.Usage          = D3D11_USAGE_DEFAULT;
		ibd.ByteWidth      = sizeof(DWORD) * numIndices;
		ibd.BindFlags      = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags      = 0;

		D3D11_SUBRESOURCE_DATA isd{};
		ZeroMemory(&isd, sizeof(D3D11_SUBRESOURCE_DATA));
		isd.pSysMem = data;
		
		THROW_HR(m_device->CreateBuffer(&ibd, &isd, buffer->m_buffer.GetAddressOf()), "Failed to create Index Buffer");
		
		TRACE("Created Index Buffer");
		return buffer;
	}

	VertexShader* GraphicsFactory::CreateVertexShader(LPCWSTR filepath, D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements)
	{
		VertexShader* vs = new VertexShader;
		
		THROW_HR(D3DReadFileToBlob(filepath, vs->GetBlob().GetAddressOf()),
			"Failed to read vertex shader to blob");
		
		THROW_HR(m_device->CreateVertexShader(vs->GetBlob()->GetBufferPointer(), vs->GetBlob()->GetBufferSize(), nullptr, vs->GetShader().GetAddressOf()),
			"Failed to create vertex shader object");

		THROW_HR(m_device->CreateInputLayout(desc,
			numElements,
			vs->GetBlob()->GetBufferPointer(),
			vs->GetBlob()->GetBufferSize(),
			vs->GetInputLayout().GetAddressOf()),
			"Failed to create input layout");
		
		return vs;
	}

}
