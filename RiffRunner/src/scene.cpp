#include "scene.h"

void accept(GLFWwindow* window, SceneId id) {
    while (!glfwWindowShouldClose(window) && id != SceneExit) {
        switch (id) {
        case SceneTest:
            id = acceptTest(window);
            break;
        case SceneMenu:
            id = acceptMenu(window);
            break;
        case SceneMusicSelector:
            id = acceptMusicSelector(window);
            break;
        }
    }
}