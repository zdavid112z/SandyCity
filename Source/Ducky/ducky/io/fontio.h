#pragma once

#include "basicio.h"
#include "ducky_pch.h"
#include "glm\glm.hpp"
#include "serializable.h"

namespace ducky { namespace io {

	struct Kerning
	{
		uint first;
		uint second;
		float amount;
	};

	struct Character
	{
		uint id;
		//float s0;
		//float t0;
		//float s1;
		//float t1;
		float x, y;
		float width;
		float height;
		float xoffset;
		float yoffset;
		float xadvance;
	};

	class FontData : public Serializable
	{
	public:
		static const uint s_FileHeader;
		static const uint s_FileFooter;

		string name;
		uint size;
		glm::vec4 paddings;
		glm::vec2 spacings;
		float lineHeight;
		float base;
		uint width;
		uint height;
		Character* characters = nullptr;
		uint numCharacters;
		Kerning* kernings = nullptr;
		uint numKernings;

		void ReadFromFile(const string& path);
		void Read(IStream*) override;
		void Write(OStream*) override;
		void Delete();

		float GetFontMetrics(const char*, float scale);
	};

} }