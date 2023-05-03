#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "sprite.h"
#include "rect.h"
#include "rectangle_shape.h"
#include "resource_manager.h"

class SongNote {
public:
    SongNote(Sprite &track, unsigned int value, double startTime, double duration = 0);
    bool checkInput(unsigned int input);
    bool intersects(Rect rect);
    bool isBefore(Rect rect);
    bool isAfter(Rect rect);
    bool isDisabled();
    bool hasTail();
    void disable();
    Rect getBounds();
    void move(int value);
    void draw(GLFWwindow* window);

private:
    unsigned int value;
    double startTime;
    double duration;
    bool disabled;
    Sprite* note;
    RectangleShape* tail;
};

SongNote::SongNote(Sprite &track, unsigned int value, double startTime, double duration) {
    this->value = value;
    this->startTime = startTime;
    this->duration = duration;
    this->disabled = false;
    
    // Note
    Texture2D noteTexture = ResourceManager::LoadTexture("resources/img/notes.png", "notes");
    this->note = new Sprite(noteTexture);

    unsigned int noteIndex = log2(value); // 1=0 2=1 4=2 8=3 16=4
    float noteSize = 40.0f;
    float noteWidth = noteSize / 5;
    float notePosition = track.getBounds().left + (noteIndex * track.getSize().x / 5) + track.getSize().x / 10;
    Rect noteTextureRect(glm::vec2(noteIndex * noteWidth, 0.0f), glm::vec2((noteIndex+1) * noteWidth, noteSize / 6));
    
    this->note->setSize(glm::vec2(noteSize));
    this->note->setOrigin(this->note->getSize().x / 2, 0.0f);
    this->note->setPosition(notePosition, 0.0f);
    this->note->setTextureRect(noteTextureRect);
    //this->note->setProjection(*track.getProjection());
    this->note->getProjection()->setRotation(glm::vec3(glm::radians(-50.0f), 0.0f, 0.0f));
    this->note->getProjection()->moveY(-100.0f);
    this->note->getProjection()->moveZ(-55.0f);

    // Tail
    if (duration > 0) {
        this->tail = new RectangleShape(10, duration * 10);
        this->tail->setOrigin(this->tail->getSize().x / 2, this->tail->getSize().y);
        this->tail->setPosition(this->note->getPosition().x, 0.0f);
        this->tail->getProjection()->setRotation(glm::vec3(glm::radians(-50.0f), 0.0f, 0.0f));
        this->tail->getProjection()->moveY(-100.0f);
        this->tail->getProjection()->moveZ(-55.0f);
        switch (value) {
            case 1: this->tail->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.7f)); break;
            case 2: this->tail->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.7f)); break;
            case 4: this->tail->setColor(glm::vec4(1.0f, 1.0f, 0.0f, 0.7f)); break;
            case 8: this->tail->setColor(glm::vec4(0.0f, 0.0f, 1.0f, 0.7f)); break;
            case 16: this->tail->setColor(glm::vec4(1.0f, 0.65f, 0.0f, 0.7f)); break;
        }
    } else {
        this->tail = nullptr;
    }
}

bool SongNote::checkInput(unsigned int input) {
    return (input & this->value) > 0;
}

bool SongNote::intersects(Rect rect) {
    return this->note->getBounds().intersects(rect);
}

bool SongNote::isBefore(Rect rect) {
    return this->note->getBounds().height < rect.top;
}

bool SongNote::isAfter(Rect rect) {
    return this->note->getBounds().top > rect.height;
}

bool SongNote::isDisabled() {
    return this->disabled;
}

bool SongNote::hasTail() {
    return this->duration > 0.0;
}

void SongNote::disable() {
    this->disabled = true;
}

Rect SongNote::getBounds() {
    float left = this->note->getBounds().left;
    float top = hasTail() ? this->tail->getBounds().top : this->note->getBounds().top;
    float width = this->note->getBounds().width;
    float height = this->note->getBounds().height;
    return Rect(left, top, width, height);
}

void SongNote::move(int value) {
    this->note->moveY(value);
    if (hasTail()) this->tail->moveY(value);
}

void SongNote::draw(GLFWwindow* window) {
    if (hasTail()) this->tail->draw(window);
    this->note->draw(window);
}