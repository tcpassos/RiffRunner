#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include "text_renderer.h"
#include "resource_manager.h"
#include "menu.h"

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    // Create the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Riff Runner", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        return -1;
    }

    // Sets the window icon
    GLFWimage icons[1];
    icons[0].pixels = stbi_load("resources/img/icon.png", &icons[0].width, &icons[0].height, 0, 4);
    glfwSetWindowIcon(window, 1, icons);
    stbi_image_free(icons[0].pixels);

    //TextRenderer* textRenderer;
    //textRenderer = new TextRenderer(800, 600);
    //textRenderer->Load("resources/fonts/TribalBox.TTF", 40);

    // Run in loop until window is closed
    while (!glfwWindowShouldClose(window)) {
        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //textRenderer->RenderText("Batata e bom d+", 5.0f, 5.0f, 1.0f);

        Menu menu(800, 600, 0, 0);
        menu.addItem("<Iniciar>");
        menu.addItem("<Opcoes>");
        menu.addItem("<Sair>");
        menu.draw();

        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    // Delete all resources as loaded using the resource manager
    ResourceManager::Clear();
    glfwTerminate();
    return 0;
}