#pragma once

#include "../entity.h"
#include "graphics\mesh.h"
#include "graphics\shader.h"
#include "graphics\materials\simpleMaterial.h"
#include "transformComponent.h"
#include "core\scene.h"
#include "graphics\layer\layer3d.h"

namespace ducky { namespace core {

	class MeshComponent : public Component
	{
	public:
		MeshComponent(graphics::Shader* shader, graphics::Mesh* mesh, graphics::Material* material, graphics::Camera* cam);

		void Render() override;

		string GetType() override { return GetComponentType(); }
		static string GetComponentType() { return "Mesh"; }

	private:
		graphics::DrawCall3D m_DrawCall;
	};

} }
