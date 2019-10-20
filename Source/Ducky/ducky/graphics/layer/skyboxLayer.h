#pragma once

#include "../textureCube.h"
#include "../shader.h"
#include "layer.h"
#include "../mesh.h"
#include "../camera.h"

namespace ducky { namespace graphics {

	struct SkyboxUniformBuffer
	{
		glm::mat4 invViewMatrix;
		glm::mat4 invProjectionMatrix;
	};

	class SkyboxLayer : public Layer
	{
	public:
		SkyboxLayer();
		~SkyboxLayer();
		void Render(LightSetup*) override;
		void SetSkyboxTexture(TextureCube* c) { m_SkyboxTexture = c; }
		TextureCube* GetSkyboxTexture() { return m_SkyboxTexture; }
		void SetCamera(Camera* c) { m_Camera = c; }
		static string GetLayerName() { return "Skybox"; }
	private:
		Camera* m_Camera = nullptr;
		Mesh* m_Cube;
		Shader* m_Shader;
		Buffer* m_UniformBuffer;
		SkyboxUniformBuffer m_BufferData;
		TextureCube* m_SkyboxTexture = nullptr;
	};

} }