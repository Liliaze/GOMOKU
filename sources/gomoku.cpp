#include "gomoku.h"
#include "player.h"
#include "humanPlayer.h"
#include "randomPlayer.h"
#include "minMaxDynamicPlayer.h"
#include "noobIA.h"
#include "rules.h"
#include "interface.h"

using namespace sf;

Gomoku::Gomoku(Rules &rules, Interface &interface) : rules(rules), interface(interface)
{
	clone = false;
	whitePlayer = new MinMaxDynamicPlayer(5);
	blackPlayer = new MinMaxDynamicPlayer(5);
	for (int i = 0; i < GW; i++) {
		for (int j = 0; j < GH; j++) {
			board[i][j] = FREE;
			focus[i][j] = false;
		}
	}
	focus[GW/2][GH/2] = true;
	rules.setGomoku(this);
	interface.setGomoku(this);
	currentPlayer = blackPlayer;
}

void	Gomoku::updateWhitePlayer()
{
	whitePlayer->setSpriteStone(&(interface._whiteStone));
	whitePlayer->setCanteen(interface.whiteCanteen);
	whitePlayer->setGomoku(this);
	whitePlayer->setColor(WHITE);
	whitePlayer->setEnemy(blackPlayer);
}

void	Gomoku::updateBlackPlayer()
{
	blackPlayer->setSpriteStone(&(interface._blackStone));
	blackPlayer->setCanteen(interface.blackCanteen);
	blackPlayer->setGomoku(this);
	blackPlayer->setColor(BLACK);
	blackPlayer->setEnemy(whitePlayer);
}

Gomoku::Gomoku(Gomoku *copyFrom, Rules &copyRules) : whitePlayer(copyFrom->aWhitePlayer()), blackPlayer(copyFrom->aBlackPlayer()), rules(copyRules), interface(copyFrom->getInterface())
{
	clone = true;
	memcpy((void*)board, (void*)(copyFrom->getBoard()), sizeof(Stone[GW][GH]));
	memcpy((void*)focus, (void*)(copyFrom->getFocus()), sizeof(bool[GW][GH]));
	currentPlayer = copyFrom->getCurrentPlayer();
}

Gomoku::~Gomoku()
{
	if (!clone) {
		delete whitePlayer;
		delete blackPlayer;
	}
}

void Gomoku::end() {
	interface.setState(GOODBYE);
	interface.update();
	sleep(1);
	interface.getWindow().close();
	exit(1);
}

void Gomoku::drawStone() {
	sf::Vector2<int> coord = interface.getCoordBoard(currentPlayer->coordPlayed.x, currentPlayer->coordPlayed.y);
		interface.putStone(*(currentPlayer->stoneSprite), coord.x , coord.y);
}

void Gomoku::start() {

	std::vector<std::pair<unsigned char, unsigned char>> captured;
//	interface.setState(MENU);
	interface.setState(GAME);
	currentPlayer = blackPlayer;
	while (interface.getState() == MENU)  {
		interface.checkEvent(*currentPlayer);
        interface.update();
	}
	int x = 0;
	int y = 0;
	updateWhitePlayer();
	updateBlackPlayer();
	currentPlayer = blackPlayer;
	while (!rules.checkEnd(*currentPlayer)) {
        interface.update();
		//PLAY
		currentPlayer->play(rules, interface);
		x = currentPlayer->coordPlayed.x;
		y = currentPlayer->coordPlayed.y;
		//UPDATE HEURISTIC
		currentPlayer->myHeuristic.put(x, y);
		currentPlayer->ennemyHeuristic.clear(x, y);
		currentPlayer->myHeuristic.print(x, y);
		currentPlayer->ennemyHeuristic.print(x, y);
		//DRAW
		drawStone();
		//CAPTURE
		checkCapture(*currentPlayer, x, y, *(currentPlayer->getEnemy()), captured);
		captureAll(*currentPlayer, currentPlayer->getEnemy()->getSpriteStone(), captured);
		//OBSERVE
		currentPlayer->getEnemy()->observe(rules, x, y, captured);
		currentPlayer->observeMyCapture(captured);
		//END
		currentPlayer->played = false;
		currentPlayer = currentPlayer->getEnemy();
		interface.checkEvent(*currentPlayer);
		captured.clear();
		rules.turnCounter += 1;
		if (rules.turnCounter == 4)
			exit(1);
	}
	DEBUG << "Game end after " << rules.turnCounter << " turns\n";
	this->end();
}


Stone Gomoku::getStone(int x, int y) {
	 if (x < 0 || y < 0 || x >= GW || y >= GH)
	 	return OUT_LIMIT;
	return board[x][y];
}

void Gomoku::updateFocus(int x, int y) {
	for (int i = x-FOCUS; i <= x+FOCUS ; i++) {
		for (int j = y-FOCUS; j <= y+FOCUS ; j++) {
			if (i >= 0 && j >= 0 && i < GW && j < GH)
				focus[i][j] = true;
		}
	}
}

void Gomoku::printStone(int i, int j, int lastX, int lastY) {
	if (board[i][j] == WHITE) {
		DEBUG << ((lastX == i && lastY == j) ? GREEN : DEFAULT_COLOR) << HLL_GREY << "O" << DEFAULT_COLOR;
	} else if (board[i][j] == BLACK) {
		DEBUG << ((lastX == i && lastY == j) ? RED : DARK_BLACK) << HLIGHT_GREY << "X" << DEFAULT_COLOR;
	} else {
		if (focus[i][j]) {
			DEBUG << ".";
		} else {
			DEBUG << "#";
		}
	}
}

void Gomoku::printBoard(int lastX, int lastY) {
	DEBUG << "\n";
	for (int j = 0; j < GH; j++) {
		for (int i = 0; i < GW; i++) {
			printStone(i, j, lastX, lastY);
		}
		DEBUG << "\n";
	}
}

bool Gomoku::horizontalLine(Stone color, int x, int y) {
	for (int i = x; i < (x + 5); i++) {
		if (getStone(i, y) != color) {
			return false;
		}
	}
	return true;
}

bool Gomoku::verticalLine(Stone color, int x, int y) {
	for (int i = y; i < (y + 5); i++) {
		if (getStone(x, i) != color)
			return false;
	}
	return true;
}

bool Gomoku::rightDiagonal(Stone color, int x, int y) {
	for (int i = 0; i < 5; i++) {
		if (getStone(x + i, y + i) != color)
			return false;
	}
	return true;
}

bool Gomoku::leftDiagonal(Stone color, int x, int y) {
	for (int i = 0; i < 5; i++) {
		if (getStone(x - i, y + i) != color)
			return false;
	}
	return true;
}

bool Gomoku::checkLine(Stone color, int x, int y) {
	if (getStone(x, y) != color)
		return false;
	if (horizontalLine(color, x, y))
		return true;
	if (verticalLine(color, x, y))
		return true;
	if (rightDiagonal(color, x, y))
		return true;
	if (leftDiagonal(color, x, y))
		return true;
	return false;
}

bool Gomoku::fiveStoneLine(Stone color, int &x, int &y) {
	for (int i = 0; i < GW; i++) {
		for (int j = 0; j < GH; j++) {
			if (checkLine(color, i, j)) {
				x = i;
				y = j;
				return true;
			}
		}
	}
	return false;
}

void Gomoku::checkCapture(Player &current, unsigned char x, unsigned char y, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &result)
{
	checkLeft(current, x, y, enemy, result);
	checkRight(current, x, y, enemy, result);
	checkUp(current, x, y, enemy, result);
	checkDown(current, x, y, enemy, result);
	checkUpLeft(current, x, y, enemy, result);
	checkDownLeft(current, x, y, enemy, result);
	checkUpRight(current, x, y, enemy, result);
	checkDownRight(current, x, y, enemy, result);
}

void Gomoku::captureAll(Player &current, sf::Sprite *spriteEnemy, std::vector<std::pair<unsigned char, unsigned char>> &captured) {
	
	for (auto it = captured.begin(); it != captured.end(); ++it) {
		capture(current, spriteEnemy, it->first, it->second);
	}
}

void Gomoku::capture(Player &current, Sprite *spriteEnemy, int x1, int y1) {
	current.nbCapture += 1;
	setStone(FREE, x1, y1);
			DEBUG << "CAPTURE = " << current.nbCapture << " " << x1 << " " << y1 << "\n";
	interface.capture(current, spriteEnemy, x1, y1); // ajoute 1 stone de la couleur enemy du current dans sa zone de capture, pour cela check le nb de capture et put des stones au bon endroit.
}

bool Gomoku::checkBetween(Stone colorEnemy, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
	return (getStone(x1, y1) == colorEnemy && getStone(x2,y2) == colorEnemy);
}

void Gomoku::checkLeft(Player &current, unsigned char xPlayed, unsigned char yPlayed, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured) {
	if (getStone(xPlayed - 3, yPlayed) == current.getColor()) {
		if (checkBetween(enemy.getColor(), xPlayed - 2, yPlayed, xPlayed - 1, yPlayed)) {
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed - 1, yPlayed));
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed - 2, yPlayed));
		}
	}
}

void Gomoku::checkRight(Player &current, unsigned char xPlayed, unsigned char yPlayed, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured) {
	if (getStone(xPlayed + 3, yPlayed) == current.getColor()) {
		if (checkBetween(enemy.getColor(), xPlayed + 2, yPlayed, xPlayed + 1, yPlayed)) {
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed + 1, yPlayed));
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed + 2, yPlayed));
		}
	}
}

void Gomoku::checkUp(Player &current, unsigned char xPlayed, unsigned char yPlayed, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured) {
	if (getStone(xPlayed, yPlayed - 3) == current.getColor()) {
		if (checkBetween(enemy.getColor(), xPlayed, yPlayed - 2, xPlayed, yPlayed - 1)) {
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed, yPlayed - 1));
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed, yPlayed - 2));
		}
	}
}

void Gomoku::checkDown(Player &current, unsigned char xPlayed, unsigned char yPlayed, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured) {
	if (getStone(xPlayed, yPlayed + 3) == current.getColor()) {
		if (checkBetween(enemy.getColor(), xPlayed, yPlayed + 2, xPlayed, yPlayed + 1)) {
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed, yPlayed + 1));
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed, yPlayed + 2));
		}
	}
}

void Gomoku::checkUpLeft(Player &current, unsigned char xPlayed, unsigned char yPlayed, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured) {
	if (getStone(xPlayed - 3, yPlayed - 3) == current.getColor()) {
		if (checkBetween(enemy.getColor(), xPlayed - 2, yPlayed - 2, xPlayed - 1, yPlayed - 1)) {
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed - 1, yPlayed - 1));
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed - 2, yPlayed - 2));
		}
	}
}

void Gomoku::checkDownLeft(Player &current, unsigned char xPlayed, unsigned char yPlayed, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured) {
	if (getStone(xPlayed - 3, yPlayed + 3) == current.getColor()) {
		if (checkBetween(enemy.getColor(), xPlayed - 2, yPlayed + 2, xPlayed - 1, yPlayed + 1)) {
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed - 1, yPlayed + 1));
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed - 2, yPlayed + 2));
		}
	}
}

void Gomoku::checkUpRight(Player &current, unsigned char xPlayed, unsigned char yPlayed, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured) {
	if (getStone(xPlayed + 3, yPlayed - 3) == current.getColor()) {
		if (checkBetween(enemy.getColor(), xPlayed + 2, yPlayed - 2, xPlayed + 1, yPlayed - 1)) {
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed + 1, yPlayed - 1));
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed + 2, yPlayed - 2));
		}
	}
}

void Gomoku::checkDownRight(Player &current, unsigned char xPlayed, unsigned char yPlayed, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured) {
	if (getStone(xPlayed + 3, yPlayed + 3) == current.getColor()) {
		if (checkBetween(enemy.getColor(), xPlayed + 2, yPlayed + 2, xPlayed + 1, yPlayed + 1)) {
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed + 1, yPlayed + 1));
			captured.push_back(std::pair<unsigned char, unsigned char>(xPlayed + 2, yPlayed + 2));
		}
	}
}
