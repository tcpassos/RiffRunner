#include "animation.h"

Animation::Animation() {
	this->totalLength = 0.0;
}

void Animation::addFrame(Frame frame) {
	totalLength += frame.duration;
	this->frames.push_back(frame);
}

void Animation::update(Sprite* sprite, double elapsedTime) {
    // Use this progress as a counter. Final frame at progress <= 0
    double progress = elapsedTime;
    for (Frame frame : frames) {
        progress -= frame.duration;

        // When progress is <= 0 or we are on the last frame in the list, stop
        if (progress <= 0.0 || &frame == &frames.back()) {
            sprite->setTextureRect(*frame.rect);
            break; // we found our frame
        }
    }
}