#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "effect.h"
#include "resource_manager.h"
#include "shader.h"

class EffectGrayScale : public Effect {
public:
    EffectGrayScale() {
        this->shader = ResourceManager::loadShader("assets/shaders/effects/gray_scale.vs", "assets/shaders/effects/gray_scale.fs", nullptr, "effectGrayScale");
    }

	void setup() override {
        shader.use();
        shader.setInteger("texBuff", 0);
	}

private:
    Shader shader;
};