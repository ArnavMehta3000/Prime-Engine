#include "pch.h"
#include "GraphicsFactory.h"
#include "Prime/Logger.h"
#include <atlstr.h>

namespace Prime
{
	GraphicsFactory::GraphicsFactory()
	{
		m_device = nullptr;
	}

	void GraphicsFactory::Init(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		m_device = device;
		m_context = context;
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
		
		THROW_HR(m_device->CreateBuffer(&vbd, &vsd, buffer->GetCOM().GetAddressOf()), "Failed to create Vertex Buffer");

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

	Texture2D* GraphicsFactory::CreateTextureFromFile(LPCWSTR filepath, D3D11_USAGE usage, UINT bindFlags, UINT cpuAccessFlags, UINT miscFlags, WIC_LOADER_FLAGS loadFlags = WIC_LOADER_DEFAULT)
	{
		Texture2D* texture = new Texture2D;
		ComPtr<ID3D11Resource> resource;
		HRESULT hr = CreateWICTextureFromFileEx(
			m_device, m_context,
			filepath, 0,
			usage, bindFlags, cpuAccessFlags, miscFlags, loadFlags,
			resource.GetAddressOf(), texture->m_resourceView.GetAddressOf()
		);
		THROW_HR(hr, "Failed to create Texture2D");
		texture->Init(resource);

		resource->Release();
		resource = nullptr;

		auto path = CW2A(filepath);
		TRACE("Created texture from file: " << path.m_psz);
		return texture;
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
		
		auto path = CW2A(filepath);
		TRACE("Created Vertex Shader from file: " << path.m_psz);
		return vs;
	}

	PixelShader* GraphicsFactory::CreatePixelShader(LPCWSTR filepath)
	{
		PixelShader* ps = new PixelShader;
		
		THROW_HR(D3DReadFileToBlob(filepath, ps->GetBlob().GetAddressOf()),
			"Failed to read pixel shader to blob");

		THROW_HR(m_device->CreatePixelShader(ps->GetBlob()->GetBufferPointer(), ps->GetBlob()->GetBufferSize(), nullptr, ps->GetShader().GetAddressOf()),
			"Failed to create pixel shader object");
		
		auto path = CW2A(filepath);
		TRACE("Created Pixel Shader from file: " << path.m_psz);
		return ps;
	}

}
