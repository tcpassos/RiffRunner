#include <iostream>

#include <string>
#include "scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include "resource_manager.h"
#include "sprite.h"
#include <vector>
#include <string.h>
#include "audio_player.h"
#include "text_renderer.h"
#include "menu.h"

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

    AudioPlayer failureSong;
    failureSong.loadAudio("resources/sound/musicaTristeDoNaruto.wav");
    failureSong.play();

    AudioPlayer failureAudio;
    failureAudio.loadAudio("resources/sound/rocky.wav");
    failureAudio.setVolume(45);
    failureAudio.play();


    while (!glfwWindowShouldClose(window)) {
        int oldOption = failureMenu.getItemIndex();

        glViewport(0, 0, width, height);
        glfwPollEvents();

        if (failureMenu.getItemIndex() != oldOption && failureMenu.getItemIndex() == 1)
            failureMenu.playSound("resources/sound/naoconsegue.wav");

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