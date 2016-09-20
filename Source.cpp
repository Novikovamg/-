#include <SFML/Graphics.hpp>
#include <iostream>
#include "Desk.h"
#include "Player.h"
#include "Menu.h"

using namespace sf;

int main() {
	RenderWindow *window = new RenderWindow(VideoMode(512, 512), "checkers", sf::Style::Close);
	Clock clock;
	Player player;
	//�������� �������� ����� � �����
	Texture *imageOfDesk = new Texture;
	imageOfDesk->loadFromFile("C:/image.png");
	//�������� �������� ����
	Texture *imageOfMenu = new Texture;
	imageOfMenu->loadFromFile("C:/menu.png");
	//�������� �����
	Font font;
	font.loadFromFile("C:/Font.TTF");

	Desk desk(imageOfDesk);
	Menu menu(imageOfMenu, &font);

	int workState = 0;//0-���� 1-����� ������ ������ 2-����� ������ ����������

	while (window->isOpen()) {
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time /= 800;

		Vector2i pixelPos = Mouse::getPosition(*window);//�������� ����� �������

		Event event;
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				window->close();
		}
		
		switch (workState) {
		case 0:
			menu.update(pixelPos.x, pixelPos.y, event.type == Event::MouseButtonPressed, workState);
			break;
		case 1:
			desk.update(pixelPos.x, pixelPos.y, event.type == Event::MouseButtonPressed);
			break;
		case 2:
			if (!desk.computerIsMove()) {
				desk.update(pixelPos.x, pixelPos.y, event.type == Event::MouseButtonPressed);
			}
			else {
				bool continuane = true;
				while (continuane) {
					player.makeDecision(imageOfDesk, desk.getState());
					desk.update(player.getMoveFrome().x, player.getMoveFrome().y, true);
					desk.update(player.getMoveTo().x, player.getMoveTo().y, true);
					continuane = desk.computerIsMove();
				}
			}
			break;
		default:
			window->close();
		}

		window->clear();

		switch (workState) {
		case 0:
			menu.draw(window);
			break;
		case 1:
			desk.drawDesk(window);//������ �����
			break;
		case 2:
			desk.drawDesk(window);//������ �����
			break;
		}

		

		window->display();
	}
	return 0;
}