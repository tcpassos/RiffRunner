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

void Menu::init() {
    // TODO: Implement
    
}

void Menu::draw()
{
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

        textRenderer->RenderText(menuItens[i], x, y, 1.0f);

    }
}
void Menu::moveUp()
{
    // TODO: Implement
}
void Menu::moveDown() {
    // TODO: Implement
}
