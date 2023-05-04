#include "menu.h"
#include "resource_manager.h"
#include "scene.h"
#include "sound.h"
#include "sprite.h"

// ======================================================================================
// Keyboard navigation keys callback
// ======================================================================================
void key_callback_failure(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Menu* menu = (Menu*)glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        menu->previous();
        
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        menu->next();
    }
}

// ======================================================================================
// Failure scene
// ======================================================================================
SceneId acceptFailure(GLFWwindow* window) {

    // Screen size
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Failure menu
    Menu failureMenu(width, height, width / 12 * 3, height / 8 * 6);

    const int FAILURE_RESTART = failureMenu.addItem("<Reiniciar>");
    const int FAILURE_GIVEUP = failureMenu.addItem("<Desistir>");

    // Menu key callback
    glfwSetKeyCallback(window, key_callback_failure);
    glfwSetWindowUserPointer(window, &failureMenu);

    // Background
    Texture2D backgroundTexture = ResourceManager::LoadTexture("resources/img/sadPepe.jpg", "Failure");
    Sprite backgroundImage(backgroundTexture);

    // Failure audios
    Sound failureSong("resources/sound/musicaTristeDoNaruto.wav");
    failureSong.play();
    Sound failureAudio("resources/sound/rocky.wav");
    failureAudio.setVolume(45);
    failureAudio.play();
    Sound naoConsegue("resources/sound/naoconsegue.wav");


    while (!glfwWindowShouldClose(window)) {
        int oldOption = failureMenu.getItemIndex();

        glViewport(0, 0, width, height);
        glfwPollEvents();

        if (failureMenu.getItemIndex() != oldOption && failureMenu.getItemIndex() == 1)
            naoConsegue.play();

        // Volta ao menu
        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && failureMenu.getItemIndex() == FAILURE_GIVEUP)
            return SceneMenu;

        // Clear color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        backgroundImage.draw(window);
        failureMenu.draw();
        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}