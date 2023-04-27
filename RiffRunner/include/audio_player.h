#pragma once

#include <iostream>
#include <AL/al.h>
#include <AL/alc.h>

class AudioPlayer {
public:
	AudioPlayer();
	~AudioPlayer();
	bool loadAudio(const char* filePath);
	void play();
	void pause();
	void stop();
	void setVolume(float volume);

private:
	ALCdevice* device = nullptr;
	ALCcontext* context = nullptr;
	ALuint buffer = 0;
	ALuint source = 0;
};