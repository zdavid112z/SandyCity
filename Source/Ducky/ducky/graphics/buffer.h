#pragma once

#include "ducky_pch.h"
#include "opengl.h"

namespace ducky { namespace graphics {

	enum BufferType
	{
		BUFFER_UNIFORM = GL_UNIFORM_BUFFER,
		BUFFER_VERTEX = GL_ARRAY_BUFFER,
		BUFFER_INDEX = GL_ELEMENT_ARRAY_BUFFER
	};


	struct BufferDesc
	{
		BufferType type;
		uint32 size;
		bool dynamic;
		void* data;
	};

	enum MapType
	{
#ifndef DUCKY_GLES
		MAP_READ = GL_READ_ONLY,
		MAP_WRITE = GL_WRITE_ONLY,
		MAP_READ_WRITE = GL_READ_WRITE
#else
		MAP_READ,
		MAP_WRITE,
		MAP_READ_WRITE
#endif
	};

	class Buffer
	{
	public:
		Buffer(const BufferDesc&);
		~Buffer();
		void SetData(void*, uint32 offset, uint32 size);
		void* Map(MapType);
		void Unmap();
		const BufferDesc& GetDesc();
		void Bind();
		void Unbind();
		GLuint GetBuffer();
	protected:
		GLuint m_Buffer;
		BufferDesc m_Desc;
	};

} }