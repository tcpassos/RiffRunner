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

	int screenWidth, screenHeight;
	glfwGetWindowSize(glfwGetCurrentContext(), &screenWidth, &screenHeight);

	// Indicator
							  ResourceManager::loadTexture("assets/img/hud/green.png", "performanceGreen");
    Texture2D yellowTexture = ResourceManager::loadTexture("assets/img/hud/yellow.png", "performanceYellow");
							  ResourceManager::loadTexture("assets/img/hud/red.png", "performanceRed");
	this->indicator = new Sprite(yellowTexture);
	this->indicator->setSize(screenWidth * 0.3, screenHeight * 0.23);
	this->indicator->setPosition(0, screenHeight / 3);

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
	this->display->setSize(indicator->getSize().x, indicator->getSize().y * 0.6);
	this->display->setColor(glm::vec4(1.0, 1.0, 1.0, 0.5));
	this->scoreText = new TextRenderer(screenWidth, screenHeight, Font("assets/fonts/digital-7.ttf", 45));
	this->scoreText->setHorizontalAlignment(TextRight);

	// Special Multiplier
	multiplierTextures.push_back(ResourceManager::loadTexture("assets/img/hud/x1.png", "x1"));
	multiplierTextures.push_back(ResourceManager::loadTexture("assets/img/hud/x2.png", "x2"));
	multiplierTextures.push_back(ResourceManager::loadTexture("assets/img/hud/x3.png", "x3"));
	multiplierTextures.push_back(ResourceManager::loadTexture("assets/img/hud/x4.png", "x4"));
	multiplierTextures.push_back(ResourceManager::loadTexture("assets/img/hud/sx2.png", "sx2"));
	multiplierTextures.push_back(ResourceManager::loadTexture("assets/img/hud/sx4.png", "sx4"));
	multiplierTextures.push_back(ResourceManager::loadTexture("assets/img/hud/sx6.png", "sx6"));
	multiplierTextures.push_back(ResourceManager::loadTexture("assets/img/hud/sx8.png", "sx8"));
	this->multiplier = new Sprite(multiplierTextures[0]);
	this->multiplier->setPosition(screenWidth - this->multiplier->getSize().x, this->indicator->getPosition().y);

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

	Texture2D performanceTexture;
	if (this->performance < this->limit / 3) {
		performanceTexture = ResourceManager::getTexture("performanceRed");
	}
	else if (this->performance > (this->limit / 3) * 2) {
		performanceTexture = ResourceManager::getTexture("performanceGreen");
	}
	else {
		performanceTexture = ResourceManager::getTexture("performanceYellow");
	}
	this->indicator->setTexture(performanceTexture);
}

void HUD::updateMultiplier() {
	unsigned int multiplierIndex = getMultiplier();

	if (!specialActive) {
		this->multiplier->setTexture(multiplierTextures[multiplierIndex - 1]);
	} else {
		this->multiplier->setTexture(multiplierTextures[multiplierIndex + 3]);
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
								this->display->getPosition().x + this->display->getSize().x - 30, this->display->getBounds().top + 30);
	this->specialBar->draw(window);
	this->multiplier->draw(window);
}