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
    const int MENU_MAIN_START    = mainMenu.addItem("<Iniciar>");
    const int MENU_MAIN_SETTINGS = mainMenu.addItem("<Opcoes>");
    const int MENU_MAIN_EXIT     = mainMenu.addItem("<Sair>");

    // Settings menu
    Menu settingsMenu(width, height, 0, 0);
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
    Sound menuMusic("resources/sound/menu.wav");
    menuMusic.setVolume(50);
    menuMusic.play();

    // Background image
    Sprite* menuImage = new Sprite(ResourceManager::LoadTexture("resources/img/menu.jpg", "menu"));

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
            int selected = menu->getItemIndex();

            switch (menuType) {
            /* MAIN MENU  --------------------------------- */
            case MenuTypeMain:
                if (selected == MENU_MAIN_START) {
                    return SceneMusicSelector;
                    glfwSetWindowShouldClose(window, true);
                // Enter settings menu
                } else if (selected == MENU_MAIN_SETTINGS) {
                    menuType = MenuTypeSettings;
                    menu = &settingsMenu;
                // Exit program
                } else if (selected == MENU_MAIN_EXIT) {
                    return SceneExit;
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

        menuImage->draw(window);
        menu->draw();

        // Swap front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    return SceneExit;
}