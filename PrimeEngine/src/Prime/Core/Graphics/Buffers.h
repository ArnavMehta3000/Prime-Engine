#pragma once

namespace Prime
{
	enum class DataBufferType
	{
		VertexBuffer,
		IndexBuffer,
		ConstantBuffer,
	};

	enum class DataBufferUsage
	{
		Normal,
		Immutable,
		Dynamic,
		Staging,
	};

	enum class DataBufferCPUAccess
	{
		None,
		CPURead,
		CPUWrite
	};

	struct DataBufferDesc
	{
		DataBufferType Type;
		DataBufferUsage Usage;
		DataBufferCPUAccess CPUAccess = DataBufferCPUAccess::None;
	};

	class DataBuffer
	{
		friend class GraphicsEngine;
	public:
		DataBuffer();
		~DataBuffer();

		inline const ID3D11Buffer*        Get() const { return m_buffer.Get(); }
		inline const ComPtr<ID3D11Buffer> GetCOM() const { return m_buffer; }

		inline const UINT*                GetStride() const { return m_stride; }
		inline const UINT*                GetOffset() const { return m_offset; }

	private:
		const char* m_bufferType;
		UINT* m_stride;
		UINT* m_offset;
		ComPtr<ID3D11Buffer> m_buffer;
	};
}
