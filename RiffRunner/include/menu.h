#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <text_renderer.h>
#include <audio_player.h>

using namespace std;

class Menu
{
public:
    Menu(int screenWidth, int screenHeight, int initialPosX = 0, int initialPosY = 0, string font = "resources/fonts/TribalBox.ttf");

    int addItem(string label);
    void previous();
    void next();
    void setAlignmentVertical() { this->isVertical = true; };
    void setAlignmentHorizontal() { this->isVertical = false; };
    int getItemIndex() { return selectedItem; }
    void draw();

private:
    TextRenderer* textRenderer;
    AudioPlayer clickSound;

    int selectedItem;
    vector<string> menuItens;

    int width;
    int heigth;
    bool isVertical;

    int posX = 0;
    int posY = 0;
    int size = 52;
};