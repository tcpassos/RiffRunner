#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "texture.h"
#include "shader.h"
#include "projection.h"
#include "rect.h"

class Shape {
public:
    Shape(unsigned int width, unsigned int height);
    ~Shape();
    void setColor(glm::vec4 color) { this->color = color; }
    glm::vec4 getColor() { return this->color; }
    void setOpacity(float opacity) { this->color.a = opacity; }
    float getOpacity() { return this->color.a; }

    void setPosition(glm::vec2 position) { this->position = position; }
    void setPosition(float x, float y) { this->position.x = x; this->position.y = y; }
    glm::vec2 getPosition() { return this->position; }
    void moveX(float movement) { this->position = glm::vec2(this->position.x + movement, this->position.y); }
    void moveY(float movement) { this->position = glm::vec2(this->position.x, this->position.y + movement); }

    void setSize(glm::vec2 size) { this->size = size; }
    void setSize(float width, float height) { this->size.x = width; this->size.y = height; }
    glm::vec2 getSize() { return this->size; }

    void setRotation(float rotation) { this->rotation = rotation; }
    float getRotation() { return this->rotation; }
    void rotate(float rotation) { this->rotation = this->rotation + rotation; };

    void setOrigin(glm::vec2 origin) { this->origin = origin; }
    void setOrigin(float x, float y) { this->origin.x = x; this->origin.y = y; }
    glm::vec2 getOrigin() { return this->origin; }

    void setProjection(Projection& projection, bool preserveModel = false) { this->projection = &projection; this->preserveModel = preserveModel; }
    Projection* getProjection() { return this->projection; }

    Rect getBounds();

protected:
    GLuint VBO;
    GLuint VAO;
    Projection* projection;
    glm::vec4 color;
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 origin;
    float rotation;
    bool preserveModel;

    glm::mat4 getProjectionMatrix(float width, float height);
    glm::mat4 getModelMatrix();

private:
    void initRenderData();
};