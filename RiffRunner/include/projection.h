#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Projection {
public:
    Projection(glm::vec3 position, glm::vec3 rotation, float fieldOfView = glm::radians(120.0f), float nearPlane = 10.0f, float farPlane = 600.0f) {
        this->position = position;
        this->rotation = rotation;
        this->fieldOfView = fieldOfView;
        this->nearPlane = nearPlane;
        this->farPlane = farPlane;
    }

    void setPosition(glm::vec3 position) { this->position = position; };
    glm::vec3 getPosition() { return this->position; }
    void moveX(float value) { this->position = glm::vec3(position.x + value, position.y, position.z); }
    void moveY(float value) { this->position = glm::vec3(position.x, position.y + value, position.z); }
    void moveZ(float value) { this->position = glm::vec3(position.x, position.y, position.z + value); }

    void setRotation(glm::vec3 rotation) { this->rotation = rotation; };
    glm::vec3 getRotation() { return this->rotation; }

    void setFieldOfView(float fieldOfView) { this->fieldOfView = fieldOfView; };
    float getFieldOfView() { return this->fieldOfView; }

    void setNearPlane(float nearPlane) { this->nearPlane = nearPlane; };
    float getNearPlane() { return this->nearPlane; }
    void setFarPlane(float farPlane) { this->farPlane = farPlane; };
    float getFarPlane() { return this->farPlane; }

private:
    glm::vec3 position;
    glm::vec3 rotation;
    float fieldOfView;
    float nearPlane;
    float farPlane;
};

