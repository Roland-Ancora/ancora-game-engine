#include "ageSound.h"



using namespace age;
using namespace audiere;



AudioDevicePtr Sound::audio_device;

void Sound::Init()
{
	audio_device = OpenDevice();
}

void Sound::set_audio_file(const char* file_name)
{
	sound = OpenSound(audio_device, file_name, false);
}





void SoundMultiple::set_volume(float vol)
{
	for (uint8_t i = 0; i < AGE_SOUND_SOUNDMULTIPLE_SOUND_BUFFER_COUNT; i++)
		s[i].set_volume(vol);
}

void SoundMultiple::play()
{
	for (uint8_t i = 0; i < AGE_SOUND_SOUNDMULTIPLE_SOUND_BUFFER_COUNT; i++) {
		if (s[i].is_playing())
			continue;
		s[i].play();
	}
}

void SoundMultiple::stop()
{
	for (uint8_t i = 0; i < AGE_SOUND_SOUNDMULTIPLE_SOUND_BUFFER_COUNT; i++)
		s[i].stop();
}

void SoundMultiple::set_audio_file(const char* file_name)
{
	for (uint8_t i = 0; i < AGE_SOUND_SOUNDMULTIPLE_SOUND_BUFFER_COUNT; i++)
		s[i].set_audio_file(file_name);
}
