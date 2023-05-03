#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "resource_manager.h"
#include "shape.h"

class RectangleShape : public Shape {
public:
	RectangleShape(unsigned int width, unsigned int heigth);
	void draw(GLFWwindow* window);

private:
	Shader shader;
};