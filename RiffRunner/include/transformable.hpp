#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "projection.h"

class Transformable {
public:
	Transformable() {
        // Calculates the required distance from the view on the z-axis to be able to see the full frame
        int screenWidth, screenHeight;
        glfwGetWindowSize(glfwGetCurrentContext(), &screenWidth, &screenHeight);
        float distanceZ = (screenHeight / 2.0f) / glm::tan(glm::radians(120.0) / 2.0f);

		this->position = glm::vec2(0.0f);
		this->size = glm::vec2(0);
		this->origin = glm::vec2(0.0f);
		this->rotation = 0.0f;
        this->projection = new Projection(glm::vec3(0.0f, 0.0f, -distanceZ), glm::vec3(0.0f), glm::radians(120.0), 10.0, screenHeight);
        this->preserveModel = false;
	}

    void setPosition(glm::vec2 position) {
        this->position = position;
    }

    void setPosition(float x, float y) {
        this->position.x = x;
        this->position.y = y;
    }

    void setSize(glm::vec2 size) {
        this->size = size;
    }

    void setSize(float width, float height) {
        this->size.x = width;
        this->size.y = height;
    }

    void setRotation(float rotation) {
        this->rotation = rotation;
    }

    void setOrigin(glm::vec2 origin) {
        this->origin = origin;
    }

    void setOrigin(float x, float y) {
        this->origin.x = x; this->origin.y = y;
    }

    void setProjection(Projection& projection, bool preserveModel = false) {
        this->projection = &projection;
        this->preserveModel = preserveModel;
    }

    glm::vec2 getPosition() {
        return this->position;
    }
    
    glm::vec2 getSize() {
        return this->size;
    }

    float getRotation() {
        return this->rotation;
    }

    glm::vec2 getOrigin() {
        return this->origin;
    }

    Projection* getProjection() {
        return this->projection;
    }

    void move(float x, float y) {
        moveX(x); moveY(y);
    }

    void moveX(float movement) {
        this->position = glm::vec2(this->position.x + movement, this->position.y);
    }
    
    void moveY(float movement) {
        this->position = glm::vec2(this->position.x, this->position.y + movement);
    }

    void rotate(float rotation) {
        this->rotation = this->rotation + rotation;
    }

protected:
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2 origin;
	float rotation;
	Projection* projection;
    bool preserveModel;
};