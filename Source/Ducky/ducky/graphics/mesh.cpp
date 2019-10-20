#include "ducky_pch.h"
#include "mesh.h"
#include "buffer.h"

namespace ducky { namespace graphics {

	Mesh::Mesh(const io::ModelData& md)
	{
		m_HasIBO = md.numIndices != 0;
		if(m_HasIBO)
			m_Count = md.numIndices;
		else m_Count = md.numVertices;

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, md.numVertices * sizeof(io::Vertex), md.vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(io::Vertex), (const void*)(offsetof(io::Vertex, position)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(io::Vertex), (const void*)(offsetof(io::Vertex, texCoord)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(io::Vertex), (const void*)(offsetof(io::Vertex, normal)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(io::Vertex), (const void*)(offsetof(io::Vertex, tangent)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(io::Vertex), (const void*)(offsetof(io::Vertex, bitangent)));
		
		if (m_HasIBO)
		{
			glGenBuffers(1, &m_IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, md.numIndices * sizeof(uint32), md.indices, GL_STATIC_DRAW);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		if(m_HasIBO)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	Mesh::Mesh(const io::ModelData& md, Buffer* ibo, uint numindices)
	{
		m_HasIBO = true;
		m_Count = numindices;

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, md.numVertices * sizeof(io::Vertex), md.vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(io::Vertex), (const void*)(offsetof(io::Vertex, position)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(io::Vertex), (const void*)(offsetof(io::Vertex, texCoord)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(io::Vertex), (const void*)(offsetof(io::Vertex, normal)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(io::Vertex), (const void*)(offsetof(io::Vertex, tangent)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(io::Vertex), (const void*)(offsetof(io::Vertex, bitangent)));

		ibo->Bind();

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		ibo->Unbind();

		m_IBO = ibo->GetBuffer();
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_IBO);
	}

	void Mesh::Render()
	{
		glBindVertexArray(m_VAO);
		if (m_HasIBO)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
			glDrawElements(GL_TRIANGLES, m_Count, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, m_Count);
		}
		glBindVertexArray(0);
	}

	void Mesh::RenderStrips(uint32 restart, uint32 count)
	{
		if (count == 0)
			count = m_Count;
		glBindVertexArray(m_VAO);
		if (restart)
		{
#ifndef DUCKY_GLES
			glEnable(GL_PRIMITIVE_RESTART);
			glPrimitiveRestartIndex(restart);
#endif
		}
		if (m_HasIBO)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
			glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
		}
		if (restart)
		{
#ifndef DUCKY_GLES
			glDisable(GL_PRIMITIVE_RESTART);
#endif
		}
		glBindVertexArray(0);
	}

} }