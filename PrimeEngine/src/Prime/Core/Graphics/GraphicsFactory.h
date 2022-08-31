#pragma once
#include "WICTextureLoader.h"
#include "Prime/Core/Graphics/D3D.h"
#include "Prime/Core/Graphics/Buffers.h"
#include "Prime/Core/Graphics/Shaders.h"
#include "Prime/Core/Texture2D.h"
#include "Prime/Core/ServiceLocator/IService.h"



namespace Prime
{
	class GraphicsFactory : public IService
	{
	public:
		GraphicsFactory();
		~GraphicsFactory() {}

		void Init(ID3D11Device* device, ID3D11DeviceContext* context);

		
		VertexBuffer* CreateVertexBuffer(const void* data, UINT dataTypeSize, UINT numVertices);
		IndexBuffer* CreateIndexBuffer(const DWORD* data, UINT numIndices);
		
		Texture2D* CreateTextureFromFile(LPCWSTR filepath, D3D11_USAGE usage, UINT bindFlags, UINT cpuAccessFlags, UINT miscFlags, WIC_LOADER_FLAGS loadFlags);

		template <typename T>
		ConstantBuffer<T>* CreateConstantBuffer()
		{
			ConstantBuffer<T>* cBuffer = new ConstantBuffer<T>;

			D3D11_BUFFER_DESC desc{};
			ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
			desc.Usage               = D3D11_USAGE_DYNAMIC;
			desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags           = 0;
			desc.ByteWidth           = static_cast<UINT>(sizeof(T)) + (16 - sizeof(T) % 16);  // For 16 yte alignment
			desc.StructureByteStride = 0;

			HRESULT hr = this->m_device->CreateBuffer(&desc, 0, cBuffer->GetCOM().GetAddressOf());
			if (FAILED(hr))
			{
				std::cerr << "Failed to create constant buffer" << std::endl;
			}

			return cBuffer;
		}
		
		VertexShader* CreateVertexShader(LPCWSTR filepath, D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements);
		PixelShader* CreatePixelShader(LPCWSTR filepath);

	private:
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
	};
}

