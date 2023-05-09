#include <menu.h>

using namespace std;

Menu::Menu(int screenWidth, int screenHeigth, int initialPosX, int initialPosY, string font) {
    width = screenWidth;
    heigth = screenHeigth;
    posX = initialPosX;
    posY = initialPosY;
    isVertical = true;
    selectedItem = 0;
    textRenderer = new TextRenderer(width, heigth);
    textRenderer->load(font, 52);
    clickSound = new Sound("assets/sound/click.wav");
}

int Menu::addItem(string label) {
    menuItens.push_back(label);
    return menuItens.size() - 1;
}

void Menu::playSound(std::string sound) {
    customSound = new Sound(sound.c_str());
    customSound->playOnce();
}

void Menu::draw() {
    int x, y;

    if (this->isVertical) {
        for (int i = 0; i < menuItens.size(); i++) {
            if (posX == 0 && posY == 0){
                x = width / 2;
                y = heigth / 2 - menuItens.size() * 38 + i * 76;
            } else {
                x = posX;
                y = posY + i * 76;
            }

            // The selected item will be red and slightly larger
            glm::vec4 itemColor = selectedItem == i ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(1.0f);
            float itemScale = selectedItem == i ? 1.2f : 1.0f;
            textRenderer->setColor(itemColor);
            textRenderer->setScale(itemScale);
            textRenderer->renderText(menuItens[i], x, y);
        }
    } else {
        textRenderer->renderText(menuItens[this->selectedItem], posX, posY);
    }

}

void Menu::previous() {
    if (selectedItem - 1 >= 0) {
        selectedItem--;
        clickSound->playOnce();
    }
}

void Menu::next() {
    if (selectedItem + 1 < menuItens.size()) {
        selectedItem++;
        clickSound->playOnce();
    }
}

