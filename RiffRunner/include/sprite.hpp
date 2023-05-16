#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "framebuffer.hpp"
#include "post_processing_pipeline.hpp"
#include "texture.h"
#include "shader.h"
#include "shape.hpp"

class Sprite : public Shape {
public:
    Sprite(Texture2D &texture) : Shape(texture.width, texture.height) {
        this->shader = ResourceManager::loadShader("assets/shaders/sprite.vs", "assets/shaders/sprite.fs", nullptr, "shaderSprite");
        this->texture = texture;
        this->textureRect = new Rect(glm::vec2(0.0f, 0.0f), glm::vec2(this->texture.width, this->texture.height));
    }

    void setTexture(Texture2D &texture) {
        this->texture = texture;
    }

    void setTextureRect(Rect& textureRect) {
        this->textureRect = &textureRect;

        // Normalize vertices
        glm::vec2 topLeft = glm::vec2(textureRect.left / this->size.x, textureRect.top / this->size.y);
        glm::vec2 topRight = glm::vec2(textureRect.width / this->size.x, textureRect.top / this->size.y);
        glm::vec2 bottomLeft = glm::vec2(textureRect.left / this->size.x, textureRect.height / this->size.y);
        glm::vec2 bottomRight = glm::vec2(textureRect.width / this->size.x, textureRect.height / this->size.y);

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

        // Top left
        updateTextureCoord(0, topLeft.x, topLeft.y);
        updateTextureCoord(5, topLeft.x, topLeft.y);
        // Top right
        updateTextureCoord(4, topRight.x, topRight.y);
        // Bottom left
        updateTextureCoord(1, bottomLeft.x, bottomLeft.y);
        // Bottom right
        updateTextureCoord(2, bottomRight.x, bottomRight.y);
        updateTextureCoord(3, bottomRight.x, bottomRight.y);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    Rect& getTextureRect() {
        return *this->textureRect;
    };

private:
    Shader shader;
    Texture2D texture;
    Rect* textureRect;

    void updateTextureCoord(int index, float x, float y) {
        GLintptr offsetS = 3 * sizeof(GLfloat);
        GLintptr offsetT = 4 * sizeof(GLfloat);
        glBufferSubData(GL_ARRAY_BUFFER, index * 5 * sizeof(GLfloat) + offsetS, sizeof(GLfloat), &x);
        glBufferSubData(GL_ARRAY_BUFFER, index * 5 * sizeof(GLfloat) + offsetT, sizeof(GLfloat), &y);
    }

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
        this->shader.setVector4f("texColor", this->color);
        this->shader.setInteger("texBuff", 0);

        // Prepare texture
        glActiveTexture(GL_TEXTURE0);
        this->texture.bind();
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