#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scenes.h"
#include "menu.h"

void key_callback_menu(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Menu* menu = (Menu*)glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        menu->moveUp();
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        menu->moveDown();
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        menu->enter(menu->getItem());
    }
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_BACKSPACE) && action == GLFW_PRESS) {
        menu->back();
    }
}

SceneId acceptMenu(GLFWwindow* window) {
    // Settings menu
    Menu settingsMenu(800, 600, 0, 0);
    settingsMenu.addItem("<Opcoes graficas>");
    settingsMenu.addItem("<Audio>");
    settingsMenu.addItem("<Voltar>");

    // Main menu
    Menu mainMenu(800, 600, 0, 0);
    mainMenu.addItem("<Iniciar>");
    mainMenu.addItem("<Opcoes>", settingsMenu);
    mainMenu.addItem("<Sair>");

    // Menu key callback
    glfwSetKeyCallback(window, key_callback_menu);
    glfwSetWindowUserPointer(window, &mainMenu);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mainMenu.draw();

        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    return SceneExit;
}