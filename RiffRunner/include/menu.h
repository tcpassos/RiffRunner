#pragma once

#include <iostream>
#include <vector>
#include <text_renderer.h>

#define MAX_NUMBER_OF_ITEMS 3

using namespace std;

class Menu
{
public:
    Menu(int screenWidth, int screenHeight, int initialPosX = 0, int initialPosY = 0);

    void addItem(string item);
    void setString(string str, int pos);
    void setSize(int newSize) { size = newSize; }
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