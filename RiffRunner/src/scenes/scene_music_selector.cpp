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
// Sort menu items
// ======================================================================================
#define MENU_SORT_BY_ARTIST 0
#define MENU_SORT_BY_NAME 1
#define MENU_SORT_BY_FOLDER 2
int musicSelectSortType = 0;

void sortMenuItems(Menu* menu) {
    menu->clearItems();

    for (auto song : GameInfo::songs) {
        if (musicSelectSortType == MENU_SORT_BY_ARTIST) {
            menu->addItem(song.name, song.artist);
        } else if (musicSelectSortType == MENU_SORT_BY_NAME) {
            menu->addItem(song.name, song.name.substr(0, 1));
        } else if (musicSelectSortType == MENU_SORT_BY_FOLDER) {
            fs::path songPath(song.path);
            std::string songParent = songPath.parent_path().string();
            menu->addItem(song.name, songParent.substr(songParent.find("songs") + 5));
        }
    }

    menu->sort();

    if (GameInfo::selectedSong >= 0) {
        menu->setSelectedItem(GameInfo::selectedSong);
    } else {
        GameInfo::selectedSong = menu->getItemIndex();
    }
}

// ======================================================================================
// Keyboard navigation keys callback
// ======================================================================================
void key_callback_music_selector(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Menu* menu = (Menu*)glfwGetWindowUserPointer(window);

    // Left and right to change ordering
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        musicSelectSortType--;
        if (musicSelectSortType < 0) {
            musicSelectSortType = 2;
        }
        sortMenuItems(menu);
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        musicSelectSortType++;
        if (musicSelectSortType > 2) {
            musicSelectSortType = 0;
        }
        sortMenuItems(menu);
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
    }
    sortMenuItems(&musicSelectorMenu);

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
    backgroundImage.setSize(screenWidth, screenHeight);

    double lastPressTime = 0.0;

    while (!glfwWindowShouldClose(window)) {
        glViewport(0, 0, screenWidth, screenHeight);
        glfwPollEvents();

        double currentTime = glfwGetTime();

        // Up and Down to select the song
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && (currentTime - lastPressTime) > 0.1) {
            lastPressTime = currentTime;
            musicSelectorMenu.previous();
            GameInfo::selectedSong = musicSelectorMenu.getItemIndex();
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && (currentTime - lastPressTime) > 0.1) {
            lastPressTime = currentTime;
            musicSelectorMenu.next();
            GameInfo::selectedSong = musicSelectorMenu.getItemIndex();
        }
        
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