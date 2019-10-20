#include "ducky_pch.h"
#include "material.h"
#include "utils\imageUtils.h"

namespace ducky { namespace graphics {

	Texture2D* Material::g_NormalDefault = nullptr;

	void Material::InitMaterials()
	{
		io::ImageData id = ImageUtils::CreateNew(16, 16, io::IFI_RGB8, io::IF_RGB, 0x8080ff);
		g_NormalDefault = new Texture2D(id, TextureParams());
		id.Delete();
	}

} }