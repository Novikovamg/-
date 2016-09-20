#include "Desk.h"

Desk::Desk(Texture *texture) {
	ellowSquare.setTexture(*texture);
	ellowSquare.setTextureRect(IntRect(0, 128, 64, 64));
	ellowSquare.setPosition(-64, -64);
	ellowSquareVector.push_back(ellowSquare);

	whiteIsMove = true;//Первыми ходят белые
	startOfMove = true;
	endOfMove = false;
	for (int i = 64; i < 80; i++) {
		cell[i].setIsEmpty(false);
	}
	//Задаем параметры клеткам
	for (int i = 0; i < 64; i++) {
		cell[i].setSpriteTexture(texture);
		//Определяем цвет
		if (i % 2 == 0 && i / 8 % 2 == 0) {
			cell[i].setIsWhite(true);
		}
		if (i % 2 == 0 && i / 8 % 2 == 1) {
			cell[i].setIsWhite(false);
		}
		if (i % 2 == 1 && i / 8 % 2 == 0) {
			cell[i].setIsWhite(false);
		}
		if (i % 2 == 1 && i / 8 % 2 == 1) {
			cell[i].setIsWhite(true);
		}
		cell[i].setRect();
		cell[i].setX(i % 8 * 64);
		cell[i].setY(i / 8 * 64);
		cell[i].setSpritePosition();
		cell[i].setIsEmpty(true);
	}
	//Задаем параметры черным шашкам
	for (int i = 0; i < 12; i++) {
		darkChecker[i].setKing(false);

		darkChecker[i].setSpriteTexture(texture);
		darkChecker[i].setIsWhite(false);
		darkChecker[i].setRect();
		darkChecker[i].setY(i / 4 * 64);
		if (i < 4) {
			darkChecker[i].setX(i % 4 * 128 + 64);
			darkChecker[i].setCell(i * 2 + 1);
		}
		if (i >= 4 && i <= 7) {
			darkChecker[i].setX(i % 4 * 128);
			darkChecker[i].setCell(i * 2);
		}
		if (i > 7) {
			darkChecker[i].setX(i % 4 * 128 + 64);
			darkChecker[i].setCell(i * 2 + 1);
		}
		darkChecker[i].setSpritePosition();
		cell[darkChecker[i].getCell()].setIsEmpty(false);
	}
	//Задаем параметры белым шашкам
	for (int i = 0; i < 12; i++) {
		whiteChecker[i].setKing(false);

		whiteChecker[i].setSpriteTexture(texture);
		whiteChecker[i].setIsWhite(true);
		whiteChecker[i].setRect();
		whiteChecker[i].setY((i / 4 + 5) * 64);
		if (i < 4) {
			whiteChecker[i].setX(i % 4 * 128);
			whiteChecker[i].setCell(i * 2 + 40);
		}
		if (i >= 4 && i <= 7) {
			whiteChecker[i].setX(i % 4 * 128 + 64);
			whiteChecker[i].setCell(i * 2 + 41);
		}
		if (i > 7) {
			whiteChecker[i].setX(i % 4 * 128);
			whiteChecker[i].setCell(i * 2 + 40);
		}
		whiteChecker[i].setSpritePosition();
		cell[whiteChecker[i].getCell()].setIsEmpty(false);
	}
}

void Desk::drawDesk(RenderWindow *window) {
	bool r = getEndOfGame();
	if (r) {
		//abort();
		//window->close();
	}

	for (int i = 0; i < 64; i++) {
		window->draw(cell[i].getSprite());
	}
	for (int i = 0; i < 12; i++) {
		window->draw(whiteChecker[i].getSprite());

	}
	for (int i = 0; i < 12; i++) {
		window->draw(darkChecker[i].getSprite());
	}
	for (auto it : ellowSquareVector) window->draw(it);
	
}

int Desk::cellIsChousen(int mouseX, int mouseY, bool mouseIsPressed) {
	if (mouseIsPressed && mouseX >= 0 && mouseX <= 512 && mouseY >= 0 && mouseY <= 512) {
		int h = mouseY / 64;
		int w = mouseX / 64;
		int i = w + h * 8;//индекс для массива клеток
		return i;
	}
	return -1;
}

void Desk::checkerСhecking(int i) {
	if (whiteIsMove) {
		int checkerCell = whiteChecker[i].getCell();
		whiteChecker[i].freeToMoveCell.clear();
		if (!whiteChecker[i].getKing()) {
			//Узнаем куда ходить для белой шашки стоящей в крайнем левом столбце
			if (checkerCell == 8 || checkerCell == 24 || checkerCell == 40 || checkerCell == 56) {
				if (cell[checkerCell - 7].getIsEmpty() && eatenForMove == 0)
					whiteChecker[i].freeToMoveCell.push_back(checkerCell - 7);
				//Право верх
				if (!cell[checkerCell - 7].getIsEmpty() && cell[checkerCell - 14].getIsEmpty() &&
					checkerCell != 14 && checkerCell != 12 && checkerCell != 10 && checkerCell != 8) {
					for (int j = 0; j < 12; j++) {
						if (darkChecker[j].getCell() == checkerCell - 7) {
							whiteChecker[i].freeToMoveCell.push_back(checkerCell - 14);
							whiteChecker[i].eatenChecker[1] = j;
							eatingCheckers.push_back(i);
							break;
						}

					}
				}
				//Право низ
				if (!cell[checkerCell + 9].getIsEmpty() && cell[checkerCell + 18].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (darkChecker[j].getCell() == checkerCell + 9) {
							whiteChecker[i].freeToMoveCell.push_back(checkerCell + 18);
							whiteChecker[i].eatenChecker[2] = j;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
			}
			//Узнаем куда ходить для белой шашки стоящей в крайнем правом столбце
			else if (checkerCell == 7 || checkerCell == 23 || checkerCell == 39 || checkerCell == 55) {
				if (cell[checkerCell - 9].getIsEmpty() && eatenForMove == 0)
					whiteChecker[i].freeToMoveCell.push_back(checkerCell - 9);
				//Лево верх
				if (!cell[checkerCell - 9].getIsEmpty() && cell[checkerCell - 18].getIsEmpty() &&
					checkerCell != 14 && checkerCell != 12 && checkerCell != 10 && checkerCell != 8) {
					for (int j = 0; j < 12; j++) {
						if (darkChecker[j].getCell() == checkerCell - 9) {
							whiteChecker[i].freeToMoveCell.push_back(checkerCell - 18);
							whiteChecker[i].eatenChecker[0] = j;
							eatingCheckers.push_back(i);
							break;
						}

					}
				}
				//Лево низ
				if (!cell[checkerCell + 7].getIsEmpty() && cell[checkerCell + 14].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (darkChecker[j].getCell() == checkerCell + 7) {
							whiteChecker[i].freeToMoveCell.push_back(checkerCell + 14);
							whiteChecker[i].eatenChecker[3] = j;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
			}
			else {
				if (cell[checkerCell - 7].getIsEmpty() && eatenForMove == 0)
					whiteChecker[i].freeToMoveCell.push_back(checkerCell - 7);
				if (checkerCell != 14 && checkerCell != 30 && checkerCell != 46 && checkerCell != 62) {
					//Право верх
					if (!cell[checkerCell - 7].getIsEmpty() && cell[checkerCell - 14].getIsEmpty() &&
						checkerCell != 14 && checkerCell != 12 && checkerCell != 10 && checkerCell != 8 &&
						checkerCell != 30 && checkerCell != 46 && checkerCell != 62) {
						for (int j = 0; j < 12; j++) {
							if (darkChecker[j].getCell() == checkerCell - 7) {
								whiteChecker[i].freeToMoveCell.push_back(checkerCell - 14);
								whiteChecker[i].eatenChecker[1] = j;
								eatingCheckers.push_back(i);
								break;
							}

						}
					}
					//Право низ
					if (checkerCell != 49 && checkerCell != 51 && checkerCell != 53 && checkerCell != 55 &&
						checkerCell != 14 && checkerCell != 30 && checkerCell != 46 && checkerCell != 62 &&
						(!cell[checkerCell + 9].getIsEmpty() && cell[checkerCell + 18].getIsEmpty())) {
						for (int j = 0; j < 12; j++) {
							if (darkChecker[j].getCell() == checkerCell + 9) {
								whiteChecker[i].freeToMoveCell.push_back(checkerCell + 18);
								whiteChecker[i].eatenChecker[2] = j;
								eatingCheckers.push_back(i);
								break;
							}

						}
					}
				}
				if (cell[checkerCell - 9].getIsEmpty() && eatenForMove == 0)
					whiteChecker[i].freeToMoveCell.push_back(checkerCell - 9);
				if (checkerCell != 1 && checkerCell != 17 && checkerCell != 33 && checkerCell != 49) {
					//Лево верх
					if (!cell[checkerCell - 9].getIsEmpty() && cell[checkerCell - 18].getIsEmpty() &&
						checkerCell != 14 && checkerCell != 12 && checkerCell != 10 && checkerCell != 8 &&
						checkerCell != 1 && checkerCell != 17 && checkerCell != 33 && checkerCell != 49) {
						for (int j = 0; j < 12; j++) {
							if (darkChecker[j].getCell() == checkerCell - 9) {
								whiteChecker[i].freeToMoveCell.push_back(checkerCell - 18);
								whiteChecker[i].eatenChecker[0] = j;
								eatingCheckers.push_back(i);
								break;
							}

						}
					}
					//Лево низ
					if (!cell[checkerCell + 7].getIsEmpty() && cell[checkerCell + 14].getIsEmpty() &&
						checkerCell != 49 && checkerCell != 51 && checkerCell != 53 && checkerCell != 55 &&
						checkerCell != 1 && checkerCell != 17 && checkerCell != 33 && checkerCell != 49) {
						for (int j = 0; j < 12; j++) {
							if (darkChecker[j].getCell() == checkerCell + 7) {
								whiteChecker[i].freeToMoveCell.push_back(checkerCell + 14);
								whiteChecker[i].eatenChecker[3] = j;
								eatingCheckers.push_back(i);
								break;
							}
						}
					}
				}
			}
		}
		else {//Если Дамка
			  //Узнаем куда ходить белой дамке стоящей в крайнем левом столбце
			if (checkerCell == 8 || checkerCell == 24 || checkerCell == 40 || checkerCell == 56) {
				int c;
				for (c = checkerCell - 7; c > 1; c -= 7) {
					if (!cell[c].getIsEmpty()) break;
					whiteChecker[i].freeToMoveCell.push_back(c);
				}
				if ((c != 7 && c != 5 && c != 3 && c != 1 && c != 8 && c != 10 && c != 12 && c != 14)
					&& whiteChecker[i].getCell() != 8 && cell[c - 7].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (darkChecker[j].getCell() == c) {
							whiteChecker[i].freeToMoveCell.push_back(c - 7);
							whiteChecker[i].eatenChecker[1] = j;
							whiteChecker[i].cellOfEatenChecker[1] = c - 7;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
				for (c = checkerCell + 9; c < 62; c += 9) {
					if (!cell[c].getIsEmpty()) break;
					whiteChecker[i].freeToMoveCell.push_back(c);
				}
				if ((c != 62 && c != 60 && c != 58 && c != 56 && c != 49 && c != 51 && c != 53 && c != 55)
					&& cell[c + 9].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (darkChecker[j].getCell() == c) {
							whiteChecker[i].freeToMoveCell.push_back(c + 9);
							whiteChecker[i].eatenChecker[2] = j;
							whiteChecker[i].cellOfEatenChecker[2] = c + 9;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
			}
			//Узнаем куда ходить белой дамке стоящей в крайнем правом столбце
			else if (checkerCell == 7 || checkerCell == 23 || checkerCell == 39 || checkerCell == 55) {
				int c;
				for (c = checkerCell - 9; c > 1; c -= 9) {
					if (!cell[c].getIsEmpty()) break;
					whiteChecker[i].freeToMoveCell.push_back(c);
				}
				if ((c != 7 && c != 5 && c != 3 && c != 1 && c != 8 && c != 10 && c != 12 && c != 14)
					&& cell[c - 9].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (darkChecker[j].getCell() == c) {
							whiteChecker[i].freeToMoveCell.push_back(c - 9);
							whiteChecker[i].eatenChecker[0] = j;
							whiteChecker[i].cellOfEatenChecker[0] = c - 9;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
				for (c = checkerCell + 7; c < 62; c += 7) {
					if (!cell[c].getIsEmpty()) break;
					whiteChecker[i].freeToMoveCell.push_back(c);
				}
				if ((c != 62 && c != 60 && c != 58 && c != 56 && c != 49 && c != 51 && c != 53 && c != 55)
					&& whiteChecker[i].getCell() != 55 && cell[c + 7].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (darkChecker[j].getCell() == c) {
							whiteChecker[i].freeToMoveCell.push_back(c + 7);
							whiteChecker[i].eatenChecker[3] = j;
							whiteChecker[i].cellOfEatenChecker[3] = c + 7;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
			}
			//Все остальные
			else {
				int c;
				//Северо-восток
				for (c = checkerCell - 7; c > 1; c -= 7) {
					if (!cell[c].getIsEmpty()) break;
					whiteChecker[i].freeToMoveCell.push_back(c);
					if (c == 7 || c == 23 || c == 39 || c == 55) break;
				}
				if ((c != 7 && c != 5 && c != 3 && c != 1 && c != 7 && c != 23 && c != 39 && c != 55) &&
					(whiteChecker[i].getCell() != 8 && whiteChecker[i].getCell() != 10 && whiteChecker[i].getCell() != 12 && whiteChecker[i].getCell() != 14) &&
					(whiteChecker[i].getCell() != 30 && whiteChecker[i].getCell() != 46 && whiteChecker[i].getCell() != 62) &&
					cell[c - 7].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (darkChecker[j].getCell() == c) {
							whiteChecker[i].freeToMoveCell.push_back(c - 7);
							whiteChecker[i].eatenChecker[1] = j;
							whiteChecker[i].cellOfEatenChecker[1] = c - 7;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
				//Юго-восток
				for (c = checkerCell + 9; c < 62; c += 9) {
					if (!cell[c].getIsEmpty()) break;
					whiteChecker[i].freeToMoveCell.push_back(c);
					if (c == 7 || c == 23 || c == 39 || c == 55) break;
				}
				if ((c != 62 && c != 60 && c != 58 && c != 56 && c != 7 && c != 23 && c != 39 && c != 55) &&
					(whiteChecker[i].getCell() != 49 && whiteChecker[i].getCell() != 51 && whiteChecker[i].getCell() != 53 && whiteChecker[i].getCell() != 55) &&
					(whiteChecker[i].getCell() != 30 && whiteChecker[i].getCell() != 46 && whiteChecker[i].getCell() != 62) &&
					cell[c + 9].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (darkChecker[j].getCell() == c) {
							whiteChecker[i].freeToMoveCell.push_back(c + 9);
							whiteChecker[i].eatenChecker[2] = j;
							whiteChecker[i].cellOfEatenChecker[2] = c + 9;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
				//Северо-Запад
				for (c = checkerCell - 9; c > 1; c -= 9) {
					if (!cell[c].getIsEmpty()) break;
					whiteChecker[i].freeToMoveCell.push_back(c);
					if (c == 8 || c == 24 || c == 40 || c == 56) break;
				}
				if ((c != 7 && c != 5 && c != 3 && c != 1 && c != 8 && c != 24 && c != 40 && c != 56) &&
					(whiteChecker[i].getCell() != 8 && whiteChecker[i].getCell() != 10 && whiteChecker[i].getCell() != 12 && whiteChecker[i].getCell() != 14) &&
					(whiteChecker[i].getCell() != 1 && whiteChecker[i].getCell() != 17 && whiteChecker[i].getCell() != 33 && whiteChecker[i].getCell() != 49) &&
					cell[c - 9].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (darkChecker[j].getCell() == c) {
							whiteChecker[i].freeToMoveCell.push_back(c - 9);
							whiteChecker[i].eatenChecker[0] = j;
							whiteChecker[i].cellOfEatenChecker[0] = c - 9;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
				//Юго-запад
				for (c = checkerCell + 7; c < 62; c += 7) {
					if (!cell[c].getIsEmpty()) break;
					whiteChecker[i].freeToMoveCell.push_back(c);
					if (c == 8 || c == 24 || c == 40 || c == 56) break;
				}
				if ((c != 62 && c != 60 && c != 58 && c != 56 && c != 8 && c != 24 && c != 40 && c != 56) &&
					(whiteChecker[i].getCell() != 49 && whiteChecker[i].getCell() != 51 && whiteChecker[i].getCell() != 53 && whiteChecker[i].getCell() != 55) &&
					(whiteChecker[i].getCell() != 1 && whiteChecker[i].getCell() != 17 && whiteChecker[i].getCell() != 33 && whiteChecker[i].getCell() != 49) &&
					cell[c + 7].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (darkChecker[j].getCell() == c) {
							whiteChecker[i].freeToMoveCell.push_back(c + 7);
							whiteChecker[i].eatenChecker[3] = j;
							whiteChecker[i].cellOfEatenChecker[3] = c + 7;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
			}
		}
	}
	if (!whiteIsMove) {
		int checkerCell = darkChecker[i].getCell();
		darkChecker[i].freeToMoveCell.clear();
		if (!darkChecker[i].getKing()) {
			//Узнаем куда ходить для черной шашки стоящей в крайнем левом столбце
			if (checkerCell == 8 || checkerCell == 24 || checkerCell == 40 || checkerCell == 56) {
				if (cell[checkerCell + 9].getIsEmpty() && eatenForMove == 0)
					darkChecker[i].freeToMoveCell.push_back(checkerCell + 9);

				//Право низ
				if (!cell[checkerCell + 9].getIsEmpty() && cell[checkerCell + 18].getIsEmpty() &&
					checkerCell != 49 && checkerCell != 51 && checkerCell != 53 && checkerCell != 55) {
					for (int j = 0; j < 12; j++) {
						if (whiteChecker[j].getCell() == checkerCell + 9) {
							darkChecker[i].freeToMoveCell.push_back(checkerCell + 18);
							darkChecker[i].eatenChecker[2] = j;
							eatingCheckers.push_back(i);
							break;
						}

					}
				}
				//Право верх
				if (!cell[checkerCell - 7].getIsEmpty() && cell[checkerCell - 14].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (whiteChecker[j].getCell() == checkerCell - 7) {
							darkChecker[i].freeToMoveCell.push_back(checkerCell - 14);
							darkChecker[i].eatenChecker[1] = j;
							eatingCheckers.push_back(i);
							break;
						}

					}
				}

			}
			//Узнаем куда ходить для черной шашки стоящей в крайнем правом столбце
			else if (checkerCell == 7 || checkerCell == 23 || checkerCell == 39 || checkerCell == 55) {
				if (cell[checkerCell + 7].getIsEmpty())
					darkChecker[i].freeToMoveCell.push_back(checkerCell + 7);

				//Лево верх
				if (!cell[checkerCell - 9].getIsEmpty() && cell[checkerCell - 18].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (whiteChecker[j].getCell() == checkerCell - 9) {
							darkChecker[i].freeToMoveCell.push_back(checkerCell - 18);
							darkChecker[i].eatenChecker[0] = j;
							eatingCheckers.push_back(i);
							break;
						}

					}
				}
				//Лево низ
				if (!cell[checkerCell + 7].getIsEmpty() && cell[checkerCell + 14].getIsEmpty() &&
					checkerCell != 49 && checkerCell != 51 && checkerCell != 53 && checkerCell != 55) {
					for (int j = 0; j < 12; j++) {
						if (whiteChecker[j].getCell() == checkerCell + 7) {
							darkChecker[i].freeToMoveCell.push_back(checkerCell + 14);
							darkChecker[i].eatenChecker[3] = j;
							eatingCheckers.push_back(i);
							break;
						}

					}
				}

			}
			else {
				if (cell[checkerCell + 7].getIsEmpty())
					darkChecker[i].freeToMoveCell.push_back(checkerCell + 7);

				if (checkerCell != 14 && checkerCell != 30 && checkerCell != 46 && checkerCell != 62) {
					//Право верх
					if (!cell[checkerCell - 7].getIsEmpty() && cell[checkerCell - 14].getIsEmpty() &&
						checkerCell != 14 && checkerCell != 12 && checkerCell != 10 && checkerCell != 8 &&
						checkerCell != 30 && checkerCell != 46 && checkerCell != 62) {
						for (int j = 0; j < 12; j++) {
							if (whiteChecker[j].getCell() == checkerCell - 7) {
								darkChecker[i].freeToMoveCell.push_back(checkerCell - 14);
								darkChecker[i].eatenChecker[1] = j;
								eatingCheckers.push_back(i);
								break;
							}

						}
					}
					//Право низ
					if (!cell[checkerCell + 9].getIsEmpty() && cell[checkerCell + 18].getIsEmpty() &&
						checkerCell != 49 && checkerCell != 51 && checkerCell != 53 && checkerCell != 55 &&
						checkerCell != 14 && checkerCell != 30 && checkerCell != 46 && checkerCell != 62) {
						for (int j = 0; j < 12; j++) {
							if (whiteChecker[j].getCell() == checkerCell + 9) {
								darkChecker[i].freeToMoveCell.push_back(checkerCell + 18);
								darkChecker[i].eatenChecker[2] = j;
								eatingCheckers.push_back(i);
								break;
							}

						}
					}
				}

				if (cell[checkerCell + 9].getIsEmpty())
					darkChecker[i].freeToMoveCell.push_back(checkerCell + 9);

				if (checkerCell != 1 && checkerCell != 17 && checkerCell != 33 && checkerCell != 49) {
					//Лево верх
					if (!cell[checkerCell - 9].getIsEmpty() && cell[checkerCell - 18].getIsEmpty() &&
						checkerCell != 14 && checkerCell != 12 && checkerCell != 10 && checkerCell != 8 &&
						checkerCell != 1 && checkerCell != 17 && checkerCell != 33 && checkerCell != 49) {
						for (int j = 0; j < 12; j++) {
							if (whiteChecker[j].getCell() == checkerCell - 9) {
								darkChecker[i].freeToMoveCell.push_back(checkerCell - 18);
								darkChecker[i].eatenChecker[0] = j;
								eatingCheckers.push_back(i);
								break;
							}

						}
					}
					//Лево низ
					if (!cell[checkerCell + 7].getIsEmpty() && cell[checkerCell + 14].getIsEmpty() &&
						checkerCell != 49 && checkerCell != 51 && checkerCell != 53 && checkerCell != 55 &&
						checkerCell != 1 && checkerCell != 17 && checkerCell != 33 && checkerCell != 49) {
						for (int j = 0; j < 12; j++) {
							if (whiteChecker[j].getCell() == checkerCell + 7) {
								darkChecker[i].freeToMoveCell.push_back(checkerCell + 14);
								darkChecker[i].eatenChecker[3] = j;
								eatingCheckers.push_back(i);
								break;
							}
						}
					}
				}
			}
		}
		else {
			//Узнаем куда ходить черной дамке стоящей в крайнем левом столбце
			if (checkerCell == 8 || checkerCell == 24 || checkerCell == 40 || checkerCell == 56) {
				int c;
				for (c = checkerCell - 7; c > 1; c -= 7) {
					if (!cell[c].getIsEmpty()) break;
					darkChecker[i].freeToMoveCell.push_back(c);
				}
				if ((c != 7 && c != 5 && c != 3 && c != 1 && c != 8 && c != 10 && c != 12 && c != 14)
					&& darkChecker[i].getCell() != 8 && cell[c - 7].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (whiteChecker[j].getCell() == c) {
							darkChecker[i].freeToMoveCell.push_back(c - 7);
							darkChecker[i].eatenChecker[1] = j;
							darkChecker[i].cellOfEatenChecker[1] = c - 7;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
				for (c = checkerCell + 9; c < 62; c += 9) {
					if (!cell[c].getIsEmpty()) break;
					darkChecker[i].freeToMoveCell.push_back(c);
				}
				if ((c != 62 && c != 60 && c != 58 && c != 56 && c != 49 && c != 51 && c != 53 && c != 55)
					&& cell[c + 9].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (whiteChecker[j].getCell() == c) {
							darkChecker[i].freeToMoveCell.push_back(c + 9);
							darkChecker[i].eatenChecker[2] = j;
							darkChecker[i].cellOfEatenChecker[2] = c + 9;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
			}
			//Узнаем куда ходить белой дамке стоящей в крайнем правом столбце
			else if (checkerCell == 7 || checkerCell == 23 || checkerCell == 39 || checkerCell == 55) {
				int c;
				for (c = checkerCell - 9; c > 1; c -= 9) {
					if (!cell[c].getIsEmpty()) break;
					darkChecker[i].freeToMoveCell.push_back(c);
				}
				if ((c != 7 && c != 5 && c != 3 && c != 1 && c != 8 && c != 10 && c != 12 && c != 14)
					&& cell[c - 9].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (whiteChecker[j].getCell() == c) {
							darkChecker[i].freeToMoveCell.push_back(c - 9);
							darkChecker[i].eatenChecker[0] = j;
							darkChecker[i].cellOfEatenChecker[0] = c - 9;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
				for (c = checkerCell + 7; c < 62; c += 7) {
					if (!cell[c].getIsEmpty()) break;
					darkChecker[i].freeToMoveCell.push_back(c);
				}
				if ((c != 62 && c != 60 && c != 58 && c != 56 && c != 49 && c != 51 && c != 53 && c != 55)
					&& darkChecker[i].getCell() != 55 && cell[c + 7].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (whiteChecker[j].getCell() == c) {
							darkChecker[i].freeToMoveCell.push_back(c + 7);
							darkChecker[i].eatenChecker[3] = j;
							darkChecker[i].cellOfEatenChecker[3] = c + 7;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
			}
			//Все остальные
			else {
				int c;
				//Северо-восток
				for (c = checkerCell - 7; c > 1; c -= 7) {
					if (!cell[c].getIsEmpty()) break;
					darkChecker[i].freeToMoveCell.push_back(c);
					if (c == 7 || c == 23 || c == 39 || c == 55) break;
				}
				if ((c != 7 && c != 5 && c != 3 && c != 1 && c != 7 && c != 23 && c != 39 && c != 55) &&
					(darkChecker[i].getCell() != 8 && darkChecker[i].getCell() != 10 && darkChecker[i].getCell() != 12 && darkChecker[i].getCell() != 14) &&
					(whiteChecker[i].getCell() != 30 && whiteChecker[i].getCell() != 46 && whiteChecker[i].getCell() != 62) &&
					cell[c - 7].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (whiteChecker[j].getCell() == c) {
							darkChecker[i].freeToMoveCell.push_back(c - 7);
							darkChecker[i].eatenChecker[1] = j;
							darkChecker[i].cellOfEatenChecker[1] = c - 7;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
				//Юго-восток
				for (c = checkerCell + 9; c < 62; c += 9) {
					if (!cell[c].getIsEmpty()) break;
					darkChecker[i].freeToMoveCell.push_back(c);
					if (c == 7 || c == 23 || c == 39 || c == 55) break;
				}
				if ((c != 62 && c != 60 && c != 58 && c != 56 && c != 7 && c != 23 && c != 39 && c != 55) &&
					(darkChecker[i].getCell() != 49 && darkChecker[i].getCell() != 51 && darkChecker[i].getCell() != 53 && darkChecker[i].getCell() != 55) &&
					(whiteChecker[i].getCell() != 30 && whiteChecker[i].getCell() != 46 && whiteChecker[i].getCell() != 62) &&
					cell[c + 9].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (whiteChecker[j].getCell() == c) {
							darkChecker[i].freeToMoveCell.push_back(c + 9);
							darkChecker[i].eatenChecker[2] = j;
							darkChecker[i].cellOfEatenChecker[2] = c + 9;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
				//Северо-Запад
				for (c = checkerCell - 9; c > 1; c -= 9) {
					if (!cell[c].getIsEmpty()) break;
					darkChecker[i].freeToMoveCell.push_back(c);
					if (c == 8 || c == 24 || c == 40 || c == 56) break;
				}
				if ((c != 7 && c != 5 && c != 3 && c != 1 && c != 8 && c != 24 && c != 40 && c != 56) &&
					(darkChecker[i].getCell() != 8 && darkChecker[i].getCell() != 10 && darkChecker[i].getCell() != 12 && darkChecker[i].getCell() != 14) &&
					(whiteChecker[i].getCell() != 1 && whiteChecker[i].getCell() != 17 && whiteChecker[i].getCell() != 33 && whiteChecker[i].getCell() != 49) &&
					cell[c - 9].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (whiteChecker[j].getCell() == c) {
							darkChecker[i].freeToMoveCell.push_back(c - 9);
							darkChecker[i].eatenChecker[0] = j;
							darkChecker[i].cellOfEatenChecker[0] = c - 9;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
				//Юго-запад
				for (c = checkerCell + 7; c < 62; c += 7) {
					if (!cell[c].getIsEmpty()) break;
					darkChecker[i].freeToMoveCell.push_back(c);
					if (c == 8 || c == 24 || c == 40 || c == 56) break;
				}
				if ((c != 62 && c != 60 && c != 58 && c != 56 && c != 8 && c != 24 && c != 40 && c != 56) &&
					(darkChecker[i].getCell() != 49 && darkChecker[i].getCell() != 51 && darkChecker[i].getCell() != 53 && darkChecker[i].getCell() != 55) &&
					(whiteChecker[i].getCell() != 1 && whiteChecker[i].getCell() != 17 && whiteChecker[i].getCell() != 33 && whiteChecker[i].getCell() != 49) &&
					cell[c + 7].getIsEmpty()) {
					for (int j = 0; j < 12; j++) {
						if (whiteChecker[j].getCell() == c) {
							darkChecker[i].freeToMoveCell.push_back(c + 7);
							darkChecker[i].eatenChecker[3] = j;
							darkChecker[i].cellOfEatenChecker[3] = c + 7;
							eatingCheckers.push_back(i);
							break;
						}
					}
				}
			}
		}
	}
}

void Desk::update(int mouseX, int mouseY, bool mouseIsPressed) {
	if (startOfMove) {
		chousenChekker = -1;//Убираем метку с выбранной ранее шашки

		for (int i = 0; i < 12; i++) {
			checkerСhecking(i);
		}

		startOfMove = false;
	}

	int movedChecker = -1;
	int indexOfChousenCell = cellIsChousen(mouseX, mouseY, mouseIsPressed);

	if (indexOfChousenCell > -1) {
		//Узнаем какая шашка выбрана
		if (!cell[indexOfChousenCell].getIsEmpty()) {
			if (whiteIsMove) {
				for (int j = 0; j < 12; j++) {
					if (whiteChecker[j].getCell() == indexOfChousenCell) chousenChekker = j;
				}
			}
			if (!whiteIsMove) {
				for (int j = 0; j < 12; j++) {
					if (darkChecker[j].getCell() == indexOfChousenCell) chousenChekker = j;
				}
			}
		}
		//Выделяем свободные для хода зоны
		if (chousenChekker > -1) {//Если шашка выбрана
			ellowSquareVector.clear();
			ellowSquareVector.push_back(ellowSquare);
			ellowSquareVector[0].setPosition(mouseX - mouseX % 64, mouseY - mouseY % 64);

			freeToMoveCell.clear();

			if (whiteIsMove) {
				for (auto i : whiteChecker[chousenChekker].freeToMoveCell) {
					ellowSquareVector.push_back(ellowSquare);
					ellowSquareVector[ellowSquareVector.size() - 1].setPosition(i % 8 * 64, i / 8 * 64);
				}
				if (!ellowSquareVector.empty()) {
					freeToMoveCell = whiteChecker[chousenChekker].freeToMoveCell;
				}
			}
			if (!whiteIsMove) {
				for (auto i : darkChecker[chousenChekker].freeToMoveCell) {
					ellowSquareVector.push_back(ellowSquare);
					ellowSquareVector[ellowSquareVector.size() - 1].setPosition(i % 8 * 64, i / 8 * 64);
				}
				if (!ellowSquareVector.empty()) {
					freeToMoveCell = darkChecker[chousenChekker].freeToMoveCell;
				}
			}
		}
		//Если есть куда ходить
		if (!freeToMoveCell.empty()) {
			if (whiteIsMove) {
				for (auto i : freeToMoveCell) {
					if (i == indexOfChousenCell) {
						bool existOfEatenChecker = false;
						for (auto eaten : whiteChecker[chousenChekker].eatenChecker) if (eaten > -1) existOfEatenChecker = true;

						int eatingChecker = -1;
						for (auto eating : eatingCheckers)
							if (chousenChekker == eating) eatingChecker = eating;

						int move = -1;//Направление движения
						if (indexOfChousenCell + 18 == whiteChecker[chousenChekker].getCell())
							move = 0;
						if (indexOfChousenCell + 14 == whiteChecker[chousenChekker].getCell())
							move = 1;
						if (indexOfChousenCell - 18 == whiteChecker[chousenChekker].getCell())
							move = 2;
						if (indexOfChousenCell - 14 == whiteChecker[chousenChekker].getCell())
							move = 3;

						whiteChecker[chousenChekker].setX(i % 8 * 64);
						whiteChecker[chousenChekker].setY(i / 8 * 64);
						whiteChecker[chousenChekker].setSpritePosition();
						whiteChecker[chousenChekker].freeToMoveCell.clear();
						cell[whiteChecker[chousenChekker].getCell()].setIsEmpty(true);

						whiteChecker[chousenChekker].setCell(i);
						cell[whiteChecker[chousenChekker].getCell()].setIsEmpty(false);

						freeToMoveCell.clear();
						ellowSquareVector.clear();

						//Съела
						if (!whiteChecker[chousenChekker].getKing()) {
							if (existOfEatenChecker && move != -1) {
								cell[darkChecker[whiteChecker[chousenChekker].eatenChecker[move]].getCell()].setIsEmpty(true);
								darkChecker[whiteChecker[chousenChekker].eatenChecker[move]].setCell(0);
								darkChecker[whiteChecker[chousenChekker].eatenChecker[move]].setX(-100);
								darkChecker[whiteChecker[chousenChekker].eatenChecker[move]].setSpritePosition();
								eatingChecker = -1;
								eatenForMove++;
							}
						}
						else {
							for (int j = 0; j < 4; j++) {
								if (indexOfChousenCell == whiteChecker[chousenChekker].cellOfEatenChecker[j]) {
									cell[darkChecker[whiteChecker[chousenChekker].eatenChecker[j]].getCell()].setIsEmpty(true);
									darkChecker[whiteChecker[chousenChekker].eatenChecker[j]].setCell(0);
									darkChecker[whiteChecker[chousenChekker].eatenChecker[j]].setX(-100);
									darkChecker[whiteChecker[chousenChekker].eatenChecker[j]].setSpritePosition();
									eatingChecker = -1;
									eatenForMove++;
								}
							}
						}

						int newCheckerCell = whiteChecker[chousenChekker].getCell();
						if (newCheckerCell == 1 || newCheckerCell == 3 || newCheckerCell == 5 || newCheckerCell == 7)
							whiteChecker[chousenChekker].setKing(true);

						endOfMove = true;
						movedChecker = chousenChekker;

						break;
					}
				}
			}
			if (!whiteIsMove) {
				for (auto i : freeToMoveCell) {
					if (i == indexOfChousenCell) {
						bool existOfEatenChecker = false;
						for (auto eaten : darkChecker[chousenChekker].eatenChecker) if (eaten > -1) existOfEatenChecker = true;

						int eatingChecker = -1;
						for (auto eating : eatingCheckers)
							if (chousenChekker == eating) eatingChecker = eating;

						int move = -1;
						if (indexOfChousenCell + 18 == darkChecker[chousenChekker].getCell())
							move = 0;
						if (indexOfChousenCell + 14 == darkChecker[chousenChekker].getCell())
							move = 1;
						if (indexOfChousenCell - 18 == darkChecker[chousenChekker].getCell())
							move = 2;
						if (indexOfChousenCell - 14 == darkChecker[chousenChekker].getCell())
							move = 3;

						darkChecker[chousenChekker].setX(i % 8 * 64);
						darkChecker[chousenChekker].setY(i / 8 * 64);
						darkChecker[chousenChekker].setSpritePosition();
						darkChecker[chousenChekker].freeToMoveCell.clear();
						cell[darkChecker[chousenChekker].getCell()].setIsEmpty(true);

						darkChecker[chousenChekker].setCell(i);
						cell[darkChecker[chousenChekker].getCell()].setIsEmpty(false);

						freeToMoveCell.clear();
						ellowSquareVector.clear();

						//Съела
						if (!darkChecker[chousenChekker].getKing()) {
							if (existOfEatenChecker && move != -1) {
								int j = darkChecker[chousenChekker].eatenChecker[move];//Индекс удаляемой шашки
								cell[whiteChecker[j].getCell()].setIsEmpty(true);
								whiteChecker[j].setCell(0);
								whiteChecker[j].setX(-100);
								whiteChecker[j].setSpritePosition();
								eatingChecker = -1;
								eatenForMove++;
							}
						}
						else {
							for (int j = 0; j < 4; j++) {
								if (indexOfChousenCell == darkChecker[chousenChekker].cellOfEatenChecker[j]) {
									cell[whiteChecker[darkChecker[chousenChekker].eatenChecker[j]].getCell()].setIsEmpty(true);
									whiteChecker[darkChecker[chousenChekker].eatenChecker[j]].setCell(0);
									whiteChecker[darkChecker[chousenChekker].eatenChecker[j]].setX(-100);
									whiteChecker[darkChecker[chousenChekker].eatenChecker[j]].setSpritePosition();
									eatingChecker = -1;
									eatenForMove++;
								}
							}
						}

						int newCheckerCell = darkChecker[chousenChekker].getCell();
						if (newCheckerCell == 56 || newCheckerCell == 58 || newCheckerCell == 60 || newCheckerCell == 62)
							darkChecker[chousenChekker].setKing(true);

						endOfMove = true;
						movedChecker = chousenChekker;

						break;
					}
				}
			}
		}

	}//Конец действия при выбранной клетке

	if (endOfMove  && movedChecker != -1) {
		eatingCheckers.clear();
		for (int j = 0; j < 12; j++) {
			for (int i = 0; i < 4; i++) {
				whiteChecker[j].eatenChecker[i] = -1;
				whiteChecker[j].cellOfEatenChecker[i] = -1;
			}
			for (int i = 0; i < 4; i++) {
				darkChecker[j].eatenChecker[i] = -1;
				darkChecker[j].cellOfEatenChecker[i] = -1;
			}
		}
		if (eatenForMove > 0 ) {

			for (int i = 0; i < 12; i++) {
				checkerСhecking(i);
			}

			bool existOfEatenChecker = false;
			if (whiteIsMove) {
				for (auto eaten : whiteChecker[movedChecker].eatenChecker) if (eaten > -1) existOfEatenChecker = true;
			}
			if (!whiteIsMove) {
				for (auto eaten : darkChecker[movedChecker].eatenChecker) if (eaten > -1) existOfEatenChecker = true;
			}

			if (!existOfEatenChecker) {
				whiteIsMove = !whiteIsMove;
				eatenForMove = 0;
				startOfMove = true;
				endOfMove = false;
			}
		}
		else {
			endOfMove = false;
			startOfMove = true;
			whiteIsMove = !whiteIsMove;
		}
	}
}

DeskState* Desk::getState() {
	DeskState* deskState = new DeskState;
	for (int i = 0; i < 12; i++) {
		deskState->whiteChecker[i] = whiteChecker[i];
		deskState->darkChecker[i] = darkChecker[i];
	}
	deskState->whiteIsMove = whiteIsMove;
	for (int i = 0; i < 64; i++) {
		deskState->cell[i] = cell[i];
	}
	deskState->startOfMove = startOfMove;
	deskState->endOfMove = endOfMove;
	deskState->chousenChekker = chousenChekker;
	deskState->ellowSquare = ellowSquare;
	deskState->freeToMoveCell = freeToMoveCell;
	deskState->eatingCheckers = eatingCheckers;
	deskState->eatenForMove = eatenForMove;
	return deskState;
}

void Desk::setState(DeskState* deskState) {
	for (int i = 0; i < 12; i++) {
		whiteChecker[i] = deskState->whiteChecker[i];
		darkChecker[i] = deskState->darkChecker[i];
	}
	whiteIsMove = deskState->whiteIsMove;
	for (int i = 0; i < 64; i++) {
		cell[i] = deskState->cell[i];
	}
	startOfMove = deskState->startOfMove;
	endOfMove = deskState->endOfMove;
	chousenChekker = deskState->chousenChekker;
	ellowSquare = deskState->ellowSquare;
	freeToMoveCell = deskState->freeToMoveCell;
	eatingCheckers = deskState->eatingCheckers;
	eatenForMove = deskState->eatenForMove;
}

int Desk::getWeight() {
	int ret = 0;
	for (int i = 0; i < 12; i++) if (whiteChecker[i].getCell() != 0) ret--;
	for (int i = 0; i < 12; i++) if (darkChecker[i].getCell() != 0) ret++;
	return ret;
}

bool Desk::getEndOfGame() {
	bool ret = true;
	for (int j = 0; j < 12; j++) {
		if (whiteIsMove) {
			ret = ( (bool)whiteChecker[j].freeToMoveCell.empty() || whiteChecker[j].getCell() == 0) && ret;
		}
		if (!whiteIsMove) {
			ret = (  (bool)darkChecker[j].freeToMoveCell.empty() || darkChecker[j].getCell() == 0) && ret;
		}
	}
	return ret;
}