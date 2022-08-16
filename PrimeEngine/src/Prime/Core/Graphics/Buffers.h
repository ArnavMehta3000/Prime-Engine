#pragma once

namespace Prime
{
	class IBuffer
	{
		friend class GraphicsFactory;
	public:
		IBuffer() {}
		inline void Release()
		{
			m_buffer->Release();
		}
		ComPtr<ID3D11Buffer>& GetCOM() { return m_buffer; };
		
	protected:
		ComPtr<ID3D11Buffer> m_buffer;
	};

	

	class VertexBuffer : public IBuffer
	{
		friend class GraphicsFactory;
	public:
		VertexBuffer()
		{
			m_bufferSize = 0;
		}
		
		inline UINT  Stride()     const { return *m_stride.get(); }
		inline UINT* StridePtr()  const { return m_stride.get(); }
		inline UINT  BufferSize() const { return m_bufferSize; }
		
	private:
		UINT m_bufferSize;
		std::unique_ptr<UINT> m_stride;
	};


	
	class IndexBuffer : public IBuffer
	{
		friend class GraphicsFactory;
	public:
		IndexBuffer()
		{
			m_indexCount = 0;
		}
		
		inline UINT GetCount() const { return m_indexCount; }

	private:
		UINT m_indexCount;
	};
}
