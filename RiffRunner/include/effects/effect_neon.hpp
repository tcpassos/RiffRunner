#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "effect.h"
#include "resource_manager.h"
#include "shader.h"

class EffectNeon : public Effect {
public:
    EffectNeon() {
        this->shader = ResourceManager::loadShader("assets/shaders/effects/neon.vs", "assets/shaders/effects/neon.fs", nullptr, "effectNeon");
    }

	void setup() override {
        shader.use();
        shader.setInteger("texBuff", 0);
        shader.setFloat("time", glfwGetTime());
	}

private:
    Shader shader;
};