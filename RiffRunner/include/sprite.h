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

    void setTexture(Texture2D texture) { texture = texture; }
    void setColor(glm::vec3 color) { color = color; }
    void setPosition(float x, float y) { position.x = x; position.y = y; }
    void setScale(float x, float y) { scale.x = x; scale.y = y; }
    void setOrigin(float x, float y) { origin.x = x; origin.y = y; }
    void draw(GLFWwindow* window);

private:
    Texture2D texture;
    Shader shader;
    glm::vec3 color;
    glm::vec2 position;
    glm::vec2 scale;
    glm::vec2 origin;

    unsigned int quadVAO;

    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};