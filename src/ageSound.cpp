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
