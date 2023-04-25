#include <menu.h>

using namespace std;

Menu::Menu(int screenWidth, int screenHeigth, int initialPosX, int initialPosY) {
    width = screenWidth;
    heigth = screenHeigth;
    posX = initialPosX;
    posY = initialPosY;
    selectedItem = 0;
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
    // TODO: Implement
}
void Menu::moveUp()
{
    // TODO: Implement
}
void Menu::moveDown() {
    // TODO: Implement
}
