#include "gomoku.h"
#include "player.h"
#include "humanPlayer.h"
#include "randomPlayer.h"
#include "minMaxDynamicPlayer.h"
#include "assistedHumanPlayer.h"
#include "noobIA.h"
#include "rules.h"
#include "defaultRules.h"
#include "interface.h"

using namespace sf;

Gomoku::Gomoku(Interface &interface) : interface(interface)
{
	whitePlayer = new HumanPlayer();
	blackPlayer = new HumanPlayer();
	rules = new DefaultRules();
	initGomoku();
}

Gomoku::Gomoku(Gomoku *copyFrom) : whitePlayer(copyFrom->aWhitePlayer()), blackPlayer(copyFrom->aBlackPlayer()), rules(copyFrom->aRules()), interface(copyFrom->getInterface())
{
	clone = true;
	memcpy((void*)board, (void*)(copyFrom->getBoard()), sizeof(Stone[GW][GH]));
	memcpy((void*)focus, (void*)(copyFrom->getFocus()), sizeof(bool[GW][GH]));
	currentPlayer = copyFrom->getCurrentPlayer();
}

void	Gomoku::initGomoku() {
	clone = false;
	for (int i = 0; i < GW; i++) {
		for (int j = 0; j < GH; j++) {
			board[i][j] = FREE;
			focus[i][j] = false;
		}
	}
	focus[GW/2][GH/2] = true;
	updateRules();
	interface.setGomoku(this);
	currentPlayer = blackPlayer;
}

void	Gomoku::resetColorPlayer()
{
	//HeuristicBoard	&tmpH = *(whitePlayer->getMyHeuristic());
	int		tmpCapture = whitePlayer->getNbCapture();
	
	updatePlayerInBlack(getBlackPlayer());
	updatePlayerInWhite(getWhitePlayer());
	//currentPlayer = blackPlayer;

	whitePlayer->setNbCapture(blackPlayer->getNbCapture());
	blackPlayer->setNbCapture(tmpCapture);
/*	whitePlayer->setMyHeuristic(*(whitePlayer->getEnnemyHeuristic()));
	blackPlayer->setMyHeuristic(tmpH);
	whitePlayer->setEnnemyHeuristic(*(blackPlayer->getMyHeuristic()));
	blackPlayer->setEnnemyHeuristic(*(whitePlayer->getMyHeuristic()));*/
}

void	Gomoku::swapPlayer() {
	//HeuristicBoard	&tmpH = *(whitePlayer->getMyHeuristic());
	int		tmpCapture = whitePlayer->getNbCapture();
	updatePlayerInBlack(getWhitePlayer());
	updatePlayerInWhite(getBlackPlayer());
	blackPlayer->setEnemy(whitePlayer);
	whitePlayer->setEnemy(blackPlayer);
	blackPlayer->setNbCapture(tmpCapture);
	whitePlayer->setNbCapture(blackPlayer->getNbCapture());
	/*whitePlayer->setMyHeuristic(*(whitePlayer->getEnnemyHeuristic()));
	blackPlayer->setMyHeuristic(tmpH);
	whitePlayer->setEnnemyHeuristic(*(blackPlayer->getMyHeuristic()));
	blackPlayer->setEnnemyHeuristic(*(whitePlayer->getMyHeuristic()));*/
}

void	Gomoku::updateRules()
{
	rules->setGomoku(this);
	rules->turnCounter = 0;
}

void	Gomoku::updatePlayerInWhite(Player &p)
{
	p.setSpriteStone(&(interface._whiteStone));
	p.setCanteen(interface.whiteCanteen);
	p.setGomoku(this);
	p.setColor(WHITE);
	p.setEnemy(blackPlayer);
}

void	Gomoku::updatePlayerInBlack(Player &p)
{
	p.setSpriteStone(&(interface._blackStone));
	p.setCanteen(interface.blackCanteen);
	p.setGomoku(this);
	p.setColor(BLACK);
	p.setEnemy(whitePlayer);
}

Gomoku::~Gomoku()
{
	if (!clone) {
		delete whitePlayer;
		delete blackPlayer;
		delete rules;
	}
}

void Gomoku::end() {
	interface.cleanInterface();
	interface.setState(GOODBYE);
	interface.update();
	sleep(1);
	interface.getWindow().close();
	exit(EXIT_SUCCESS);
}

void Gomoku::drawStone() {
	sf::Vector2<int> coord = interface.getCoordBoard(currentPlayer->coordPlayed.x, currentPlayer->coordPlayed.y);
		interface.putStone(*(currentPlayer->stoneSprite), coord.x , coord.y);
}

void Gomoku::start() {
	bool again = true;
	End end = NO_END;
	interface.setState(MENU);
	while (again) {
		std::vector<std::pair<unsigned char, unsigned char>> captured;
		currentPlayer = blackPlayer;
		while (interface.getState() == MENU)  {
			interface.checkEvent(NULL);
			interface.update();
		}
		int x = 0;
		int y = 0;
		updatePlayerInBlack(getBlackPlayer());
		updatePlayerInWhite(getWhitePlayer());
		setCurrentPlayer(aBlackPlayer());
		updateRules();
		while (!(end = rules->checkEnd(*currentPlayer))) {
			interface._allShape.clear();
			interface.updateRulesText();
			interface.updateNbOfTurn();
			rules->specificRules(interface);
			if (interface.visualAid && currentPlayer->getHuman()) {
				interface.updateHelperToPlay();
			}
			else
				interface._allHelpSprite.clear();
			interface.update();
			//PLAY
			interface.setTimeToPlay(interface._clockTurn.restart());
			currentPlayer->play(*rules, interface);
			interface.setTimeToPlay(interface._clockTurn.getElapsedTime());
			interface.updateTimerToPlay();
			x = currentPlayer->coordPlayed.x;
			y = currentPlayer->coordPlayed.y;
			//UPDATE HEURISTIC
			currentPlayer->myHeuristic.put(x, y);
			currentPlayer->ennemyHeuristic.clear(x, y);
			currentPlayer->myHeuristic.print(x, y);
			currentPlayer->ennemyHeuristic.print(x, y);
			printBoard(x, y);
			//DRAW
			drawStone();
			//CAPTURE
			checkCapture(*currentPlayer, x, y, *(currentPlayer->getEnemy()), captured);
			captureAll(*currentPlayer, currentPlayer->getEnemy()->getSpriteStone(), captured);
			//OBSERVE
			currentPlayer->getEnemy()->observe(*rules, x, y, captured);
			currentPlayer->observeMyCapture(captured);
			//END
			currentPlayer->played = false;
			currentPlayer = currentPlayer->getEnemy();
			interface.checkEvent(currentPlayer);
			captured.clear();
			rules->turnCounter += 1;
		}
		interface._allHelpSprite.clear();
		if (end == WHITE_WIN)
			interface.setState(WHITEWIN);
		else if (end == BLACK_WIN)
			interface.setState(BLACKWIN);
		else if (end == EQUALITY)
			interface.setState(EQUAL);
		while (interface.getState() != AGAIN) {
			interface.update();
			interface.checkEvent(currentPlayer);
		}
		while (interface.getState() == AGAIN) {
			interface.update();
			interface.checkEvent(currentPlayer);
		}
		initGomoku();
	}
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

void Gomoku::updateStartingFocus() {
	for (int i = 0; i < GW; i++) {
		for (int j = 0; j < GH; j++) {
			focus[i][j] = rules->startingFocus[i][j];
		}
	}
}
