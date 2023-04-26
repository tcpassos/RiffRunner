#include <menu.h>

using namespace std;

Menu::Menu(int screenWidth, int screenHeigth, int initialPosX, int initialPosY) {
    width = screenWidth;
    heigth = screenHeigth;
    posX = initialPosX;
    posY = initialPosY;
    selectedItem = 0;
    textRenderer = new TextRenderer(width, heigth);
    textRenderer->Load("resources/fonts/TribalBox.ttf", 52);
}

void Menu::addItem(string label) {
    menuItens.push_back(label);
}

void Menu::addItem(string label, Menu submenu) {
    submenus.insert(make_pair((int)menuItens.size(), submenu));
    addItem(label);
}

void Menu::draw() {
    int x, y;

    for (int i = 0; i < currentMenu->menuItens.size(); i++) {
        if (currentMenu->posX == 0 && currentMenu->posY == 0){
            x = currentMenu->width / 2;
            y = currentMenu->heigth / 2 - currentMenu->menuItens.size() * 38 + i * 76;
        }
        else {
            x = currentMenu->posX;
            y = currentMenu->posY + i * 76;
        }

        // The selected item will be red and slightly larger
        glm::vec3 itemColor = currentMenu->selectedItem == i ? glm::vec3{1.0f, 0.0f, 0.0f} : glm::vec3{1.0f, 1.0f, 1.0f};
        float itemScale = currentMenu->selectedItem == i ? 1.2f : 1.0f;
        textRenderer->RenderText(currentMenu->menuItens[i], x, y, itemScale, itemColor);
    }
}

void Menu::moveUp() {
    if (currentMenu->selectedItem - 1 >= 0)
        currentMenu->selectedItem--;
}

void Menu::moveDown() {
    if (currentMenu->selectedItem + 1 < currentMenu->menuItens.size())
        currentMenu->selectedItem++;
}

void Menu::enter(int submenuId) {
    auto it = currentMenu->submenus.find(submenuId);
    if (it != currentMenu->submenus.end()) {
        Menu* enterMenu = &currentMenu->submenus.at(submenuId);
        enterMenu->previousMenu = currentMenu;
        currentMenu = enterMenu;
    }
}

void Menu::back() {
    if (currentMenu->previousMenu != nullptr) {
        currentMenu = currentMenu->previousMenu;
    }
}