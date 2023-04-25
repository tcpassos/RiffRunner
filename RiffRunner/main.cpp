#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    /* Cria a janela */
    window = glfwCreateWindow(800, 600, "Riff Runner", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Define o ícone da janela
    GLFWimage icons[1];
    icons[0].pixels = stbi_load("resources/img/icon.png", &icons[0].width, &icons[0].height, 0, 4);
    glfwSetWindowIcon(window, 1, icons);
    stbi_image_free(icons[0].pixels);

    glfwMakeContextCurrent(window);

    /* Executa em loop até a janela ser fechada */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}