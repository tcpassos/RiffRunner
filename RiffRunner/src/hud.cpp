#include <string>

#include "effects.h"
#include "hud.h"

#define MULTIPLIER_TRESHOLD 15
#define SPECIAL_BAR_MAX 500

HUD::HUD() {
	this->limit = 100;
	this->performance = limit / 2;
	this->score = 0;
	this->streak = 0;
	this->lastUpdateTime = 0.0;
	this->specialActive = false;
	this->specialCounter = 0;

	// Indicator
							  ResourceManager::loadTexture("assets/img/hud/green.png", "performanceGreen");
    Texture2D yellowTexture = ResourceManager::loadTexture("assets/img/hud/yellow.png", "performanceYellow");
							  ResourceManager::loadTexture("assets/img/hud/red.png", "performanceRed");
	this->indicator = new Sprite(yellowTexture);
	this->indicator->setSize(yellowTexture.width * 0.55, yellowTexture.height * 0.55);
	this->indicator->setPosition(0, 200);

	// Pointer
	Texture2D pointerTexture = ResourceManager::loadTexture("assets/img/hud/pointer.png", "performancePointer");
	float pointerWidth = pointerTexture.width * 0.27;
	float pointerHeight = pointerTexture.height * 0.25;
	this->pointer = new Sprite(pointerTexture);
	this->pointer->setSize(pointerWidth, pointerHeight);
	this->pointer->setOrigin(pointerWidth / 2, pointerHeight - 15);
	this->pointer->setPosition(this->indicator->getPosition().x + this->indicator->getSize().x / 2, this->indicator->getPosition().y + 140);

	// Score
	Texture2D displayTexture = ResourceManager::loadTexture("assets/img/hud/display.png", "scoreDisplay");
	this->display = new Sprite(displayTexture);
	this->display->setPosition(0.0, this->indicator->getBounds().height);
	this->display->setSize(displayTexture.width * 0.8, displayTexture.height * 0.8);
	this->display->setColor(glm::vec4(1.0, 1.0, 1.0, 0.5));
	this->scoreText = new TextRenderer(800, 600);
	this->scoreText->load("assets/fonts/digital-7.ttf", 45);
	this->scoreText->setHorizontalAlignment(TextRight);

	// Special Multiplier
	Texture2D x1Texture = ResourceManager::loadTexture("assets/img/hud/x1.png", "x1");
						  ResourceManager::loadTexture("assets/img/hud/x2.png", "x2");
						  ResourceManager::loadTexture("assets/img/hud/x3.png", "x3");
						  ResourceManager::loadTexture("assets/img/hud/x4.png", "x4");
						  ResourceManager::loadTexture("assets/img/hud/sx2.png", "sx2");
						  ResourceManager::loadTexture("assets/img/hud/sx4.png", "sx4");
						  ResourceManager::loadTexture("assets/img/hud/sx6.png", "sx6");
						  ResourceManager::loadTexture("assets/img/hud/sx8.png", "sx8");
	this->multiplier = new Sprite(x1Texture);
	this->multiplier->setPosition(800.0f - this->multiplier->getSize().x, this->indicator->getPosition().y);

	Texture2D specialBarTexture = ResourceManager::loadTexture("assets/img/hud/special_bar.png", "specialBar");
	this->specialBar = new Sprite(specialBarTexture);
	this->specialBar->setColor(glm::vec4(0.3f, 0.8f, 0.9f, 1.0f));
	float specialBarHeight = specialBarTexture.height;
	this->specialBarUnit = specialBarHeight / SPECIAL_BAR_MAX;
	this->specialBar->setPosition(this->multiplier->getPosition().x + 143, this->multiplier->getBounds().height - 60);
	shineEffect.setIntensity(0.6);
	shineEffect.setSpeed(1.0);
	this->specialBar->addEffect(&shineEffect);
	updateSpecialBar();

	// Sound
	this->specialSound = new Sound("assets/sound/special.wav");
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

void HUD::addPoints(unsigned int points) {
	unsigned int newPoints = points * getMultiplier();
	this->score += newPoints;
	// Update special bar
	if (!specialActive) {
		specialCounter += newPoints;
		if (specialCounter > SPECIAL_BAR_MAX) specialCounter = SPECIAL_BAR_MAX;
	}
	updateSpecialBar();
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
	if (specialCounter > (SPECIAL_BAR_MAX / 2) && !specialActive) {
		specialSound->playOnce();
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

void HUD::updatePerformanceIndicator() {
	float openingAngle = 126;
	float rotationDegrees = ((float)this->performance * openingAngle / (float)this->limit) - openingAngle / 2;
	this->pointer->setRotation(glm::radians(rotationDegrees));

	if (this->performance < this->limit / 3) {
		this->indicator->setTexture(ResourceManager::getTexture("performanceRed"));
	}
	else if (this->performance > (this->limit / 3) * 2) {
		this->indicator->setTexture(ResourceManager::getTexture("performanceGreen"));
	}
	else {
		this->indicator->setTexture(ResourceManager::getTexture("performanceYellow"));
	}
}

void HUD::updateMultiplier() {
	unsigned int multiplier = getMultiplier();
	if (!specialActive) {
		if (multiplier == 1) {
			this->multiplier->setTexture(ResourceManager::getTexture("x1"));
		} else if (multiplier == 2) {
			this->multiplier->setTexture(ResourceManager::getTexture("x2"));
		} else if (multiplier == 3) {
			this->multiplier->setTexture(ResourceManager::getTexture("x3"));
		} else if (multiplier == 4) {
			this->multiplier->setTexture(ResourceManager::getTexture("x4"));
		}
	} else {
		if (multiplier == 1) {
			this->multiplier->setTexture(ResourceManager::getTexture("sx2"));
		} else if (multiplier == 2) {
			this->multiplier->setTexture(ResourceManager::getTexture("sx4"));
		} else if (multiplier == 3) {
			this->multiplier->setTexture(ResourceManager::getTexture("sx6"));
		} else if (multiplier == 4) {
			this->multiplier->setTexture(ResourceManager::getTexture("sx8"));
		}
	}
}

void HUD::updateSpecialBar() {
	specialBar->setSize(specialBar->getSize().x, specialBarUnit * specialCounter);
	specialBar->setOrigin(0.0f, specialBar->getSize().y);
}

void HUD::update(double elapsedTime) {
	if (elapsedTime - lastUpdateTime < 0.2) /* 0.2 sec */ {
		return;
	}
	lastUpdateTime = elapsedTime;
	if (specialActive) {
		specialCounter -= 5;
		if (specialCounter <= 0) {
			specialCounter = 0;
			specialActive = false;
			specialSound->playOnce();
			updateMultiplier();
		}
		updateSpecialBar();
	}
}

void HUD::draw(GLFWwindow* window) {
	this->indicator->draw(window);
	this->pointer->draw(window);
	this->display->draw(window);
	this->scoreText->renderText(std::to_string(this->score),
								this->display->getPosition().x + this->display->getSize().x - 30, this->display->getBounds().top + 35);
	this->specialBar->draw(window);
	this->multiplier->draw(window);
}