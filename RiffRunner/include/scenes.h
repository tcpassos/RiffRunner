#pragma once

typedef enum SceneId {
    SceneMenu,
    SceneExit
} SceneId;

SceneId acceptMenu(GLFWwindow* window);