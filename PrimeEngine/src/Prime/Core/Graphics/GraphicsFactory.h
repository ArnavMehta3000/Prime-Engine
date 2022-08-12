#pragma once
#include "Prime/Core/Graphics/D3D.h"
#include "Prime/Core/ServiceLocator/IService.h"
#include "Prime/Core/Graphics/Buffers.h"
#include "Prime/Core/Graphics/Shaders.h"


namespace Prime
{
	class GraphicsFactory : public IService
	{
	public:
		GraphicsFactory();
		~GraphicsFactory() {}

		void Init(ID3D11Device* device);

		VertexBuffer* CreateVertexBuffer(const void* data, UINT dataTypeSize, UINT numVertices);
		IndexBuffer* CreateIndexBuffer(const DWORD* data, UINT numIndices);
		
		VertexShader* CreateVertexShader(LPCWSTR filepath, D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements);

	private:
		ID3D11Device* m_device;
	};
}

