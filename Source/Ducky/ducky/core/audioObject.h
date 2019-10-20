#pragma once

#ifndef DUCKY_DISABLE_OPENAL
#include "openal\ALFramework.h"
#endif
#include "glm\glm.hpp"
#include "utils\debug.h"

namespace ducky { namespace core {

	class AudioObject
	{
	public:
		AudioObject(const char* path)
		{
#ifndef DUCKY_DISABLE_OPENAL
			alGenBuffers(1, &m_Buffer);
			if (!ALFWLoadWaveToBuffer(path, m_Buffer))
			{
				utils::Debug::Printer << utils::ERR << "Failed to load sound file " << path << ENDL;
			}
			else
			{
				alGenSources(1, &m_Source);
				alSourcei(m_Source, AL_BUFFER, m_Buffer);
			}
#endif
		}

		~AudioObject()
		{
#ifndef DUCKY_DISABLE_OPENAL
			alSourceStop(m_Source);
			alDeleteSources(1, &m_Source);
			alDeleteBuffers(1, &m_Buffer);
#endif
		}

		void Play()
		{
#ifndef DUCKY_DISABLE_OPENAL
			alSourcePlay(m_Source);
#endif
		}

		void Pause()
		{
#ifndef DUCKY_DISABLE_OPENAL
			alSourcePause(m_Source);
#endif
		}

		void Stop()
		{
#ifndef DUCKY_DISABLE_OPENAL
			alSourceStop(m_Source);
#endif
		}

		void Rewind()
		{
#ifndef DUCKY_DISABLE_OPENAL
			alSourceRewind(m_Source);
#endif
		}

		bool IsPlaying()
		{
#ifndef DUCKY_DISABLE_OPENAL
			ALint state;
			alGetSourcei(m_Source, AL_SOURCE_STATE, &state);
			return state == AL_PLAYING;
#else
			return false;
#endif
		}

		void SetLooping(bool b)
		{
#ifndef DUCKY_DISABLE_OPENAL
			alSourcei(m_Source, AL_LOOPING, b ? AL_TRUE : AL_FALSE);
#endif
		}

		/*
		Sound must be mono not stereo
		*/
		void SetPosition(const glm::vec3& v)
		{
#ifndef DUCKY_DISABLE_OPENAL
			alSource3f(m_Source, AL_POSITION, v.x, v.y, v.z);
#endif
		}

		/*
		Sound must be mono not stereo
		*/
		void SetVelocity(const glm::vec3& v)
		{
#ifndef DUCKY_DISABLE_OPENAL
			alSource3f(m_Source, AL_VELOCITY, v.x, v.y, v.z);
#endif
		}

		/*
		Sound must be mono not stereo
		*/
		void SetDirection(const glm::vec3& v)
		{
#ifndef DUCKY_DISABLE_OPENAL
			alSource3f(m_Source, AL_DIRECTION, v.x, v.y, v.z);
#endif
		}

		void SetPitch(float f)
		{
#ifndef DUCKY_DISABLE_OPENAL
			alSourcef(m_Source, AL_PITCH, f);
#endif
		}

		void SetGain(float f)
		{
#ifndef DUCKY_DISABLE_OPENAL
			alSourcef(m_Source, AL_GAIN, f);
#endif
		}

		int GetSizeInBytes()
		{
#ifndef DUCKY_DISABLE_OPENAL
			int a;
			alGetBufferi(m_Buffer, AL_SIZE, &a);
			return a;
#else
			return 0;
#endif
		}

		int GetChannels()
		{
#ifndef DUCKY_DISABLE_OPENAL
			int a;
			alGetBufferi(m_Buffer, AL_CHANNELS, &a);
			return a;
#else
			return 0;
#endif
		}

		int GetBits()
		{
#ifndef DUCKY_DISABLE_OPENAL
			int a;
			alGetBufferi(m_Buffer, AL_BITS, &a);
			return a;
#else
			return 0;
#endif
		}

		int GetFrequency()
		{
#ifndef DUCKY_DISABLE_OPENAL
			int a;
			alGetBufferi(m_Buffer, AL_FREQUENCY, &a);
			return a;
#else
			return 0;
#endif
		}

		int GetDurationInSamples()
		{
#ifndef DUCKY_DISABLE_OPENAL
			return GetSizeInBytes() * 8 / (GetChannels() * GetBits());
#else 
			return 0;
#endif
		}

		float GetDuration()
		{
#ifndef DUCKY_DISABLE_OPENAL
			return (float)GetDurationInSamples() / (float)GetFrequency();
#else
			return 0;
#endif
		}

		static void SetListenerPosition(const glm::vec3& v)
		{
#ifndef DUCKY_DISABLE_OPENAL
			alListener3f(AL_POSITION, v.x, v.y, v.z);
#endif
		}

		static void SetListenerVelocity(const glm::vec3& v)
		{
#ifndef DUCKY_DISABLE_OPENAL
			alListener3f(AL_VELOCITY, v.x, v.y, v.z);
#endif
		}
	private:
#ifndef DUCKY_DISABLE_OPENAL
		ALuint m_Buffer;
		ALuint m_Source;
#endif
	};

} }