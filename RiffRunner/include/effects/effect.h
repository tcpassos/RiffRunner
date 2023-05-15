#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "framebuffer.hpp"

class Effect {
public:
	Effect() {
        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };
        unsigned int quadVBO;
        glGenVertexArrays(1, &this->quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(this->quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
	}

	void apply(FrameBuffer& inputFramebuffer, FrameBuffer& outputFramebuffer) {
        // Prepare the post processing effect
        setup();
        // Render texture with effect in the output framebuffer
        outputFramebuffer.bind();
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        inputFramebuffer.texture.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
	}

protected:
    // screen quad VAO
    unsigned int quadVAO;
    // Method implemented by the subclass to prepare the post-processing shader
    virtual void setup() = 0;
};