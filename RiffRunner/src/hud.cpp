#include <string>

#include "hud.h"

HUD::HUD() {
	this->limit = 100;
	this->performance = limit / 2;
	this->score = 0;

	// Indicator
	Texture2D greenTexture = ResourceManager::LoadTexture("resources/img/hud/green.png", "performanceGreen");
	Texture2D yellowTexture = ResourceManager::LoadTexture("resources/img/hud/yellow.png", "performanceYellow");
	Texture2D redTexture = ResourceManager::LoadTexture("resources/img/hud/red.png", "performanceRed");
	this->indicator = new Sprite(yellowTexture);
	this->indicator->setSize(yellowTexture.Width * 0.55, yellowTexture.Height * 0.55);
	this->indicator->setPosition(0, 300);

	// Pointer
	Texture2D pointerTexture = ResourceManager::LoadTexture("resources/img/hud/pointer.png", "performancePointer");
	float pointerWidth = pointerTexture.Width * 0.27;
	float pointerHeight = pointerTexture.Height * 0.25;
	this->pointer = new Sprite(pointerTexture);
	this->pointer->setSize(pointerWidth, pointerHeight);
	this->pointer->setOrigin(pointerWidth / 2, pointerHeight - 15);
	this->pointer->setPosition(this->indicator->getPosition().x + this->indicator->getSize().x / 2, this->indicator->getPosition().y + 140);

	// Score
	Texture2D displayTexture = ResourceManager::LoadTexture("resources/img/hud/display.png", "scoreDisplay");
	this->display = new Sprite(displayTexture);
	this->display->setPosition(0.0, this->indicator->getBounds().height);
	this->display->setSize(displayTexture.Width * 0.8, displayTexture.Height * 0.8);
	this->display->setColor(glm::vec4(1.0, 1.0, 1.0, 0.5));
	this->scoreText = new TextRenderer(800, 600);
	this->scoreText->Load("resources/fonts/digital-7.ttf", 45);
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

void HUD::draw(GLFWwindow* window) {
	this->indicator->draw(window);
	this->pointer->draw(window);
	this->display->draw(window);
	this->scoreText->RenderText(std::to_string(this->score),
								this->display->getPosition().x + this->display->getSize().x / 2, this->display->getBounds().top + 35,
							    1.0f);
}