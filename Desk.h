#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;

class Object {
public:
	void setSpriteTexture(Texture *texture) {
		sprite.setTexture(*texture);
	}
	virtual void setRect() {}
	Sprite getSprite() { return sprite; }
	void setSpritePosition() { sprite.setPosition(x, y); }
	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
	void setIsWhite(bool isWhite) { this->isWhite = isWhite; }
	int getX() { return x; }
	int getY() { return y; }
	bool getIsWhite() { return isWhite; }
	virtual ~Object() {};
protected:
	Sprite sprite;
private:
	int x, y;
	bool isWhite;
};

class Cell : public Object {
public:
	void setRect() {
		if ( getIsWhite() ) {
			sprite.setTextureRect(IntRect(64,64,64,64));
		}
		else {
			sprite.setTextureRect(IntRect(0, 64, 64, 64));
		}
	}
	void setIsEmpty(bool isEmpty) { this->isEmpty = isEmpty; }
	bool getIsEmpty() { return isEmpty; }
private:
	bool isEmpty;
};

class Checker : public Object {
public:
	void setRect() {
		if ( getIsWhite() ) {
			sprite.setTextureRect(IntRect(0, 0, 64, 64));
		}
		else {
			sprite.setTextureRect(IntRect(64, 0, 64, 64));
		}
	}

	virtual void setLocation(int h, int w) {
		setX(h*64);
		setY(w*64);
		setSpritePosition();
	}	

	int getCell() { return cell; }
	void setCell(int cell) { this->cell = cell; }
	bool getKing() { return king; }
	void setKing(bool king) { this->king = king; }

	std::vector<int> freeToMoveCell;
	int eatenChecker[4] = { -1, -1, -1, -1 };//едомая
	int cellOfEatenChecker[4] = { -1, -1, -1, -1 };//едомая
private:
	int cell;
	bool king;
};

struct DeskState {
	Checker whiteChecker[12], darkChecker[12];//Массив шашек
	bool whiteIsMove;
	Cell cell[64];//Массив ячеек
	bool startOfMove, endOfMove;
	int chousenChekker;
	Sprite ellowSquare;
	std::vector<Sprite> ellowSquareVector;
	std::vector<int> freeToMoveCell;
	std::vector<int> eatingCheckers;//едящая
	int eatenForMove = 0;
};

class Desk {
public:
	Desk(Texture *texture);
	void drawDesk(RenderWindow *window);
	int cellIsChousen(int mouseX, int mouseY, bool mouseIsPressed); 
	void checkerСhecking(int i);//Нахождение свободнвх ходов для шашки, добавление едомых в список и т.п.
	void update(int mouseX, int mouseY, bool mouseIsPressed);
	DeskState* getState();
	void setState(DeskState* deskState);
	int getWeight();
	bool computerIsMove() { return (startOfMove||eatenForMove>0) && !whiteIsMove; }
	bool getWhiteIsMove() { return whiteIsMove; }
	bool getEndOfGame();
private:
	bool startOfMove, endOfMove;
	Checker whiteChecker[12], darkChecker[12];//Массив шашек
	bool whiteIsMove;
	Cell cell[80];//Массив ячеек
	int chousenChekker;
	Sprite ellowSquare;
	std::vector<Sprite> ellowSquareVector;
	std::vector<int> freeToMoveCell;
	std::vector<int> eatingCheckers;//едящая
	int eatenForMove = 0;
};