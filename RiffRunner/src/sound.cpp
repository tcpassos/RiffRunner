#include "sound.h"

irrklang::ISoundEngine* Sound::engine = irrklang::createIrrKlangDevice();

Sound::Sound(const char* filePath) {
    this->filePath = filePath;
    sound = engine->play2D(filePath, false, true, true);
}

void Sound::stopAll() {
    engine->stopAllSounds();
}

void Sound::dropEngine() {
    engine->drop();
}

void Sound::play() {
    if (sound->isFinished()) {
        sound = engine->play2D(filePath, false, true, true);
    }
    sound->setIsPaused(false);
}

void Sound::playOnce() {
    sound = engine->play2D(filePath);
}

void Sound::pause() {
    sound->setIsPaused(true);
}

void Sound::stop() {
    sound->stop();
}

void Sound::setVolume(unsigned int volume) {
    sound->setVolume((float)volume / 100.0);
}

bool Sound::isFinished() {
    return sound->isFinished();
}