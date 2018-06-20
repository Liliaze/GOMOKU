#include "gomoku.h"
#include "player.h"

Player::Player() : myHeuristic(), ennemyHeuristic(), nbCapture(0), played(false), human(false)
{
	DEBUG << "NEW PLAYER\n";
}

Player::~Player() {
	DEBUG << "BYE PLAYER\n";
}

void Player::putStone(int x, int y) {
	if (stoneColor == WHITE) {
		DEBUG << DEFAULT_COLOR << HLL_GREY << "White player put stone in " << x << "/" << y << DEFAULT_COLOR << "\n";
	} else {
		DEBUG << DARK_BLACK << HLIGHT_GREY << "Black player put stone in " << x << "/" << y << DEFAULT_COLOR << "\n";
	}
	gomoku->setStone(stoneColor, x, y);
	gomoku->updateFocus(x, y);
}
void Player::unPutStone(int x, int y) {
	gomoku->unSetStone(x,y);
	//gomoku->cancelFocus(x,y);
}

void Player::observe(Rules &rules, int x, int y, std::vector<std::pair<unsigned char, unsigned char>> &captured) {
	(void)rules;
	myHeuristic.clear(x, y);
	ennemyHeuristic.put(x, y);
	for (auto it = captured.begin(); it != captured.end(); ++it) {
		myHeuristic.beCaptured(it->first, it->second);
		ennemyHeuristic.capture(it->first, it->second);
		DEBUG << RED << "CAPTURED BY ENNEMY !\n" << DEFAULT_COLOR;
//		myHeuristic.print(x, y);
//		ennemyHeuristic.print(x, y);
	}
}

void Player::observeMyCapture(std::vector<std::pair<unsigned char, unsigned char>> &captured) {
	for (auto it = captured.begin(); it != captured.end(); ++it) {
		myHeuristic.capture(it->first, it->second);
		ennemyHeuristic.beCaptured(it->first, it->second);
		DEBUG << GREEN << "CAPTURE ENNEMY STONES !\n" << DEFAULT_COLOR;
//		myHeuristic.print();
//		ennemyHeuristic.print();
	}
}
