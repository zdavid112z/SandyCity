#include "ducky_pch.h"
#include "audioComponent.h"
#include "rigidBodyComponent.h"
#include "transformComponent.h"

namespace ducky { namespace core {

	void AudioComponent::Update()
	{

		TransformComponent* tr = m_Parent->GetComponent<TransformComponent>();
		if (tr != nullptr)
		{
			glm::vec4 pos = tr->GetTransform().GetModel() * glm::vec4(0, 0, 0, 1);
			glm::mat4 view = m_Parent->GetScene()->GetCamera()->GetViewMatrix();
			m_Audio->SetPosition(glm::vec3(view * pos));
		}
		RigidBodyComponent* rb = m_Parent->GetComponent<RigidBodyComponent>();
		if (rb != nullptr)
		{
			m_Audio->SetVelocity(rb->GetRigidBody()->GetLinearVelocity());
		}
	}

} }