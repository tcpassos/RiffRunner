#include <string>

#include "hud.h"

#define MULTIPLIER_TRESHOLD 30
#define SPECIAL_BAR_MAX 1000

HUD::HUD() {
	this->limit = 100;
	this->performance = limit / 2;
	this->score = 0;
	this->streak = 0;
	this->lastUpdateTime = 0.0;
	this->specialActive = false;

	// Indicator
							  ResourceManager::LoadTexture("resources/img/hud/green.png", "performanceGreen");
    Texture2D yellowTexture = ResourceManager::LoadTexture("resources/img/hud/yellow.png", "performanceYellow");
							  ResourceManager::LoadTexture("resources/img/hud/red.png", "performanceRed");
	this->indicator = new Sprite(yellowTexture);
	this->indicator->setSize(yellowTexture.width * 0.55, yellowTexture.height * 0.55);
	this->indicator->setPosition(0, 200);

	// Pointer
	Texture2D pointerTexture = ResourceManager::LoadTexture("resources/img/hud/pointer.png", "performancePointer");
	float pointerWidth = pointerTexture.width * 0.27;
	float pointerHeight = pointerTexture.height * 0.25;
	this->pointer = new Sprite(pointerTexture);
	this->pointer->setSize(pointerWidth, pointerHeight);
	this->pointer->setOrigin(pointerWidth / 2, pointerHeight - 15);
	this->pointer->setPosition(this->indicator->getPosition().x + this->indicator->getSize().x / 2, this->indicator->getPosition().y + 140);

	// Score
	Texture2D displayTexture = ResourceManager::LoadTexture("resources/img/hud/display.png", "scoreDisplay");
	this->display = new Sprite(displayTexture);
	this->display->setPosition(0.0, this->indicator->getBounds().height);
	this->display->setSize(displayTexture.width * 0.8, displayTexture.height * 0.8);
	this->display->setColor(glm::vec4(1.0, 1.0, 1.0, 0.5));
	this->scoreText = new TextRenderer(800, 600);
	this->scoreText->load("resources/fonts/digital-7.ttf", 45);

	// Special Multiplier
	Texture2D x1Texture = ResourceManager::LoadTexture("resources/img/hud/x1.png", "x1");
						  ResourceManager::LoadTexture("resources/img/hud/x2.png", "x2");
						  ResourceManager::LoadTexture("resources/img/hud/x3.png", "x3");
						  ResourceManager::LoadTexture("resources/img/hud/x4.png", "x4");
						  ResourceManager::LoadTexture("resources/img/hud/sx2.png", "sx2");
						  ResourceManager::LoadTexture("resources/img/hud/sx4.png", "sx4");
						  ResourceManager::LoadTexture("resources/img/hud/sx6.png", "sx6");
						  ResourceManager::LoadTexture("resources/img/hud/sx8.png", "sx8");
	this->multiplier = new Sprite(x1Texture);
	this->multiplier->setPosition(800.0f - this->multiplier->getSize().x, this->indicator->getPosition().y);

	this->specialCounter = 0;
	Texture2D specialBarTexture = ResourceManager::LoadTexture("resources/img/hud/special_bar.png", "specialBar");
	this->specialBarHeight = specialBarTexture.height;
	this->specialBar = new Sprite(specialBarTexture);
	this->specialBar->setOrigin(0.0f, this->specialBar->getSize().y);
	this->specialBar->setPosition(this->multiplier->getPosition().x + 143, this->multiplier->getBounds().height - 55);
	this->specialBar->setSize(this->specialBar->getSize().x, 0.0f);
	this->specialBar->setColor(glm::vec4(0.3f, 0.8f, 0.9f, 1.0f));
}

void HUD::incrementPerformance() {
	if (this->performance < this->limit)
		this->performance++;

	updatePerformanceIndicator();
};

void HUD::decrementPerformance() {
	if (this->performance > 0)
		this->performance--;

	updatePerformanceIndicator();
}

void HUD::updatePerformanceIndicator() {
	float openingAngle = 126;
	float rotationDegrees = ((float)this->performance * openingAngle / (float)this->limit) - openingAngle/2;
	this->pointer->setRotation(glm::radians(rotationDegrees));

	if (this->performance < this->limit / 3) {
		this->indicator->setTexture(ResourceManager::GetTexture("performanceRed"));
	}
	else if (this->performance > (this->limit / 3) * 2) {
		this->indicator->setTexture(ResourceManager::GetTexture("performanceGreen"));
	}
	else {
		this->indicator->setTexture(ResourceManager::GetTexture("performanceYellow"));
	}
}

void HUD::addPoints(unsigned int points) {
	unsigned int newPoints = points * getMultiplier();
	this->score += newPoints;
	// Update special bar
	specialCounter += newPoints;
	if (specialCounter > SPECIAL_BAR_MAX) specialCounter = SPECIAL_BAR_MAX;
	float specialBarUnit = specialBarHeight / SPECIAL_BAR_MAX;
	specialBar->setSize(specialBar->getSize().x, specialBarUnit * specialCounter);
	specialBar->setOrigin(0.0f, specialBar->getSize().y);
}

void HUD::incrementStreak() {
	this->streak++;
	updateMultiplier();
}

void HUD::clearStreak() {
	this->streak = 0;
	updateMultiplier();
}

void HUD::activateSpecial() {
	if (specialCounter > SPECIAL_BAR_MAX / 2) {
		specialActive = true;
		updateMultiplier();
	}
}

unsigned int HUD::getMultiplier() {
	if (this->streak < MULTIPLIER_TRESHOLD) {
		return 1;
	} else if (this->streak < MULTIPLIER_TRESHOLD * 2) {
		return 2;
	} else if (this->streak < MULTIPLIER_TRESHOLD * 3) {
		return 3;
	} else {
		return 4;
	}
}
void HUD::updateMultiplier() {
	unsigned int multiplier = getMultiplier();
	if (!specialActive) {
		if (multiplier == 1) {
			this->multiplier->setTexture(ResourceManager::GetTexture("x1"));
		} else if (multiplier == 2) {
			this->multiplier->setTexture(ResourceManager::GetTexture("x2"));
		} else if (multiplier == 3) {
			this->multiplier->setTexture(ResourceManager::GetTexture("x3"));
		} else if (multiplier == 4) {
			this->multiplier->setTexture(ResourceManager::GetTexture("x4"));
		}
	} else {
		if (multiplier == 1) {
			this->multiplier->setTexture(ResourceManager::GetTexture("sx2"));
		} else if (multiplier == 2) {
			this->multiplier->setTexture(ResourceManager::GetTexture("sx4"));
		} else if (multiplier == 3) {
			this->multiplier->setTexture(ResourceManager::GetTexture("sx6"));
		} else if (multiplier == 4) {
			this->multiplier->setTexture(ResourceManager::GetTexture("sx8"));
		}
	}
}

void HUD::update(double elapsedTime) {
	if (elapsedTime - lastUpdateTime < 1.0) /* 1 sec */ {
		return;
	}
	lastUpdateTime = elapsedTime;
	if (specialActive) {
		specialCounter--;
		if (specialCounter == 0) {
			specialActive = false;
			updateMultiplier();
		}
	}
}

void HUD::draw(GLFWwindow* window) {
	this->indicator->draw(window);
	this->pointer->draw(window);
	this->display->draw(window);
	this->scoreText->renderText(std::to_string(this->score),
								this->display->getPosition().x + this->display->getSize().x / 2, this->display->getBounds().top + 35);
	this->specialBar->draw(window);
	this->multiplier->draw(window);
}