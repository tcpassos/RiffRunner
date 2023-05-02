#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include "text_renderer.h"
#include "resource_manager.h"
#include "scene.h"

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

    // Run scenes
    accept(window, SceneFailure);

    // Delete all resources as loaded using the resource manager
    ResourceManager::Clear();
    glfwTerminate();
    return 0;
}