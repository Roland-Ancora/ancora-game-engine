#pragma once
#include "windows.h"
#include "audiere.h"


using namespace audiere;

namespace age {

	class Sound {
		static AudioDevicePtr audio_device;
		OutputStreamPtr sound;
	public:
		static void Init();
		void set_audio_file(const char* file_name);
		void play() { sound->play(); }
		void stop() { sound->stop(); }
		void set_volume(float vol) { sound->setVolume(vol); }
		void set_repeat(bool repeat) { sound->setRepeat(repeat); }
		bool is_playing() { return sound->isPlaying(); }
		float get_volume() { return sound->getVolume(); }
	};

}
