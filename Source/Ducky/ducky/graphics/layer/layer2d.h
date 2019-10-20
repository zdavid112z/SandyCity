#pragma once

#include "../opengl.h"
#include "layer.h"
#include "graphics\buffer.h"
#include "graphics\texture2d.h"
#include "graphics\mesh.h"
#include "graphics\shader.h"
#include "io\fontio.h"
#include "graphics\camera.h"
#include "graphics\spriteSheet.h"

namespace ducky { namespace graphics {
	
	struct UniformBuffer2D
	{
		glm::mat4 viewProjection;
	};

	struct InstanceData2D
	{
		InstanceData2D() {}
		glm::vec4 minmax = glm::vec4(0, 0, 1, 1);
		glm::vec4 texture_mask_alpha_limits = glm::vec4(-1, -1, 0, 1);
		uint borderColor = 0;
		glm::vec4 borderLimitsOffset = glm::vec4(0, 0, 0, 0);
		glm::vec4 sizePosition = glm::vec4(1, 1, 0, 0);
		uint32 color1 = 0xffffffff;
		uint32 color2 = 0xffffffff;
		uint32 color3 = 0xffffffff;
		uint32 color4 = 0xffffffff;
		union
		{
			glm::mat4 model = glm::mat4(1);
			struct 
			{
				glm::vec4 model1;
				glm::vec4 model2;
				glm::vec4 model3;
				glm::vec4 model4;
			};
		};
	};

	struct Vertex2D
	{
		glm::vec3 position;
		glm::vec2 texCoord;
	};

	class Layer2D : public Layer
	{
	public:
		Layer2D();
		~Layer2D();

		void DrawImage(const glm::mat4& model, const glm::vec4& sizePosition, Texture2D* texture, float zindex = 0, int spriteW = 1, int spriteH = 1, int posX = 0, int posY = 0, bool flipX = false, bool flipY = false, const glm::vec2& alphaLimits = glm::vec2(0, 1), Texture2D* mask = nullptr, uint32 c1 = 0xffffffff, uint32 c2 = 0xffffffff, uint32 c3 = 0xffffffff, uint32 c4 = 0xffffffff);
		void DrawSprite(const glm::mat4& model, const glm::vec4& sizePosition, Sprite* sprite, float zindex = 0, bool flipX = false, bool flipY = false, const glm::vec2& alphaLimits = glm::vec2(0, 1), Texture2D* mask = nullptr, uint32 c1 = 0xffffffff, uint32 c2 = 0xffffffff, uint32 c3 = 0xffffffff, uint32 c4 = 0xffffffff);
		void DrawRect(const glm::mat4& model, const glm::vec4& sizePosition, uint32 c1, uint32 c2, uint32 c3, uint32 c4, float zindex = 0, Texture2D* mask = nullptr, const glm::vec4& minmax = glm::vec4(0, 0, 1, 1));
		void DrawString(const char* text, io::FontData* fd, Texture2D* fontMap, Texture2D* mask, glm::mat4 model, float scale, uint c1, uint c2, uint c3, uint c4, bool centered, float zindex = 0, glm::vec2 alphaLimits = glm::vec2(0, 1), uint borderColor = 0, glm::vec4 borderLimitsOffset = glm::vec4(0, 0, 0, 0), glm::vec3 position = glm::vec3(0));
		void DrawStringSimple(const char* text, io::FontData* fd, Texture2D* fontMap, const glm::vec3& position, float scale, uint color, bool centered = 0, float zindex = 0, const glm::vec2& alphaLimits = glm::vec2(0.45f, 0.55f));
		void DrawCharacter(const io::Character& ch, io::FontData* fd, Texture2D* fontMap, Texture2D* mask, glm::mat4 model, float scale, uint c1, uint c2, uint c3, uint c4, float zindex, glm::vec2 alphaLimits, uint borderColor, glm::vec4 borderLimitsOffset, glm::vec3 position);
		void AddDrawCall(const InstanceData2D&, float zindex);
		void Render(LightSetup*) override;
		static string GetLayerName() { return "Layer2d"; }
	private:
		float AddTexture(Texture2D*);
	private:
		GLuint m_VAO, m_VBO, m_IBO;
		GLuint m_IDAO;
		vector<Texture2D*> m_Textures;
		const int m_MaxInstances = 32000;
		Mesh* m_Plane;
		Shader* m_Shader;
		InstanceData2D* m_Instances;
		float* m_ZIndices;
		uint32 m_InstancesCount;
		Buffer* m_UniformBuffer;
		UniformBuffer2D m_UniformBufferData;
	};

} }