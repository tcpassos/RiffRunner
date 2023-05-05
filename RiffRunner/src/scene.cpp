#include "scene.h"

void accept(GLFWwindow* window, SceneId id) {
    while (!glfwWindowShouldClose(window)) {
        switch (id) {
        case SceneMenu:
            id = acceptMenu(window);
            break;
        case SceneMusicSelector:
            id = acceptMusicSelector(window);
            break;
        case SceneDifficultySelector:
            id = acceptDifficultySelector(window);;
            break;
        case SceneGame:
            id = acceptGame(window);
            break;
        case SceneFailure:
            id = acceptFailure(window);
            break;
        case ScenePause:
            id = acceptPause(window);
            break;
        case SceneExit:
            glfwSetWindowShouldClose(window, true);
            break;
        }
    }
}