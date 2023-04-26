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

void Menu::addItem(string item) {
    menuItens.push_back(item);
}

void Menu::setString(string str, int pos) {
    if (menuItens.size() > pos) {
        menuItens[pos] = str;
    }
}

void Menu::draw() {
    int x, y;

    for (int i = 0; i < menuItens.size(); i++) {
        if (posX == 0 && posY == 0){
            x = width / 2;
            y = heigth / 2 - menuItens.size() * 38 + i * 76;
        } 
        else {
            x = posX;
            y = posY + i * 76;
        }

        // The selected item will be red and slightly larger
        glm::vec3 itemColor = selectedItem == i ? glm::vec3{1.0f, 0.0f, 0.0f} : glm::vec3{1.0f, 1.0f, 1.0f};
        float itemScale = selectedItem == i ? 1.2f : 1.0f;
        textRenderer->RenderText(menuItens[i], x, y, itemScale, itemColor);
    }
}
void Menu::moveUp() {
    if (selectedItem - 1 >= 0)
        selectedItem--;
}

void Menu::moveDown() {
    if (selectedItem + 1 < menuItens.size())
        selectedItem++;
}
