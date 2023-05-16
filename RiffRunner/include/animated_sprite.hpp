#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "animation.hpp"
#include "frame.h"
#include "sprite.hpp"

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite(Texture2D texture) : Sprite(texture) {
		this->elapsedTime = 0.0;
		this->lastTime = glfwGetTime();
	};
	
	void addFrame(Frame frame) {
		animation.addFrame(frame);
	}

	void resetAnimation() {
		lastTime = glfwGetTime();
		elapsedTime = 0.0;
	}

	void update() {
		double currentTime = glfwGetTime();
		elapsedTime += currentTime - lastTime;
		lastTime = currentTime;
		animation.update(this, elapsedTime);
	}

	bool isRunning() {
		return elapsedTime > 0.0 && elapsedTime <= animation.getTotalLength();
	}

private:
	Animation animation;
	double elapsedTime;
	double lastTime;
};