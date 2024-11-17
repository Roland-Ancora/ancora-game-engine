//#################################################################//
//
//								Sound
//	
//		The Sound class is responsible for playing sounds.
// 
//		The Sound class has external dependencies: Windows,
//	Audiere.
//
//
//#################################################################//



#pragma once
#include <windows.h>
#include <audiere.h>



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

}
