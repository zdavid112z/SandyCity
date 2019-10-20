#pragma once

#include "layer.h"
#include "../mesh.h"
#include "../texture2d.h"
#include "../shader.h"
#include "../material.h"
#include "../camera.h"

namespace ducky { namespace graphics {
	
	class Layer3D;

	struct SimpleDrawCall
	{
		Mesh* mesh;
		Material* material;
		Camera* camera;
		glm::mat4 transform;
		Buffer* userdata;
		string userdatalocation;
	};

	class SimpleLayer3D
	{
	public:
		SimpleLayer3D(Shader* s);
		virtual ~SimpleLayer3D();
		
		void Render(LightSetup*);
		void AddDrawCall(const SimpleDrawCall& dc) { m_DrawCalls.push_back(dc); }
		Shader* GetShader() { return m_Shader; }
	protected:
		MVPUniformBuffer m_UniformBuffer;
		Buffer* m_ShaderSystemBuffer;
		vector<SimpleDrawCall> m_DrawCalls;
		Shader* m_Shader;
	};

} }