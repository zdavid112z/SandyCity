#include "ducky_pch.h"
#include "meshComponent.h"

namespace ducky { namespace core {

	MeshComponent::MeshComponent(graphics::Shader* shader, graphics::Mesh* mesh, graphics::Material* material, graphics::Camera* cam)
	{
		m_DrawCall.camera = cam;
		m_DrawCall.material = material;
		m_DrawCall.mesh = mesh;
		m_DrawCall.shader = shader;
	}

	void MeshComponent::Render()
	{
		m_DrawCall.transform = m_Parent->GetComponent<TransformComponent>()->GetTransform().GetModel();
		m_Parent->GetScene()->GetLayer<graphics::Layer3D>()->AddDrawCall(m_DrawCall);
	}

} }