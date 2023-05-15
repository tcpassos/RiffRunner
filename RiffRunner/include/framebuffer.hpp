#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "texture.h"

class FrameBuffer {
public:
    GLuint id;
    Texture2D texture;
    int width;
    int height;

    FrameBuffer() : id(0), width(0), height(0) { }

    static FrameBuffer getDefault() {
        FrameBuffer defaultFrameBuffer;
        glfwGetWindowSize(glfwGetCurrentContext(), &defaultFrameBuffer.width, &defaultFrameBuffer.height);
        return defaultFrameBuffer;
    }

    void generate(unsigned int width, unsigned int height) {
        this->width = width;
        this->height = height;

        // Creates the framebuffer
        glGenFramebuffers(1, &id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        glViewport(0, 0, width, height);

        // Creates the color texture associated with the framebuffer
        texture.internalFormat = GL_RGBA;
        texture.imageFormat = GL_RGBA;
        texture.generate(width, height, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);

        // Create Render Buffer Object
        unsigned int RBO;
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        // Check if framebuffer was created
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Error creating framebuffer: " << status << std::endl;
        }

        // Unbind framebuffer and texture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
    }

    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};
