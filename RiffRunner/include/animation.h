#pragma once

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sprite.h"
#include "frame.h"
#include "rect.h"

class Animation {
public:
	Animation(Sprite &sprite);
	void addFrame(Frame frame);
	void reset() { this->totalProgress = 0.0; }
	void update();
	bool isRunning() { return totalProgress < totalLength;  }

private:
	Sprite* sprite;
	std::vector<Frame> frames;
	double totalLength;
	double totalProgress;
	double lastTime;
};