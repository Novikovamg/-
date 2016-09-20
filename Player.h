#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Desk.h"
#include <memory>

float const SMALLEST_FLOAT = -2147483647;

struct AbstractState;

struct Ñoordinates {
	int x, y;
};

struct Move {
	int cellFrom, cellTo;
	std::shared_ptr<AbstractState> newAbstractState;
};

struct AbstractState {
	float weight;
	bool whiteIsMove;
	std::shared_ptr<DeskState> deskState;
	std::vector<Move> moves;
	
	void setChecker() {
		for (int i = 0; i < 12; i++) {
			if (whiteIsMove) {
				for (auto it : deskState->whiteChecker[i].freeToMoveCell) {
					Move move;
					move.cellFrom = deskState->whiteChecker[i].getCell();
					if (move.cellFrom == 0) continue;
					move.cellTo = it;
					moves.push_back(move);
				}
			}
			if (!whiteIsMove) {
				for (auto it : deskState->darkChecker[i].freeToMoveCell) {
					Move move;
					move.cellFrom = deskState->darkChecker[i].getCell();
					if (move.cellFrom == 0)  continue;
					move.cellTo = it;
					moves.push_back(move);
				}
			}
		}
	}
};

class Tree {
public:
	Tree(Texture *imageOfDesk, DeskState *deskState) {
		desk = std::shared_ptr<Desk>(new Desk(imageOfDesk));
		desk->setState(deskState);
		top = makeAbstractState();
		makeAllAbstractStates(top,6);
	}

	//Âîçâğàùàåì AbstractState äëÿ òîãî ñîñòîÿíèÿ, â êîòîğîì íàõîäèòñÿ ñåé÷àñ äîñêà
	std::shared_ptr<AbstractState> makeAbstractState() {
		std::shared_ptr<AbstractState> ret(new AbstractState);
		desk->update(10, 10, false);
		for (int i = 0; i < 12; i++) desk->checkerÑhecking(i);
		ret->deskState = std::shared_ptr<DeskState>(desk->getState());
		ret->whiteIsMove = desk->getWhiteIsMove();
		ret->weight = desk->getWeight();
		ret->setChecker();
		return ret;
	}

	//Ñîçäàåì êîîğäèíàòû ìûøè èç èíäåêñà êëåòêè
	Ñoordinates makeMouseCoordinates(int cell) {
		Ñoordinates ret;
		ret.x = cell % 8 * 64;
		ret.y = cell / 8 * 64;
		return ret;
	}

	//Äåëàåì øàã íà äîñêå
	void makeStep(int cellFrom, int cellTo) {
		int mouseXFrom = makeMouseCoordinates(cellFrom).x;
		int mouseYFrom = makeMouseCoordinates(cellFrom).y;
		int mouseXTo = makeMouseCoordinates(cellTo).x;
		int mouseYTo = makeMouseCoordinates(cellTo).y;

		desk->update(mouseXFrom, mouseYFrom, true);
		desk->update(mouseXTo, mouseYTo, true);
	}

	float makeOneAbstractState(Move& move, DeskState *state, int level) {
		float weight;

		makeStep(move.cellFrom, move.cellTo);
		move.newAbstractState = makeAbstractState();

		if (level >= 0) weight = makeAllAbstractStates(move.newAbstractState, level);
		else { weight = desk->getWeight(); }
		
		desk->setState(state);
		move.newAbstractState->deskState.reset();

		return weight;
	}

	int makeAllAbstractStates(std::shared_ptr<AbstractState>  ptr, int level) {
		float weight = 0;
		level--;
		for (auto &it : ptr->moves) {
			weight += makeOneAbstractState(it, ptr->deskState.get(), level);
		}
		ptr->weight = weight / ptr->moves.size();
		return weight;
	}
	
	void print() {
		std::cout << top->weight << std::endl;
		for (auto it : top->moves)
			std::cout << "from: " << it.cellFrom << " to: " << it.cellTo << " weight: " << it.newAbstractState->weight << std::endl;
	}

	int getBestMove() {
		float bestWeight = SMALLEST_FLOAT;
		int ret = 0;
		for (int i = 0; i < top->moves.size(); i++) {
			if (bestWeight < top->moves[i].newAbstractState->weight) {
				bestWeight = top->moves[i].newAbstractState->weight;
				ret = i;
			}else if (bestWeight == top->moves[i].newAbstractState->weight && std::rand() % 10 < 5) {
				bestWeight = top->moves[i].newAbstractState->weight;
				ret = i;
			}
		}
		return ret;
	}

	Ñoordinates getFrom(int moveIndex) {
		Ñoordinates ret;
		return ret = makeMouseCoordinates(top->moves[moveIndex].cellFrom);
	}
	Ñoordinates getTo(int moveIndex) {
		Ñoordinates ret;
		return ret = makeMouseCoordinates(top->moves[moveIndex].cellTo);
	}

private:
	std::shared_ptr<AbstractState> top; 
	std::shared_ptr<Desk> desk;
};




class Player {
public:
	void makeDecision(Texture *imageOfDesk, DeskState *deskState) {
		Tree decisionTree(imageOfDesk, deskState);
		int bestMove = decisionTree.getBestMove();
		moveFrom = decisionTree.getFrom(bestMove);
		moveTo = decisionTree.getTo(bestMove);
		decisionTree.print();
	}
	Ñoordinates getMoveFrome() { return moveFrom; }
	Ñoordinates getMoveTo() { return moveTo; }
private:
	Ñoordinates moveFrom, moveTo;
}; 