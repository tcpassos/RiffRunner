#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "texture.h"
#include "shader.h"
#include "shape.h"

class Sprite : public Shape {
public:
    Sprite(Texture2D texture);

    void setTexture(Texture2D texture) { this->texture = texture; }
    void setTextureRect(Rect &textureRect);
    Rect& getTextureRect() { return *this->textureRect; };
    void draw(GLFWwindow* window);

private:
    Shader shader;
    Texture2D texture;
    Rect* textureRect;

    void updateTextureCoord(int index, float x, float y);
};