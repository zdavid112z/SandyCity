#include "ducky_pch.h"
#include "simpleLayer3d.h"

namespace ducky { namespace graphics {

	SimpleLayer3D::SimpleLayer3D(Shader* shader) :
		m_Shader(shader)
	{
		BufferDesc bd;
		bd.data = nullptr;
		bd.dynamic = true;
		bd.size = sizeof(MVPUniformBuffer);
		bd.type = BUFFER_UNIFORM;
		m_ShaderSystemBuffer = new Buffer(bd);
	}

	SimpleLayer3D::~SimpleLayer3D()
	{

	}

	void SimpleLayer3D::Render(LightSetup* ls)
	{
		m_Shader->Bind();
		if(ls != nullptr)
			ls->Bind(m_Shader);
		for (uint i = 0; i < m_DrawCalls.size(); i++)
		{
			const SimpleDrawCall& dc = m_DrawCalls[i];

			if ((dc.material->GetFlags() & MATERIAL_DISABLE_DEPTH_TEST) != 0)
				glDisable(GL_DEPTH_TEST);
			if ((dc.material->GetFlags() & MATERIAL_TRANSPARENCY) != 0)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			if ((dc.material->GetFlags() & MATERIAL_ADDITIVE_BLENDING) != 0)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			}

			m_UniformBuffer.mvp = dc.camera->GetViewProjectionMatrix() * dc.transform;
			m_UniformBuffer.model = dc.transform;
			m_UniformBuffer.camPosition = glm::vec4(dc.camera->GetPosition(), 1.0f);
			m_ShaderSystemBuffer->SetData(&m_UniformBuffer, 0, sizeof(MVPUniformBuffer));
			m_Shader->SetUniformBuffer("MVPUniformBuffer", m_ShaderSystemBuffer);
			if (dc.userdata != nullptr)
				m_Shader->SetUniformBuffer(dc.userdatalocation, dc.userdata);
			dc.material->Bind(m_Shader);
			dc.mesh->Render();
			dc.material->Unbind(m_Shader);

			if ((dc.material->GetFlags() & MATERIAL_DISABLE_DEPTH_TEST) != 0)
				glEnable(GL_DEPTH_TEST);
			if ((dc.material->GetFlags() & MATERIAL_TRANSPARENCY) != 0)
				glDisable(GL_BLEND);
			if ((dc.material->GetFlags() & MATERIAL_ADDITIVE_BLENDING) != 0)
				glDisable(GL_BLEND);
		}
		m_DrawCalls.clear();
	}

} }