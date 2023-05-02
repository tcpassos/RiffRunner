#include "audio_player.h"

AudioPlayer::AudioPlayer() {
    // Initialize the OpenAL device
    device = alcOpenDevice(nullptr);
    if (!device) {
        std::cerr << "Error initializing OpenAL device" << std::endl;
        return;
    }
    // Create the OpenAL context
    context = alcCreateContext(device, nullptr);
    if (!context) {
        std::cerr << "Error creating OpenAL context" << std::endl;
        return;
    }
    // Make the context active
    alcMakeContextCurrent(context);
}

AudioPlayer::~AudioPlayer() {
    // Stops and flushes the buffer and source
    alSourceStop(source);
    alDeleteBuffers(1, &buffer);
    alDeleteSources(1, &source);
    // Finalize context and device
    alcDestroyContext(context);
    alcCloseDevice(device);
}

bool AudioPlayer::loadAudio(const char* filePath) {
    ALint frequency, channels;

    // Generate the OpenAL buffer
    alGenBuffers(1, &buffer);
    // Load the audio file
    FILE* file;
    if (fopen_s(&file, filePath, "rb") != 0) {
        std::cerr << "Error loading audio file" << std::endl;
        return false;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char header[44];
    fread(header, 1, 44, file);
    // Get the frequency and channel information from the file header
    frequency = *(int*)(header + 24);
    channels = *(short*)(header + 22);

    unsigned char* data = new unsigned char[size];
    fread(data, sizeof(char), size, file);
    fclose(file);
    // Configures OpenAL buffer parameters
    ALenum format;
    if (channels == 2) {
        format = AL_FORMAT_STEREO16;
    } else {
        format = AL_FORMAT_MONO16;
    }
    size_t aligned_size = size - (size % 4);
    alBufferData(buffer, format, data, aligned_size, frequency);
    delete[] data;
    // Generate the OpenAL source
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    // Check if there was an error
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "Error loading audio file: " << error << std::endl;
        return false;
    }
    return true;
}

void AudioPlayer::play() {
    alSourcePlay(source);
}

void AudioPlayer::pause() {
    alSourcePause(source);
}

void AudioPlayer::stop() {
    alSourceStop(source);
}

void AudioPlayer::setVolume(float volume) {
    alSourcef(source, AL_GAIN, volume/100 );
}