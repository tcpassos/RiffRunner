#include "song_note.h"

SongNote::SongNote(Sprite& track, unsigned int value, unsigned int tailLength) {
    this->value = value;
    this->index = log2(value); // 1=0 2=1 4=2 8=3 16=4
    this->tailLength = tailLength;
    this->disabled = false;
    this->consumed = false;
    this->lastHoldingPointTime = 0;

    // Note
    Texture2D noteTexture = ResourceManager::LoadTexture("resources/img/notes.png", "notes");
    this->note = new Sprite(noteTexture);

    float noteSize = 40.0f;
    float noteWidth = noteSize / 5;
    float noteHeigth = noteSize / 6;
    float notePosition = track.getBounds().left + (this->index * track.getSize().x / 5) + track.getSize().x / 10;
    noteNormalTextureRect = new Rect(glm::vec2(this->index * noteWidth, 0.0f), glm::vec2((this->index + 1) * noteWidth, noteHeigth));
    noteSpecialTextureRect = new Rect(glm::vec2(this->index * noteWidth, noteHeigth * 5), glm::vec2((this->index + 1) * noteWidth, noteHeigth * 6));

    this->note->setSize(glm::vec2(noteSize));
    this->note->setOrigin(this->note->getSize().x / 2, 0.0f);
    this->note->setPosition(notePosition, 0.0f);
    this->note->setTextureRect(*noteNormalTextureRect);
    this->note->setProjection(*track.getProjection(), true);

    // Tail
    if (tailLength > 0) {
        this->tail = new RectangleShape(10, tailLength);
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

bool SongNote::isConsumed() {
    return this->consumed;
}

bool SongNote::hasTail() {
    return this->tailLength > 0;
}

unsigned int SongNote::getIndex() {
    return this->index;
}

unsigned int SongNote::getValue() {
    return this->value;
}

Rect SongNote::getBounds() {
    float left = this->note->getBounds().left;
    float top = hasTail() ? this->tail->getBounds().top : this->note->getBounds().top;
    float width = this->note->getBounds().width;
    float height = this->note->getBounds().height;
    return Rect(left, top, width, height);
}

void SongNote::disable() {
    this->disabled = true;

    if (hasTail()) {
        glm::vec4 tailColor = this->tail->getColor();
        tailColor.a = 0.3f;
        this->tail->setColor(tailColor);
    }
}

unsigned int SongNote::hold(unsigned int positionY) {
    if (note->getPosition().y < positionY) {
        return 0;
    }
    // increments the score every 0.2 seconds
    unsigned int points = 0;
    double currentTime = glfwGetTime();
    if (currentTime - lastHoldingPointTime > 0.2) {
        lastHoldingPointTime = currentTime;
        points++;
    }
    this->consumed = true;
    // Hide note
    note->setColor(glm::vec4(0.0f));
    // Increases tail opacity
    glm::vec4 tailColor = tail->getColor();
    tailColor.a = 1.0f;
    tail->setColor(tailColor);
    // Update tail length
    if (hasTail()) {
        float newLength = tail->getSize().y - (tail->getBounds().height - positionY);
        if (newLength > 0) {
            tail->setSize(tail->getSize().x, newLength);
            note->setPosition(note->getPosition().x, positionY);
        }
    }
    return points;
}

void SongNote::move(float value) {
    this->note->moveY(value);
    if (hasTail()) this->tail->moveY(value);
}

void SongNote::draw(GLFWwindow* window, bool specialActive) {
    if (hasTail()) this->tail->draw(window);
    if (specialActive) {
        note->setTextureRect(*noteSpecialTextureRect);
    } else {
        note->setTextureRect(*noteNormalTextureRect);
    }
    note->draw(window);
}