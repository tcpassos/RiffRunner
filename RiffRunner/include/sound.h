#pragma once

#include <irrklang/irrKlang.h>

class Sound {
public:
    Sound(const char* filePath);
    void play();
    void playOnce();
    void pause();
    void stop();
    void setVolume(unsigned int volume);
    bool isFinished();
    static void stopAll();
    static void dropEngine();

private:
    static irrklang::ISoundEngine* engine;
    irrklang::ISound* sound;
    const char* filePath;
};