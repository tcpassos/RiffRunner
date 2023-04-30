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
    Sprite(Texture2D texture);
    // Destructor
    ~Sprite();

    void setTexture(Texture2D texture) { this->texture = texture; }
    void setTextureRect(glm::vec4 textureRect);
    void setColor(glm::vec4 color) { this->color = color; }
    void setPosition(glm::vec3 position) { this->position = position; }
    void setSize(glm::vec3 size) { this->size = size; }
    void setOrigin(glm::vec3 origin) { this->origin = origin; }
    void setRotation(glm::vec3 rotation) { this->rotation = rotation; }
    void draw(GLFWwindow* window);

private:
    Shader shader;
    GLuint VBO;
    GLuint VAO;
    Texture2D texture;
    glm::vec4 textureRect;
    glm::vec4 color;
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 origin;
    glm::vec3 rotation;

    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};