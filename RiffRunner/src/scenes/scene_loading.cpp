#include <thread>
#include <chrono>

#include "game_info.h"
#include "resource_manager.h"
#include "scene.h"
#include "text_renderer.h"

bool texturesLoaded = false;

void loadTextures(GLFWwindow* sharedContext) {
    glfwMakeContextCurrent(sharedContext);

    // Load album cover textures
    GameInfo::loadSongs();
    for (auto song : GameInfo::songs) {
        std::string albumCoverImage;
        for (fs::directory_iterator it(song.path), end; it != end; ++it) {
            if (it->path().extension() == ".png" || (it->path().extension() == ".PNG") || (it->path().extension() == ".jpg") || (it->path().extension() == ".JPG")) {
                albumCoverImage = it->path().string();
                break;
            }
        }
        ResourceManager::loadTexture(albumCoverImage.c_str(), song.path);
    }

    texturesLoaded = true;
    glfwMakeContextCurrent(nullptr);
}

SceneId acceptLoading(GLFWwindow* window) {
    // Create the loading thread context and hide it
    GLFWwindow* sharedContext = glfwCreateWindow(1, 1, "", nullptr, window);
    std::thread textureThread(loadTextures, sharedContext);
    glfwHideWindow(sharedContext);

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    
    // Text renderer
    TextRenderer loadingTextRenderer(windowWidth, windowHeight, Font("assets/fonts/Queen of Clubs.otf", 60));

    int counter = 0;

    // Loop until textures are loaded
    while (!texturesLoaded) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        std::string loadingText = "CARREGANDO";
        if (++counter > 3) counter = 0;
        for (int i = 0; i < counter; i++) loadingText += ".";
        loadingTextRenderer.renderText(loadingText, windowWidth / 2, windowHeight / 2);

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        glfwSwapBuffers(window);
    }

    textureThread.join();
    glfwDestroyWindow(sharedContext);
    return SceneMenu;
}