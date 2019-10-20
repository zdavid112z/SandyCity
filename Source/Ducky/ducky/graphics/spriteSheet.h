#pragma once

#include "texture2d.h"
#include "io\basicio.h"

namespace ducky { namespace graphics {

	class SpriteSheet;

	struct Sprite
	{
		SpriteSheet* sheet;
		glm::vec2 texMin, texMax;
		string name;
	};

	class SpriteSheet
	{
	public:
		// If GenerateAuto is called, the ImageData will be cleared, but not deleted
		SpriteSheet(io::ImageData* copy, Texture2D* texture);
		~SpriteSheet();

		// If GenerateAuto is called, the ImageData will be cleared, but not deleted
		void GenerateAuto(int padding, uint minSize);
		void GenerateGrid(uint width, uint height, bool topDown = true);

		void Read(io::IStream&);
		void Write(io::OStream&) const;

		Sprite* AddSprite(const Sprite&);
		Sprite* Rename(const string& oldName, const string& newName);
		Texture2D* GetTexture() const;
		Sprite* GetSprite(uint index);
		uint GetNumSprites() const;
		Sprite* GetSprite(const string& name);
	protected:
		byte GetPixelAlpha(uint x, uint y);
		void NullPixel(uint x, uint y);
		glm::u16vec4 GetMinMax(uint x, uint y);
	protected:
		vector<glm::u16vec2> m_Stack;
		io::ImageData* m_ImageData;
		vector<Sprite> m_Sprites;
		Texture2D* m_Texture;
	};

} }