#include "pch.h"
#include "Buffers.h"
#include "Prime/Core/ServiceLocator/Locator.h"
#include "Prime/Core/Graphics/GraphicsEngine.h"

namespace Prime
{
	DataBuffer::DataBuffer()
	{
		m_buffer = nullptr;
		m_bufferType = "None";
		m_stride = new UINT;
		m_offset = new UINT;
	}

	DataBuffer::~DataBuffer()
	{
		m_buffer->Release();
	}
}
