#include <iostream>

#include "scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include "resource_manager.h"
#include "sprite.h"

// ======================================================================================
// Test scene
// ======================================================================================
SceneId acceptTest(GLFWwindow* window) {

    Texture2D texture = ResourceManager::LoadTexture("resources/img/menu.jpg", "menu");
    Sprite sprite(texture);

    //sprite.setSize(glm::vec3(80, 60, 0));

    int x = 0;

    while (!glfwWindowShouldClose(window)) {
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glfwPollEvents();

        // Exit on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Clear color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
        glClear(GL_COLOR_BUFFER_BIT);
        // ==========================================================

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            x++;
        }

        //sprite.draw(window);
        //sprite.setPosition(glm::vec3(x, 0, 0));
        sprite.draw(window);

        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}