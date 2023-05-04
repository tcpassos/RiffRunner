#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef enum SceneId {
    SceneMenu,
    SceneMusicSelector,
    SceneDifficultySelector,
    SceneGame,
    SceneFailure,
    SceneExit
} SceneId;

SceneId acceptMenu(GLFWwindow* window);
SceneId acceptMusicSelector(GLFWwindow* window);
SceneId acceptGame(GLFWwindow* window);
SceneId acceptDifficultySelector(GLFWwindow* window);
SceneId acceptFailure(GLFWwindow* window);
void accept(GLFWwindow* window, SceneId id);