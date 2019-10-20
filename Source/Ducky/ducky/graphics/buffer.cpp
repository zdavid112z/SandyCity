#include "ducky_pch.h"
#include "buffer.h"

namespace ducky { namespace graphics {

	Buffer::Buffer(const BufferDesc& desc)
	{
		m_Desc = desc;
		glGenBuffers(1, &m_Buffer);
		glBindBuffer(desc.type, m_Buffer);
		glBufferData(desc.type, desc.size, desc.data, desc.dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}

	Buffer::~Buffer()
	{
		glDeleteBuffers(1, &m_Buffer);
	}

	void Buffer::SetData(void* v, uint32 offset = 0, uint32 size = 0)
	{
		if (size == 0)
			size = m_Desc.size;
		glBindBuffer(m_Desc.type, m_Buffer);
		glBufferSubData(m_Desc.type, offset, size, v);
	}

	void* Buffer::Map(MapType type)
	{
#ifndef DUCKY_GLES
		glBindBuffer(m_Desc.type, m_Buffer);
		return glMapBuffer(m_Desc.type, type);
#else
		return nullptr;
#endif
	}

	void Buffer::Unmap()
	{
#ifndef DUCKY_GLES
		glUnmapBuffer(m_Desc.type);
#endif
	}

	const BufferDesc& Buffer::GetDesc()
	{
		return m_Desc;
	}

	void Buffer::Bind()
	{
		glBindBuffer(m_Desc.type, m_Buffer);
	}

	void Buffer::Unbind()
	{
		glBindBuffer(m_Desc.type, 0);
	}

	GLuint Buffer::GetBuffer()
	{
		return m_Buffer;
	}

} }