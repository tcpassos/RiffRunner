#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <text_renderer.h>

using namespace std;

class Menu
{
public:
    Menu(int screenWidth, int screenHeight, int initialPosX = 0, int initialPosY = 0);

    int addItem(string label);
    void draw();
    void moveUp();
    void moveDown();
    int getItem() { return selectedItem; }

private:
    TextRenderer* textRenderer;

    int selectedItem;
    vector<string> menuItens;

    int width;
    int heigth;

    int posX = 0;
    int posY = 0;
    int size = 52;
};