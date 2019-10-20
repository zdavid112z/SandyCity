#include "ducky_pch.h"
#include "skyboxLayer.h"

namespace ducky { namespace graphics {

	SkyboxLayer::SkyboxLayer() :
		Layer(GetLayerName(), 10)
	{
		m_Shader = new Shader("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");
		io::ModelData md;
		md.numVertices = 4;
		md.vertices = new io::Vertex[4];
		md.vertices[0].position = glm::vec3(-1, -1,  1);
		md.vertices[1].position = glm::vec3( 1, -1,  1);
		md.vertices[2].position = glm::vec3( 1,  1,  1);
		md.vertices[3].position = glm::vec3(-1,  1,  1);
		
		md.numIndices = 4;
		md.indices = new uint32[4];
		md.indices[0] = 3;
		md.indices[1] = 0;
		md.indices[2] = 2;
		md.indices[3] = 1;
		m_Cube = new Mesh(md);
		
		BufferDesc bd;
		bd.data = nullptr;
		bd.dynamic = true;
		bd.size = sizeof(SkyboxUniformBuffer);
		bd.type = BUFFER_UNIFORM;
		m_UniformBuffer = new Buffer(bd);
	}

	SkyboxLayer::~SkyboxLayer()
	{
		delete m_Cube;
		delete m_Shader;
		delete m_UniformBuffer;
	}

	void SkyboxLayer::Render(LightSetup* ls)
	{
		if (m_Camera == nullptr || m_SkyboxTexture == nullptr)
			return;
		glDisable(GL_DEPTH_TEST);
		m_Shader->Bind();
		m_BufferData.invViewMatrix = glm::inverse(m_Camera->GetViewMatrix());
		m_BufferData.invProjectionMatrix = glm::inverse(m_Camera->GetProjectionMatrix());
		m_UniformBuffer->SetData(&m_BufferData, 0, sizeof(SkyboxUniformBuffer));
		m_Shader->SetUniformBuffer("SkyboxUniformBuffer", m_UniformBuffer);
		m_SkyboxTexture->Bind();
		m_Cube->RenderStrips();
		glEnable(GL_DEPTH_TEST);
	}

} }