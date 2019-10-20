#pragma once

#include "ducky_pch.h"
#include <glm\glm.hpp>
#include "opengl.h"
#include "io\modelio.h"

namespace ducky { namespace graphics {

	class Buffer;

	class Mesh
	{
	public:
		Mesh(const io::ModelData&);
		Mesh(const io::ModelData&, Buffer* ibo, uint numIndices);
		~Mesh();
		void Render();
		void RenderStrips(uint32 restart = 0, uint32 count = 0);
	protected:
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_IBO;
		uint32 m_Count;
		bool m_HasIBO;
	};

} }