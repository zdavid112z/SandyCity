#include "ducky_pch.h"
#include "light.h"
#include "shader.h"

namespace ducky { namespace graphics {

	LightSetup::LightSetup(bool dynamic)
	{
		m_Dynamic = dynamic;
		BufferDesc bd;
		bd.data = nullptr;
		bd.dynamic = dynamic;
		bd.size = sizeof(LightUniformBuffer);
		bd.type = BUFFER_UNIFORM;
		m_Buffer = new Buffer(bd);
	}

	LightSetup::~LightSetup()
	{
		delete m_Buffer;
	}

	void LightSetup::UpdateBuffer()
	{
		if (m_Dynamic)
			ForceUpdateBuffer();
	}

	void LightSetup::ForceUpdateBuffer()
	{
		m_BufferData.numDirLights = m_DirLights.size();
		for (uint i = 0; i < m_DirLights.size(); i++)
			m_BufferData.dirLights[i] = m_DirLights[i];

		m_BufferData.numSpotLights = m_SpotLights.size();
		for (uint i = 0; i < m_SpotLights.size(); i++)
			m_BufferData.spotLights[i] = m_SpotLights[i];

		m_BufferData.numPointLights = m_PointLights.size();
		for (uint i = 0; i < m_PointLights.size(); i++)
			m_BufferData.pointLights[i] = m_PointLights[i];

		m_Buffer->SetData(&m_BufferData, 0, sizeof(LightUniformBuffer));
	}

	void LightSetup::Bind(Shader* s)
	{
		s->SetUniformBuffer("LightUniformBuffer", m_Buffer);
	}

} }