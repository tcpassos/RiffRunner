#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "scenes.h"
#include "menu.h"
#include "audio_player.h"
#include "resource_manager.h"
#include "sprite.h"

// ======================================================================================
// Keyboard navigation keys callback
// ======================================================================================
void key_callback_menu(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Menu** menuPtr = (Menu**)glfwGetWindowUserPointer(window);
    Menu* menu = *menuPtr;

    // Navigation
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        menu->moveUp();
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        menu->moveDown();
    }
}

// ======================================================================================
// Menu scene
// ======================================================================================
SceneId acceptMenu(GLFWwindow* window) {
    // Main menu
    Menu mainMenu(800, 600, 0, 0);
                                   mainMenu.addItem("<Iniciar>");
    const int MENU_MAIN_SETTINGS = mainMenu.addItem("<Opcoes>");
    const int MENU_MAIN_EXIT     = mainMenu.addItem("<Sair>");

    // Settings menu
    Menu settingsMenu(800, 600, 0, 0);
                                   settingsMenu.addItem("<Opcoes graficas>");
                                   settingsMenu.addItem("<Audio>");
    const int MENU_SETTINGS_BACK = settingsMenu.addItem("<Voltar>");

    /* Enum used to identify the current menu */
    typedef enum MenuType {
        MenuTypeMain,
        MenuTypeSettings
    } MenuType;

    // Current menu
    MenuType menuType = MenuTypeMain;
    Menu* menu = &mainMenu;

    // Menu key callback
    glfwSetKeyCallback(window, key_callback_menu);
    glfwSetWindowUserPointer(window, &menu);

    // Menu music
    AudioPlayer menuMusic;
    menuMusic.loadAudio("resources/sound/menu.wav");
    menuMusic.play();

    // Background image
    Sprite* menuImage = new Sprite();
    ResourceManager::LoadTexture("resources/img/menu.jpg", "menu");
    menuImage->setTexture(ResourceManager::GetTexture("menu"));

    // Indicates that an item has been selected from the menu with enter
    bool enterKeyPressed = false;

    // Menu loop
    while (!glfwWindowShouldClose(window)) {

// ======================================================================================
//      > Process selected menu item
// ======================================================================================

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !enterKeyPressed) {
            enterKeyPressed = true;

            // Get selected menu option index
            int selected = menu->getItem();

            switch (menuType) {
            /* MAIN MENU  --------------------------------- */
            case MenuTypeMain:
                // Enter settings menu
                if (selected == MENU_MAIN_SETTINGS) {
                    menuType = MenuTypeSettings;
                    menu = &settingsMenu;
                // Exit program
                } else if (selected == MENU_MAIN_EXIT) {
                    glfwSetWindowShouldClose(window, true);
                }
                break;
            /* SETTINGS MENU ------------------------------ */
            case MenuTypeSettings:
                // Return to main menu
                if (selected == MENU_SETTINGS_BACK) {
                    menuType = MenuTypeMain;
                    menu = &mainMenu;
                }
                break;
            }

        } else if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
            enterKeyPressed = false;
        }
        
// ======================================================================================
//      > Render
// ======================================================================================

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        menu->draw();
        menuImage->draw(window);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    return SceneExit;
}