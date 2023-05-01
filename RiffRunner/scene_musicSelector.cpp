#include <iostream>

#include <map>
#include "scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include "resource_manager.h"
#include "sprite.h"
#include <filesystem>
#include <vector>
#include <string.h>
#include "audio_player.h"
#include "text_renderer.h"

namespace fs = std::filesystem;


// ======================================================================================
// Test scene
// ======================================================================================
SceneId acceptMusicSelector(GLFWwindow* window) {

    int width, height;

    glfwGetFramebufferSize(window, &width, &height);

    AudioPlayer click;
    click.loadAudio("resources/sound/click.wav");

    TextRenderer textRenderer(width, height);
    textRenderer.Load("resources/fonts/Queen of Clubs.otf", 45);

    int cont_dir = 0;
    int indice = 0;
    std::vector<std::string> musicDir;
    std::vector<Sprite> albumCoverSprite;

    for (fs::recursive_directory_iterator it("resources\\music\\"), end; it != end; ++it) {
        std::string path = it->path().string();
        if (path.find("!") == std::string::npos) {

            if (path.find("album.") != std::string::npos) {
                if (it->path().extension() == ".png" || (it->path().extension() == ".PNG") ) {
                    Texture2D texture = ResourceManager::LoadTexture(("resources/music/" + it->path().parent_path().filename().string() + "/album.png").c_str(), it->path().parent_path().filename().string().c_str());
                    Sprite cover(texture);
                    cover.setOrigin(glm::vec3(-((texture.Width * 0.6) / 2), -(texture.Height * 0.6), 0.0));
                    cover.setPosition(glm::vec3(width/2, height/3*2, 0.0));
                    cover.setSize(glm::vec3(texture.Width*0.6, texture.Height * 0.6, 1));
                    albumCoverSprite.push_back(cover);

                    musicDir.push_back(it->path().parent_path().filename().string());
                }
            }
        }
    }

    //background
    Texture2D texture = ResourceManager::LoadTexture("resources/img/fundo.jpg", "fundo");
    Sprite sprite(texture);

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, width, height);
        glfwPollEvents();

        // Exit on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            indice--;
            if (indice < 0) indice = musicDir.size() - 1;
            click.play();
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            indice++;
            if (indice > musicDir.size() - 1) indice = 0;
            click.play();
        }

        // Clear color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
        glClear(GL_COLOR_BUFFER_BIT);

        sprite.draw(window);
        albumCoverSprite[indice].draw(window);

        textRenderer.RenderText(musicDir[indice], width/2, height/5*4, 1.0f, glm::vec3{ 1.0f, 1.0f, 1.0f });
        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}