#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "resource_manager.h"
#include "shape.hpp"

class RectangleShape : public Shape {
public:
	RectangleShape(unsigned int width, unsigned int heigth) : Shape(width, heigth) {
		this->shader = ResourceManager::loadShader("assets/shaders/shape.vs", "assets/shaders/shape.fs", nullptr, "shaderShape");
	}

private:
	Shader shader;

	void drawElements(GLFWwindow* window) override {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Prepare matrices
        glm::mat4 projection = getProjectionMatrix(width, height);
        glm::mat4 model = getModelMatrix();

        // Prepare shader
        this->shader.use();
        this->shader.setMatrix4("projection", projection);
        this->shader.setMatrix4("model", model);
        this->shader.setVector4f("fillColor", this->color);

        // Prepare blending
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_ALWAYS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Render textured quad
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
	}
};