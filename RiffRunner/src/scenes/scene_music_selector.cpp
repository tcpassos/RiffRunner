#include <string>
#include <vector>

#include "menu.hpp"
#include "resource_manager.h"
#include "scene.h"
#include "sprite.hpp"
#include "text_renderer.h"
#include "game_info.h"

namespace fs = std::filesystem;

// ======================================================================================
// Keyboard navigation keys callback
// ======================================================================================
void key_callback_music_selector(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Menu* menu = (Menu*)glfwGetWindowUserPointer(window);

    //Left and Right to select the song
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        menu->previous();
        GameInfo::selectedSong = menu->getItemIndex();
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        menu->next();
        GameInfo::selectedSong = menu->getItemIndex();
    }

    //Go to difficulty selector with ENTER
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
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

    // If there is no song to load
    if (GameInfo::songs.empty()) {
        std::cout << "Song list is empty!" << std::endl;
        return SceneMenu;
    }

    // Screen size
    int screenWidth, screenHeight;
    glfwGetWindowSize(window, &screenWidth, &screenHeight);

    // Music selector menu
    Menu musicSelectorMenu(screenWidth, screenHeight, Font("assets/fonts/Gobold Regular.otf", 25));
    musicSelectorMenu.setItemSpacing(10);
    musicSelectorMenu.setPosition(screenWidth * 0.05, screenHeight * 0.05);
    musicSelectorMenu.setTextAlignment(TextLeft);

    // Menu key callback
    glfwSetKeyCallback(window, key_callback_music_selector);
    glfwSetWindowUserPointer(window, &musicSelectorMenu);

    // Load album cover textures
    std::vector<Texture2D> albumCoverTextures;
    for (auto song : GameInfo::songs) {
        albumCoverTextures.push_back(ResourceManager::getTexture(song.path));
        musicSelectorMenu.addItem(song.name, song.artist);
    }
    musicSelectorMenu.sort();
    if (GameInfo::selectedSong >= 0) {
        musicSelectorMenu.setSelectedItem(GameInfo::selectedSong);
    } else {
        GameInfo::selectedSong = musicSelectorMenu.getItemIndex();
    }

    // Album cover
    Sprite albumCover(albumCoverTextures[GameInfo::selectedSong]);
    float coverWidth = screenWidth * 0.25;
    float coverHeight = screenHeight * 0.3;
    albumCover.setPosition((screenWidth * 3 / 5), screenHeight / 8);
    albumCover.setSize(coverWidth, coverHeight);

    TextRenderer textRenderer(screenWidth, screenHeight, Font("assets/fonts/Gobold Regular.otf", 22));
    textRenderer.setHorizontalAlignment(TextLeft);

    // Background
    Texture2D backgroundTexture = ResourceManager::loadTexture("assets/img/music_selector_background.jpg", "musicSelectorBackground");
    Sprite backgroundImage(backgroundTexture);

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, screenWidth, screenHeight);
        glfwPollEvents();

        // Clear color buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        backgroundImage.draw(window);
        musicSelectorMenu.draw();
        albumCover.setTexture(albumCoverTextures[musicSelectorMenu.getItemIndex()]);
        albumCover.draw(window);
        

        SongInfo info = GameInfo::songs[GameInfo::selectedSong];
        textRenderer.renderText(info.artist, albumCover.getBounds().left, albumCover.getBounds().height + 30);
        textRenderer.renderText(info.name, albumCover.getBounds().left, albumCover.getBounds().height + 60);
        textRenderer.renderText(info.album, albumCover.getBounds().left, albumCover.getBounds().height + 90);

        // ==========================================================
        // Switch buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}