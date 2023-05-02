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
    textRenderer->Load(font, 52);
    clickSound.loadAudio("resources/sound/click.wav");
}

int Menu::addItem(string label) {
    menuItens.push_back(label);
    return menuItens.size() - 1;
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
            glm::vec3 itemColor = selectedItem == i ? glm::vec3{1.0f, 0.0f, 0.0f} : glm::vec3{1.0f, 1.0f, 1.0f};
            float itemScale = selectedItem == i ? 1.2f : 1.0f;
            textRenderer->RenderText(menuItens[i], x, y, itemScale, itemColor);
        }
    } else {
        textRenderer->RenderText(menuItens[this->selectedItem], posX, posY, 1.0f, glm::vec3{ 1.0f, 1.0f, 1.0f });
    }

}

void Menu::previous() {
    if (selectedItem - 1 >= 0) {
        selectedItem--;
        clickSound.loadAudio("resources/sound/click.wav");
        clickSound.play();
    }
}

void Menu::next() {
    if (selectedItem + 1 < menuItens.size()) {
        selectedItem++;
        clickSound.loadAudio("resources/sound/click.wav");
        clickSound.play();
    }
}

void Menu::playSound(std::string sound) {
    clickSound.loadAudio(sound.c_str());
    clickSound.play();
}

