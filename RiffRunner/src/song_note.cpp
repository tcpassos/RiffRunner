#include "song_note.h"

SongNote::SongNote(Sprite& track, unsigned int value, double duration) {
    this->value = value;
    this->duration = duration;
    this->disabled = false;

    // Note
    Texture2D noteTexture = ResourceManager::LoadTexture("resources/img/notes.png", "notes");
    this->note = new Sprite(noteTexture);

    unsigned int noteIndex = log2(value); // 1=0 2=1 4=2 8=3 16=4
    float noteSize = 40.0f;
    float noteWidth = noteSize / 5;
    float notePosition = track.getBounds().left + (noteIndex * track.getSize().x / 5) + track.getSize().x / 10;
    Rect noteTextureRect(glm::vec2(noteIndex * noteWidth, 0.0f), glm::vec2((noteIndex + 1) * noteWidth, noteSize / 6));

    this->note->setSize(glm::vec2(noteSize));
    this->note->setOrigin(this->note->getSize().x / 2, 0.0f);
    this->note->setPosition(notePosition, 0.0f);
    this->note->setTextureRect(noteTextureRect);
    this->note->setProjection(*track.getProjection());

    // Tail
    if (duration > 0) {
        this->tail = new RectangleShape(10, duration * 10);
        this->tail->setOrigin(this->tail->getSize().x / 2, this->tail->getSize().y);
        this->tail->setPosition(this->note->getPosition().x, 0.0f);
        this->tail->setProjection(*track.getProjection());
        switch (value) {
        case 1: this->tail->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.7f)); break;
        case 2: this->tail->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.7f)); break;
        case 4: this->tail->setColor(glm::vec4(1.0f, 1.0f, 0.0f, 0.7f)); break;
        case 8: this->tail->setColor(glm::vec4(0.0f, 0.0f, 1.0f, 0.7f)); break;
        case 16: this->tail->setColor(glm::vec4(1.0f, 0.65f, 0.0f, 0.7f)); break;
        }
    }
    else {
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

    glm::vec4 noteColor = this->note->getColor();
    noteColor.a = 0.3f;
    this->note->setColor(noteColor);

    if (hasTail()) {
        glm::vec4 tailColor = this->tail->getColor();
        tailColor.a = 0.3f;
        this->tail->setColor(tailColor);
    }
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