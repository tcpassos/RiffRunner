#include <filesystem>
#include <string>
#include <vector>

#include "menu.h"
#include "resource_manager.h"
#include "scene.h"
#include "sprite.h"
#include "text_renderer.h"
#include "game_info.h"

namespace fs = std::filesystem;

// ======================================================================================
// Keyboard navigation keys callback
// ======================================================================================
void key_callback_music_selector(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Menu* menu = (Menu*)glfwGetWindowUserPointer(window);

    //Left and Right to select the song
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        menu->previous();
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        menu->next();
    }

    //Go to difficulty selector with ENTER
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        GameInfo::selectedSong = menu->getItemValue();
        accept(window, SceneDifficultySelector);
    }
    //Return to main menu with ESC
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        accept(window, SceneMenu);
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
        string path = it->path().string();
        if (path.find("!") == string::npos) {

            if (path.find("album.") != string::npos) {
                if (it->path().extension() == ".png" || (it->path().extension() == ".PNG") || (it->path().extension() == ".jpg") || (it->path().extension() == ".JPG")) {
                    string musicName = it->path().parent_path().filename().string();
                    Texture2D coverTexture = ResourceManager::LoadTexture(it->path().string().c_str(), musicName);
                    
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