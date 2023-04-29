#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef enum SceneId {
    SceneMenu,
    SceneTest,
    SceneExit
} SceneId;

SceneId acceptMenu(GLFWwindow* window);
SceneId acceptTest(GLFWwindow* window);
void accept(GLFWwindow* window, SceneId id);