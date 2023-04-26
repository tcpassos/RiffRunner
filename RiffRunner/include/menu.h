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

    void addItem(string label);
    void addItem(string label, Menu submenu);
    void draw();
    void moveUp();
    void moveDown();
    void enter(int submenuId);
    void back();
    int getItem() { return currentMenu->selectedItem; }

private:
    TextRenderer* textRenderer;

    Menu* previousMenu = nullptr;
    Menu* currentMenu = this;

    int selectedItem;
    vector<string> menuItens;
    map<int, Menu> submenus;

    int width;
    int heigth;

    int posX = 0;
    int posY = 0;
    int size = 52;
};