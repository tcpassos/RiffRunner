#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scenes.h"
#include "menu.h"

void key_callback_menu(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Menu* menu = (Menu*)glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        menu->moveUp();
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        menu->moveDown();
    }
}

SceneId acceptMenu(GLFWwindow* window) {
    // Menu config
    Menu menu(800, 600, 0, 0);
    menu.addItem("<Iniciar>");
    menu.addItem("<Opcoes>");
    menu.addItem("<Sair>");

    // Menu key callback
    glfwSetKeyCallback(window, key_callback_menu);
    glfwSetWindowUserPointer(window, &menu);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        menu.draw();

        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    return SceneExit;
}