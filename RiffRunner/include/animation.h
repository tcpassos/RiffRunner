#pragma once

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sprite.h"
#include "frame.h"
#include "rect.h"

class Animation {
public:
	Animation();
	void addFrame(Frame frame);
	void update(Sprite* sprite, double elapsedTime);
	double getTotalLength() { return totalLength; }

private:
	std::vector<Frame> frames;
	double totalLength;
};