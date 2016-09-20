#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Desk.h"

using namespace sf;

class Button : public Object{
public:
	Button(Texture *texture, int i, float x, float y, Font *fn) {
		Object();
		label = std::shared_ptr<Text>(new Text("", *fn, 20));
		type = i;
		setLabel();
		sprite.setTexture(*texture);
		sprite.setTextureRect(IntRect(0, 512, 170, 64));
		sprite.setPosition(x, y);
		setX(x);
		setY(y);
		
	}

	void setLabel() {
		switch (type)
		{
		case 0: label->setString("PvP"); label->setPosition(237,117); break;
		case 1: label->setString("PvE"); label->setPosition(237, 191); break;
		case 2: label->setString("Exit"); label->setPosition(237, 265); break;
		default:
			break;
		}
	}

	void update(float mouseX, float mouseY, bool mouseIsPressed, int &workState) {
		if (mouseX > getX() && mouseY > getY() && mouseX < getX() + 170 && mouseY < getY() + 64 && mouseIsPressed) {
			switch (type)
			{
			case 0: workState = 1; break;
			case 1: workState = 2; break;
			case 2: workState = 10; break;
			default:
				break;
			}
		}
	}

	std::shared_ptr<Text> getLabel() { return label; }
	
private:
	std::shared_ptr<Text> label;
	int type;// 0 - PvP 1 - PvE 2 - Exit
};

class Menu {
public:
	Menu(Texture *texture, Font *fn) {
		this->fn = *fn;

		backGround.setTexture(*texture);
		pvp = std::shared_ptr<Button>(new Button(texture, 0, 171, 100, fn));
		pvp->setSpritePosition();
		pve = std::shared_ptr<Button>(new Button(texture, 1, 171, 174, fn));
		pvp->setSpritePosition();
		exit = std::shared_ptr<Button>(new Button(texture, 2, 171, 248, fn));
		exit->setSpritePosition();
		

		backGround.setTextureRect(IntRect(0, 0, 512, 512));
	};

	void draw(RenderWindow *window) {
		window->draw(backGround);

		window->draw(pvp->getSprite());
		window->draw(*pvp->getLabel());
		window->draw(pve->getSprite());
		window->draw(*pve->getLabel());
		window->draw(exit->getSprite());
		window->draw(*exit->getLabel());
	}

	void update(float mouseX, float mouseY, bool mouseIsPressed, int &workState) {
		pvp->update(mouseX, mouseY, mouseIsPressed, workState);
		pve->update(mouseX, mouseY, mouseIsPressed, workState);
		exit->update(mouseX, mouseY, mouseIsPressed, workState);
	}
private:
	Sprite backGround;
	std::shared_ptr<Button> pvp, pve, exit;

	Font fn;
};