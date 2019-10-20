#pragma once

#include "../material.h"
#include "../textureCube.h"

namespace ducky { namespace graphics {

	struct PBRUniformBuffer
	{
		glm::vec3 albedoColor;
		float usingAlbedoMap = 1;
		glm::vec3 specularColor;
		float usingSpecularMap = 1;
		float usingGlossMap = 1;
		float glossColor;
		float padding1;
		float padding2;
	};

	class PBRMaterial : public Material
	{
	public:
		PBRMaterial(const string& name) :
			Material(name)
		{
			BufferDesc bd;
			bd.data = nullptr;
			bd.dynamic = false;
			bd.size = sizeof(PBRUniformBuffer);
			bd.type = BUFFER_UNIFORM;
			m_Buffer = new Buffer(bd);
		}

		virtual ~PBRMaterial()
		{
			delete m_Buffer;
		}

		void UpdateBuffer()
		{
			m_Buffer->SetData(&m_BufferData, 0, sizeof(PBRUniformBuffer));
		}

		void Bind(Shader* s) override
		{
			s->SetUniformBuffer("MaterialUniformBuffer", m_Buffer);
			m_AlbedoMap->Bind(0);
			m_SpecularMap->Bind(1);
			m_GlossMap->Bind(2);
			m_NormalMap->Bind(3);
			m_PreintegratedFG->Bind(4);
			m_EnvironmentMap->Bind(5);
		}

		void Unbind(Shader* s) override
		{
			Texture2D::Unbind(0);
			Texture2D::Unbind(1);
			Texture2D::Unbind(2);
			Texture2D::Unbind(3);
			TextureCube::Unbind(4);
		}

		void SetAlbedoColor(const glm::vec3& color, float useAlbedoColor) { m_BufferData.albedoColor = color; m_BufferData.usingAlbedoMap = 1.0f - useAlbedoColor; }
		void SetSpecularColor(const glm::vec3& color, float useSpecularColor) { m_BufferData.specularColor = color; m_BufferData.usingSpecularMap = 1.0f - useSpecularColor; }
		void SetGlossColor(float color, float useGlossColor) { m_BufferData.glossColor = color; m_BufferData.usingGlossMap = 1.0f - useGlossColor; }

		void SetPreintegratedFG(Texture2D* t) { m_PreintegratedFG = t; }
		void SetAlbedoMap(Texture2D* t) { m_AlbedoMap = t; }
		void SetSpecularMap(Texture2D* t) { m_SpecularMap = t; }
		void SetGlossMap(Texture2D* t) { m_GlossMap = t; }
		void SetNormalMap(Texture2D* t) { m_NormalMap = t; }
		void SetEnvironmentMap(TextureCube* t) { m_EnvironmentMap = t; }
	protected:
		Texture2D* m_AlbedoMap;
		Texture2D* m_SpecularMap;
		Texture2D* m_GlossMap;
		Texture2D* m_NormalMap;
		Texture2D* m_PreintegratedFG;
		TextureCube* m_EnvironmentMap;
		Buffer* m_Buffer;
		PBRUniformBuffer m_BufferData;
	};
	
} }