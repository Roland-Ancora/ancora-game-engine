//#################################################################//
//
//								Sound
//	
//		The Sound class is responsible for playing sounds. The 
//	SoundMultiple class is used to play the same sound through
//	multiple channels. This allows it to owerlap itself.
// 
//		The Sound class has external dependencies: Windows,
//	Audiere.
//
//
//#################################################################//



#pragma once
#include <windows.h>
#include <audiere/audiere.h>
#include "ageCONSTANTS.h"



using namespace audiere;



namespace age {

	class Sound {
		OutputStreamPtr sound;
		static AudioDevicePtr audio_device;
	public:
		static void Init();
	public:
		void play() { sound->play(); }
		void stop() { sound->stop(); }
		void set_volume(float vol) { sound->setVolume(vol); }
		void set_repeat(bool repeat) { sound->setRepeat(repeat); }
		bool is_playing() { return sound->isPlaying(); }
		float get_volume() { return sound->getVolume(); }
	public:
		void set_audio_file(const char* file_name);
	};

	class SoundMultiple {
		Sound s[AGE_SOUND_SOUNDMULTIPLE_SOUND_BUFFER_COUNT];
	public:
		float get_volume() { return s[0].get_volume(); }
	public:
		void set_volume(float vol);
		void play();
		void stop();
		void set_audio_file(const char* file_name);
	};

}
