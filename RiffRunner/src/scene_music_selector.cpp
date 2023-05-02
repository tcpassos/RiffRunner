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
#include "menu.h"

namespace fs = std::filesystem;

// ======================================================================================
// Keyboard navigation keys callback
// ======================================================================================
void key_callback_music_selector(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Menu* menu = (Menu*)glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        menu->previous();
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        menu->next();
    }
}

// ======================================================================================
// Music selector scene
// ======================================================================================
SceneId acceptMusicSelector(GLFWwindow* window) {

    // Screen size
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Music selector menu
    Menu musicSelectorMenu(width, height, width/2 - 30, 450, "resources/fonts/Queen of Clubs.otf");
    musicSelectorMenu.setAlignmentHorizontal();

    // Menu key callback
    glfwSetKeyCallback(window, key_callback_music_selector);
    glfwSetWindowUserPointer(window, &musicSelectorMenu);

    // Load album covers
    std::vector<Sprite> albumCovers;
    for (fs::recursive_directory_iterator it("resources\\music\\"), end; it != end; ++it) {
        std::string path = it->path().string();
        if (path.find("!") == std::string::npos) {

            if (path.find("album.") != std::string::npos) {
                if (it->path().extension() == ".png" || (it->path().extension() == ".PNG") ) {
                    string musicName = it->path().parent_path().filename().string();
                    Texture2D coverTexture = ResourceManager::LoadTexture(("resources/music/" + musicName + "/album.png").c_str(), musicName);
                    
                    Sprite cover(coverTexture);
                    float coverWidth = width * 0.4;
                    float coverHeight = height * 0.5;
                    cover.setPosition((width / 2) - (coverWidth / 2), height/6);
                    cover.setSize(coverWidth, coverHeight);
                    albumCovers.push_back(cover);

                    musicSelectorMenu.addItem(musicName);
                }
            }
        }
    }

    // Background
    Texture2D backgroundTexture = ResourceManager::LoadTexture("resources/img/music_selector_background.jpg", "musicSelectorBackground");
    Sprite backgroundImage(backgroundTexture);

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, width, height);
        glfwPollEvents();

        // Exit on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            return SceneMenu;

        // Clear color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        backgroundImage.draw(window);
        albumCovers[musicSelectorMenu.getItemIndex()].draw(window);
        musicSelectorMenu.draw();

        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}