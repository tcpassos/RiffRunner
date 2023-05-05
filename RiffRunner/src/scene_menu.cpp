#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "menu.h"
#include "rect.h"
#include "resource_manager.h"
#include "scene.h"
#include "sound.h"
#include "sprite.h"

const int MENU_MAIN_START    = 0;
const int MENU_MAIN_SETTINGS = 1;
const int MENU_MAIN_EXIT     = 2;

const int MENU_SETTINGS_GRAPHICS = 0;
const int MENU_SETTINGS_AUDIO    = 1;
const int MENU_SETTINGS_BACK     = 2;


/* Enum used to identify the current menu */
typedef enum MenuType {
    MenuTypeMain,
    MenuTypeSettings
} MenuType;

// Current menu
MenuType menuType;

// ======================================================================================
// Keyboard navigation keys callback
// ======================================================================================
void key_callback_menu(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Menu** menuPtr = (Menu**)glfwGetWindowUserPointer(window);
    Menu* menu = *menuPtr;

    // Navigation
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        menu->previous();
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        menu->next();
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS){

        int selected = menu->getItemIndex();

        switch (menuType) {
            case MenuTypeMain:
                if (selected == MENU_MAIN_START) {
                    Sound::stopAll();
                    accept(window, SceneMusicSelector);
                }
                else if (selected == MENU_MAIN_SETTINGS)
                    menuType = MenuTypeSettings;
                else if (selected == MENU_MAIN_EXIT)
                    accept(window, SceneExit);
                break;
            case MenuTypeSettings:
                if (selected == MENU_SETTINGS_BACK)
                    menuType = MenuTypeMain;
                break;
        }
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        accept(window, SceneExit);
}

// ======================================================================================
// Menu scene
// ======================================================================================
SceneId acceptMenu(GLFWwindow* window) {
    
    // Screen size
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Main menu
    Menu mainMenu(width, height, 0, 0);
    mainMenu.addItem("<Iniciar>");
    mainMenu.addItem("<Opcoes>");
    mainMenu.addItem("<Sair>");

    // Settings menu
    Menu settingsMenu(width, height, 0, 0);
    settingsMenu.addItem("<Opcoes graficas>");
    settingsMenu.addItem("<Audio>");
    settingsMenu.addItem("<Voltar>");

    // Current menu
    menuType = MenuTypeMain;
    Menu* menu = &mainMenu;

    // Menu key callback
    glfwSetKeyCallback(window, key_callback_menu);
    glfwSetWindowUserPointer(window, &menu);

    // Menu music
    Sound menuMusic("resources/sound/menu.wav");
    menuMusic.setVolume(50);
    menuMusic.play();

    // Background image
    Sprite* menuImage = new Sprite(ResourceManager::LoadTexture("resources/img/menu.jpg", "menu"));

    // Indicates that an item has been selected from the menu with enter
    MenuType previousMenuType;

    // Menu loop
    while (!glfwWindowShouldClose(window)) {

        previousMenuType = menuType;

        // Poll for and process events
        glfwPollEvents();
// ======================================================================================
//      > Process selected menu item
// ======================================================================================
        if (previousMenuType != menuType) {
        
            switch (menuType) {
                case MenuTypeMain:
                    menu = &mainMenu;
                    break;
                case MenuTypeSettings:
                    menu = &settingsMenu;
                    break;
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        menuImage->draw(window);
        menu->draw();

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    return SceneExit;
}