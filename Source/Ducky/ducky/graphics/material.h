#pragma once

#include "opengl.h"
#include "shader.h"
#include "texture2d.h"

namespace ducky { namespace graphics {

	enum MaterialFlags
	{
		MATERIAL_DISABLE_DEPTH_TEST = BIT(0),
		MATERIAL_TRANSPARENCY = BIT(1),
		MATERIAL_ADDITIVE_BLENDING = BIT(2)
	};

	class Material
	{
	protected:
		static Texture2D* g_NormalDefault;
	public:
		static void InitMaterials();
	public:
		Material(const string& name) : m_Name(name) {}
		virtual ~Material() {}
		virtual void Bind(Shader*) = 0;
		virtual void Unbind(Shader*) = 0;
		const string& GetName() { return m_Name; }
		uint32& GetFlags() { return m_Flags; }
	protected:
		uint32 m_Flags = 0;
		string m_Name;
	};

} }