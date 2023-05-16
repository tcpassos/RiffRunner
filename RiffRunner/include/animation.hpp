#pragma once

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sprite.hpp"
#include "frame.h"
#include "rect.h"

class Animation {
public:
	Animation() {
		this->totalLength = 0.0;
	}
	
	void addFrame(Frame frame) {
		totalLength += frame.duration;
		this->frames.push_back(frame);
	}

	double getTotalLength() {
		return totalLength;
	}

	void update(Sprite* sprite, double elapsedTime) {
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

private:
	std::vector<Frame> frames;
	double totalLength;
};