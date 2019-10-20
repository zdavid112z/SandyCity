#pragma once

#include "ducky_pch.h"
#include "opengl.h"
#include <glm\glm.hpp>
#include "buffer.h"

namespace ducky { namespace graphics {

	class Shader;

	struct GPUDirectionalLight
	{
		glm::vec3 direction;
		float intensity;
		glm::vec3 color;
		float padding1;
	};

	struct GPUPointLight
	{
		glm::vec3 color;
		float intensity;
		glm::vec3 attenuations;
		float padding1;
		glm::vec3 position;
		float padding2;
	};

	struct GPUSpotLight
	{
		glm::vec3 color;
		float intensity;
		glm::vec3 attenuations;
		float padding1;
		glm::vec3 position;
		float padding2;
		glm::vec3 direction;
		float cosine;
	};

	struct LightUniformBuffer
	{
		GPUDirectionalLight dirLights[4];
		GPUPointLight pointLights[32];
		GPUSpotLight spotLights[64];
		int numDirLights;
		int numPointLights;
		int numSpotLights;
		float padding1;
		glm::vec3 ambient;
		float padding2;
	};

	struct DirectionalLight : GPUDirectionalLight
	{
		string name;
	};

	struct PointLight : GPUPointLight
	{
		string name;
	};

	struct SpotLight : GPUSpotLight
	{
		string name;
	};

	class LightSetup
	{
	public:
		LightSetup(bool dynamic);
		~LightSetup();

		void AddDirLight(const DirectionalLight& d) { m_DirLights.push_back(d); }
		void RemoveDirLight(const string& name)
		{
			for (uint i = 0; i < m_DirLights.size(); i++)
			{
				if (m_DirLights[i].name == name)
				{
					m_DirLights.erase(m_DirLights.begin() + i);
					return;
				}
			}
		}
		DirectionalLight* GetDirLight(const string& name)
		{
			for (uint i = 0; i < m_DirLights.size(); i++)
			{
				if (m_DirLights[i].name == name)
				{
					return &m_DirLights[i];
				}
			}
			return nullptr;
		}

		void AddSpotLight(const SpotLight& d) { m_SpotLights.push_back(d); }
		void RemoveSpotLight(const string& name)
		{
			for (uint i = 0; i < m_SpotLights.size(); i++)
			{
				if (m_SpotLights[i].name == name)
				{
					m_SpotLights.erase(m_SpotLights.begin() + i);
					return;
				}
			}
		}
		SpotLight* GetSpotLight(const string& name)
		{
			for (uint i = 0; i < m_SpotLights.size(); i++)
			{
				if (m_SpotLights[i].name == name)
				{
					return &m_SpotLights[i];
				}
			}
			return nullptr;
		}

		void AddPointLight(const PointLight& d) { m_PointLights.push_back(d); }
		void RemovePointLight(const string& name)
		{
			for (uint i = 0; i < m_PointLights.size(); i++)
			{
				if (m_PointLights[i].name == name)
				{
					m_PointLights.erase(m_PointLights.begin() + i);
					return;
				}
			}
		}
		PointLight* GetPointLight(const string& name)
		{
			for (uint i = 0; i < m_PointLights.size(); i++)
			{
				if (m_PointLights[i].name == name)
				{
					return &m_PointLights[i];
				}
			}
			return nullptr;
		}

		glm::vec3& GetAmbient() { return m_BufferData.ambient; }

		void UpdateBuffer();
		void ForceUpdateBuffer();
		void Bind(Shader* s);
	private:
		bool m_Dynamic;
		vector<DirectionalLight> m_DirLights;
		vector<PointLight> m_PointLights;
		vector<SpotLight> m_SpotLights;
		LightUniformBuffer m_BufferData;
		Buffer* m_Buffer;
	};

} }