#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef enum SceneId {
    SceneCurrent,
    SceneLoading,
    SceneMenu,
    SceneMusicSelector,
    SceneDifficultySelector,
    SceneGame,
    SceneResults,
    SceneFailure,
    ScenePause,
    SceneExit
} SceneId;

SceneId acceptLoading(GLFWwindow* window);
SceneId acceptMenu(GLFWwindow* window);
SceneId acceptMusicSelector(GLFWwindow* window);
SceneId acceptDifficultySelector(GLFWwindow* window);
SceneId acceptGame(GLFWwindow* window);
SceneId acceptResults(GLFWwindow* window);
SceneId acceptFailure(GLFWwindow* window);
SceneId acceptPause(GLFWwindow* window);
void accept(GLFWwindow* window, SceneId id);