#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "effects.h"
#include "framebuffer.hpp"

class PostProcessingPipeline {
public:
    void addEffect(Effect* effect) {
        effects.push_back(effect);
    }
    
    void clearEffects() {
        effects.clear();
    }

    bool isEmpty() {
        return effects.empty();
    }

    void begin() {
        if (!effects.empty()) {
            int screenWidth, screenHeight;
            glfwGetWindowSize(glfwGetCurrentContext(), &screenWidth, &screenHeight);

            initialFrameBuffer = ResourceManager::loadFrameBuffer(screenWidth, screenHeight, "initialFrameBuffer");
            initialFrameBuffer.bind();
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }

    void end() {
        applyPipeline(initialFrameBuffer, FrameBuffer::getDefault());
    }

private:
    FrameBuffer initialFrameBuffer;
    std::vector<Effect*> effects;

    void applyPipeline(FrameBuffer& inputFramebuffer, FrameBuffer outputFramebuffer) {
        if (effects.empty()) {
            return;
        }

        if (effects.size() == 1) {
            effects[0]->apply(inputFramebuffer, outputFramebuffer);
            return;
        }

        FrameBuffer intermediateFramebufferA = ResourceManager::loadFrameBuffer(outputFramebuffer.width, outputFramebuffer.height, "intermediateFramebufferA");
        effects[0]->apply(inputFramebuffer, intermediateFramebufferA);
        if (effects.size() == 2) {
            effects[1]->apply(intermediateFramebufferA, outputFramebuffer);
            return;
        }

        FrameBuffer intermediateFramebufferB = ResourceManager::loadFrameBuffer(outputFramebuffer.width, outputFramebuffer.height, "intermediateFramebufferB");
        effects[1]->apply(intermediateFramebufferA, intermediateFramebufferB);
        if (effects.size() == 3) {
            effects[2]->apply(intermediateFramebufferB, outputFramebuffer);
            return;
        }

        for (int i = 3; i < effects.size() - 1; i++) {
            effects[i]->apply(intermediateFramebufferB, intermediateFramebufferA);
            intermediateFramebufferB = intermediateFramebufferA;
        }

        effects[effects.size() - 1]->apply(intermediateFramebufferA, outputFramebuffer);
    }
};