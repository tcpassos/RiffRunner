#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "texture.h"
#include "shader.h"

class Sprite {
public:
    // Constructor (inits shaders/shapes)
    Sprite();
    // Destructor
    ~Sprite();

    void setTexture(Texture2D texture) { this->texture = texture; }
    void setColor(glm::vec3 color) { this->color = color; }
    void setPosition(float x, float y) { this->position.x = x; this->position.y = y; }
    void setScale(float x, float y) { this->scale.x = x; this->scale.y = y; }
    void setOrigin(float x, float y) { this->origin.x = x; this->origin.y = y; }
    void setRotation(float rotation) { this->rotation = rotation; }
    void draw(GLFWwindow* window);

private:
    Texture2D texture;
    Shader shader;
    unsigned int VAO;
    glm::vec3 color;
    glm::vec2 position;
    glm::vec2 scale;
    glm::vec2 origin;
    float rotation;

    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};