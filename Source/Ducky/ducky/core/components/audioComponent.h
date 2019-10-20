#pragma once

#include "../audioObject.h"
#include "../entity.h"
#include "../scene.h"
#include "../audioObject.h"

namespace ducky { namespace core {

	class AudioComponent : public Component
	{
	public:
		AudioComponent(AudioObject* audio) 
		{ 
			m_Audio = audio;
		}

		string GetType() { return GetComponentType(); }
		static string GetComponentType() { return "Audio"; }

		AudioObject* GetAudioObject() { return m_Audio; }

		void Update();
	private:
		AudioObject* m_Audio;
	};

} }