#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "projection.h"
#include "resource_manager.h"
#include "sprite.hpp"
#include "transformable.hpp"

class LightStripe {
public:
	LightStripe() {
        int screenWidth, screenHeight;
        glfwGetWindowSize(glfwGetCurrentContext(), &screenWidth, &screenHeight);

		this->shader = ResourceManager::loadShader("assets/shaders/light_stripe.vs", "assets/shaders/light_stripe.fs", nullptr, "shaderLightStripe");
        this->frameBuffer = ResourceManager::loadFrameBuffer(screenWidth, screenHeight, "neon");
        this->sprite = new Sprite(frameBuffer.texture);
        this->position = glm::vec2(400, 300);
        this->size = 1;
        this->radius = 0.005;
        this->intensity = 0.8;
        this->color = glm::vec3(1.0);

        float quadVertices[] = {
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

    void setPosition(float x, float y) {
        this->position = glm::vec2(x, y);
    }

    void setSize(float size) {
        this->size = size;
    }

    void setRadius(float radius) {
        this->radius = radius;
    }

    void setIntensity(float intensity) {
        this->intensity = intensity;
    }

    void setColor(glm::vec3 color) {
        this->color = color;
    }

    void setProjection(Projection& projection) {
        this->sprite->setProjection(projection);
    }

    glm::vec2 getPosition() {
        return position;
    }

    float getSize() {
        return size;
    }

    glm::vec3 getColor() {
        return color;
    }

    void move(float x, float y) {
        position.x += x;
        position.y += y;
    }
    
    void moveX(float x) {
        position.x += x;
    }

    void moveY(float y) {
        position.y += y;
    }

    void draw(GLFWwindow* window) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Prepare shader
        shader.use();
        shader.setVector2f("resolution", glm::vec2(width, height));
        shader.setVector2f("position", position);
        shader.setVector3f("neonColor", color);
        shader.setFloat("size", size);
        shader.setFloat("radius", radius);
        shader.setFloat("intensity", intensity);

        // Render neon on quad
        frameBuffer.bind();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(this->quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        frameBuffer.unbind();

        // Render sprite
        sprite->draw(window);
    }

private:
	Shader shader;
    FrameBuffer frameBuffer;
    Sprite *sprite;
    unsigned int quadVAO;

    glm::vec2 position;
    float size;
    float radius;
    float intensity;
    glm::vec3 color;
};