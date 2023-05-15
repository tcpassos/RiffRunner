#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "effect.h"
#include "resource_manager.h"
#include "shader.h"

class EffectShine : public Effect {
public:
    EffectShine() {
        this->shader = ResourceManager::loadShader("assets/shaders/effects/shine.vs", "assets/shaders/effects/shine.fs", nullptr, "effectShine");
        this->intensity = 1.0;
        this->speed = 1.0;
    }

    void setIntensity(float intensity) {
        this->intensity = intensity;
    }

    void setSpeed(float speed) {
        this->speed = speed;
    }

	void setup() override {
        shader.use();
        shader.setInteger("texBuff", 0);
        shader.setFloat("time", glfwGetTime());
        shader.setFloat("intensity", intensity);
        shader.setFloat("speed", speed);
	}

private:
    Shader shader;
    float intensity;
    float speed;
};