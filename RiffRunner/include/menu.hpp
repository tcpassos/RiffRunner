#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <map>

#include "text_renderer.h"
#include "sound.h"

using namespace std;

class Menu
{
public:
    Menu(int screenWidth, int screenHeight, Font font = Font("assets/fonts/TribalBox.ttf", 52)) {
        this->screenSize = glm::vec2(screenWidth, screenHeight);
        this->position = glm::vec2(0);
        this->selectedItem = 0;
        this->textRenderer = new TextRenderer(screenWidth, screenHeight, font);
        this->font = &font;
        this->itemSpacing = 20;
        this->clickSound = new Sound("assets/sound/click.wav");
        this->customSound = nullptr;
    }

    void setSelectedItem(int index) {
        if (index >= 0 && index < menuItems.size()) {
            for (int i = 0; i < sortedItems.size(); i++) {
                if (sortedItems[i].originalIndex == index) {
                    selectedItem = i;
                }
            }
        }
    }

    void setPosition(unsigned int x, unsigned int y) {
        position = glm::vec2(x, y);
    }

    void setTextAlignment(TextAlignment textAlignment) {
        this->textRenderer->setHorizontalAlignment(textAlignment);
    }

    void setItemSpacing(unsigned int itemSpacing) {
        this->itemSpacing = itemSpacing;
    }

    int getItemIndex() {
        return sortedItems[selectedItem].originalIndex;
    }

    std::string getItemValue() {
        return sortedItems[selectedItem].item.label;
    }

    int addItem(string label, string groupName = "") {
        int newItemIndex = menuItems.size();
        menuItems.push_back({ label, true });
        sortedItems.push_back({ { label, true }, newItemIndex });

        auto groupIt = groups.find(groupName);
        // If group doesn't exist
        if (groupIt == groups.end()) {
            vector<int> group;
            group.push_back(newItemIndex);
            groups[groupName] = group;
        } else {
            vector<int> *group = &groupIt->second;
            group->push_back(newItemIndex);
        }

        return newItemIndex;
    }

    void clearItems() {
        groups.clear();
        sortedItems.clear();
        menuItems.clear();
    }

    void previous() {
        selectedItem--;

        if (selectedItem < 0) {
            selectedItem = sortedItems.size() - 1;
        }

        while (!sortedItems[selectedItem].item.selectable) {
            selectedItem--;
            if (selectedItem < 0) {
                selectedItem = sortedItems.size() - 1;
            }
        }

        clickSound->playOnce();
    }

    void next() {
        selectedItem++;

        if (selectedItem >= sortedItems.size()) {
            selectedItem = 0;
        }

        while (!sortedItems[selectedItem].item.selectable) {
            selectedItem++;
            if (selectedItem >= sortedItems.size()) {
                selectedItem = 0;
            }
        }

        clickSound->playOnce();
    }
    
    void playSound(std::string sound) {
        customSound = new Sound(sound.c_str());
        customSound->playOnce();
    }

    void sort() {
        sortedItems.clear();
        for (auto group : groups) {
            if (!group.first.empty()) {
                sortedItems.push_back({ { group.first, false }, -1 });
            }
            for (int itemIndex : group.second) {
                sortedItems.push_back({ menuItems[itemIndex], itemIndex });
            }
        }
        // Select the next valid item
        selectedItem = 0; next();
    }

    void draw() {
        unsigned int itemHeight = font->size + itemSpacing;
        unsigned int maximumItemsOnScreen = (int) ((screenSize.y - position.y - itemHeight) / itemHeight);
        
        unsigned int startOffset = selectedItem > maximumItemsOnScreen ? selectedItem - maximumItemsOnScreen : 0;
        unsigned int lastItem = startOffset + maximumItemsOnScreen + 1;
        if (lastItem > sortedItems.size()) lastItem = sortedItems.size();

        for (unsigned int i = startOffset; i < lastItem; i++) {
            
            // The selected item will be red and slightly larger
            glm::vec4 itemColor;
            if (sortedItems[i].item.selectable) {
                if (selectedItem == i) {
                    itemColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                } else {
                    itemColor = glm::vec4(1.0f);
                }
            } else {
                itemColor = glm::vec4(0.0f, 0.5f, 1.0f, 1.0f);
            }
            float itemScale = selectedItem == i ? 1.2f : 1.0f;            
            unsigned int itemPosition = position.y + (i - startOffset) * itemHeight - itemSpacing/2;

            textRenderer->setColor(itemColor);
            textRenderer->setScale(itemScale);
            textRenderer->renderText(sortedItems[i].item.label, position.x, itemPosition);
        }
    }

private:
    struct MenuItem {
        string label;
        bool selectable;
    };

    struct SortedMenuItem {
        MenuItem item;
        int originalIndex;
    };

    TextRenderer* textRenderer;
    Sound* clickSound;
    Sound* customSound;

    int selectedItem;
    vector<MenuItem> menuItems;
    vector<SortedMenuItem> sortedItems;
    map<string, vector<int>> groups;

    glm::vec2 screenSize;
    glm::vec2 position;

    Font* font;
    unsigned int itemSpacing;
};