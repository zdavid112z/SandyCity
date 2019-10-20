#pragma once

#include "../material.h"
#include "../texture2d.h"

namespace ducky { namespace graphics {

	class SimpleMaterial : public Material
	{
	public:
		SimpleMaterial(const string& name) : Material(name) {}

		void Bind(Shader* shader) override
		{
			if(m_Texture != nullptr)
				m_Texture->Bind(0);
		}

		void Unbind(Shader* shader) override
		{

		}

		void SetTexture(Texture2D* t) { m_Texture = t; }
	private:
		Texture2D* m_Texture;
	};

} }