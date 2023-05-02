#include "animation.h"

Animation::Animation(Sprite& sprite) {
	this->sprite = &sprite;
	this->totalLength = 0.0;
	this->totalProgress = 0.0;
    this->lastTime = 0.0;
}

void Animation::addFrame(Frame& frame) {
	totalLength += frame.duration;
	this->frames.push_back(std::move(frame));
}

void Animation::update() {
    // Update elapsed time
    double currentTime = glfwGetTime();
    double elapsed = (currentTime - this->lastTime) * 1000;
    this->lastTime = currentTime;

    // Increase the total progress of the animation
    totalProgress += elapsed;

    // Use this progress as a counter. Final frame at progress <= 0
    double progress = totalProgress;
    for (Frame frame : frames) {
        progress -= frame.duration;

        // When progress is <= 0 or we are on the last frame in the list, stop
        if (progress <= 0.0 || &frame == &frames.back()) {
            this->sprite->setTextureRect(*frame.rect);
            break; // we found our frame
        }
    }
}