#include "ducky_pch.h"
#include "fontio.h"
#include "../utils/debug.h"
#include "utils\stringUtils.h"
#include "fileio.h"

namespace ducky { namespace io {
	
	const uint FontData::s_FileHeader = *((uint*)"DFTB");
	const uint FontData::s_FileFooter = *((uint*)"DFTE");

	void FontData::Read(IStream* in)
	{
		Delete();
		uint hf;
		in->ReadUInt(hf);
		Debug::Assert(hf == s_FileHeader, "Font file does not have the proper header!\n");
		in->ReadString(name);
		in->ReadUInt(size);
		in->ReadFloat(paddings[0]);
		in->ReadFloat(paddings[1]);
		in->ReadFloat(paddings[2]);
		in->ReadFloat(paddings[3]);
		in->ReadFloat(spacings[0]);
		in->ReadFloat(spacings[1]);
		in->ReadFloat(lineHeight);
		in->ReadFloat(base);
		in->ReadUInt(width);
		in->ReadUInt(height);
		in->ReadUInt(numCharacters);
		characters = new Character[numCharacters];
		in->ReadBytes((byte*)characters, numCharacters * sizeof(Character));
		in->ReadUInt(numKernings);
		kernings = new Kerning[numKernings];
		in->ReadBytes((byte*)kernings, numKernings * sizeof(Kerning));
		in->ReadUInt(hf);
		Debug::Assert(hf == s_FileFooter, "Font file does not have the proper footer!\n");
	}

	void FontData::Write(OStream* out)
	{
		out->WriteUInt(s_FileHeader);
		out->WriteString(name);
		out->WriteUInt(size);
		out->WriteFloat(paddings[0]);
		out->WriteFloat(paddings[1]);
		out->WriteFloat(paddings[2]);
		out->WriteFloat(paddings[3]);
		out->WriteFloat(spacings[0]);
		out->WriteFloat(spacings[1]);
		out->WriteFloat(lineHeight);
		out->WriteFloat(base);
		out->WriteUInt(width);
		out->WriteUInt(height);
		out->WriteUInt(numCharacters);
		out->WriteBytes((byte*)characters, numCharacters * sizeof(Character));
		out->WriteUInt(numKernings);
		out->WriteBytes((byte*)kernings, numKernings * sizeof(Kerning));
		out->WriteUInt(s_FileFooter);
	}

	float FontData::GetFontMetrics(const char* text, float scale)
	{
		float len = 0;
		char prev = 0;
		while (*text)
		{
			char ch = *text;
			io::Character* chData = NULL;
			for (uint i = 0; i < numCharacters; i++)
			{
				if (characters[i].id == ch)
				{
					chData = &characters[i];
					break;
				}
			}

			for (uint i = 0; i < numKernings; i++)
			{
				if (kernings[i].first == ch && kernings[i].second == prev)
				{
					len += kernings[i].amount * scale;
					break;
				}
			}

			len += (chData->xadvance - paddings[1] - paddings[3] - spacings[0]) * scale;
			text++;
			prev = ch;
		}
		return len;
	}

	void FontData::Delete()
	{
		if (kernings)
			delete[] kernings;
		if (characters)
			delete[] characters;
		kernings = nullptr;
		characters = nullptr;
	}

	void FontData::ReadFromFile(const string& path)
	{
		Delete();
		uint size;
		string text = FileUtils::ReadTextFile(FileUtils::ProcessPath(path).c_str());
		vector<string> lines = StringUtils::SplitString(text, "\n");
		uint ch = 0, k = 0;
		glm::vec4 paddings;
		glm::vec2 spacings;
		for (uint i = 0; i < lines.size(); i++)
		{
			vector<string> parts = StringUtils::SplitString(lines[i], " ");
			if (parts.size() == 0)
				continue;
			if (parts[0] == "info")
			{
				name = StringUtils::SplitString(parts[1], "=")[1];
				name.pop_back();
				name.erase(name.begin());

				size = StringUtils::ParseInt(StringUtils::SplitString(parts[2], "=")[1]);
				vector<string> paddingss = StringUtils::SplitString(StringUtils::SplitString(parts[10], "=")[1], ",");
				paddings.x = (float)StringUtils::ParseInt(paddingss[0]);
				paddings.y = (float)StringUtils::ParseInt(paddingss[1]);
				paddings.z = (float)StringUtils::ParseInt(paddingss[2]);
				paddings.w = (float)StringUtils::ParseInt(paddingss[3]);

				vector<string> spacingss = StringUtils::SplitString(StringUtils::SplitString(parts[11], "=")[1], ",");
				spacings.x = (float)StringUtils::ParseInt(spacingss[0]);
				spacings.y = (float)StringUtils::ParseInt(spacingss[1]);
			}
			else if (parts[0] == "common")
			{
				lineHeight = (float)StringUtils::ParseInt(StringUtils::SplitString(parts[1], "=")[1]);
				base = (float)StringUtils::ParseInt(StringUtils::SplitString(parts[2], "=")[1]);
				width = StringUtils::ParseInt(StringUtils::SplitString(parts[3], "=")[1]);
				height = StringUtils::ParseInt(StringUtils::SplitString(parts[4], "=")[1]);
			}
			else if (parts[0] == "chars")
			{
				numCharacters = StringUtils::ParseInt(StringUtils::SplitString(parts[1], "=")[1]);
				characters = new Character[numCharacters];
			}
			else if (parts[0] == "kernings")
			{
				numKernings = StringUtils::ParseInt(StringUtils::SplitString(parts[1], "=")[1]);
				kernings = new Kerning[numKernings];
			}
			else if (parts[0] == "char")
			{
				characters[ch].id = StringUtils::ParseInt(StringUtils::SplitString(parts[1], "=")[1]);
				characters[ch].x = (float)StringUtils::ParseInt(StringUtils::SplitString(parts[2], "=")[1]);
				characters[ch].y = (float)StringUtils::ParseInt(StringUtils::SplitString(parts[3], "=")[1]);
				characters[ch].width = (float)StringUtils::ParseInt(StringUtils::SplitString(parts[4], "=")[1]);
				characters[ch].height = (float)StringUtils::ParseInt(StringUtils::SplitString(parts[5], "=")[1]);
				characters[ch].xoffset = (float)StringUtils::ParseInt(StringUtils::SplitString(parts[6], "=")[1]);
				characters[ch].yoffset = (float)StringUtils::ParseInt(StringUtils::SplitString(parts[7], "=")[1]);
				characters[ch].xadvance = (float)StringUtils::ParseInt(StringUtils::SplitString(parts[8], "=")[1]);
				ch++;
			}
			else if (parts[0] == "kerning")
			{
				kernings[k].first = StringUtils::ParseInt(StringUtils::SplitString(parts[1], "=")[1]);
				kernings[k].second = StringUtils::ParseInt(StringUtils::SplitString(parts[2], "=")[1]);
				kernings[k].amount = (float)StringUtils::ParseInt(StringUtils::SplitString(parts[3], "=")[1]);
				k++;
			}
		}
		this->paddings = paddings;
		this->spacings = spacings;
	}

} }