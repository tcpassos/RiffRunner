#include "menu.h"
#include "resource_manager.h"
#include "scene.h"
#include "sound.h"
#include "sprite.h"

const int PAUSE_CONTINUE = 0;
const int PAUSE_RESTART = 1;
const int PAUSE_EXIT = 2;

// ======================================================================================
// Keyboard navigation keys callback
// ======================================================================================
void key_callback_pause(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Menu* menu = (Menu*)glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        menu->previous();
        
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        menu->next();
    }

    //Deal with selected options when ENTER is pressed
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        Sound::stopAll();
        switch (menu->getItemIndex()) {
            case PAUSE_CONTINUE:
                accept(window, SceneGame);
                break;
            case PAUSE_RESTART:
                accept(window, SceneGame);
                break;
            case PAUSE_EXIT:
                accept(window, SceneMenu);
                break;
        }
    }
    //Return to menu
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        Sound::stopAll();
        accept(window, SceneMenu);
    }

}

// ======================================================================================
// Failure scene
// ======================================================================================
SceneId acceptPause(GLFWwindow* window) {

    // Screen size
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Background
    Texture2D backgroundTexture = ResourceManager::LoadTexture("resources/img/pauseMenu.png", "Pause");
    Sprite backgroundImage(backgroundTexture);
    backgroundImage.setOrigin(glm::vec2(backgroundTexture.Width / 2, backgroundTexture.Height / 2));
    backgroundImage.setPosition(glm::vec2(width / 2, height / 2));

    // Pause menu
    Menu pauseMenu(width, height, 0, 0);

    pauseMenu.addItem("<Continuar>");
    pauseMenu.addItem("<Reiniciar>");
    pauseMenu.addItem("<Desistir>");

    // Menu key callback
    glfwSetKeyCallback(window, key_callback_pause);
    glfwSetWindowUserPointer(window, &pauseMenu);

    Sound::stopAll();

    while (!glfwWindowShouldClose(window)) {

        glViewport(0, 0, width, height);
        glfwPollEvents();

        // Clear color buffer
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        backgroundImage.draw(window);
        pauseMenu.draw();
        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}