#include "humanPlayer.h"
#include "rules.h"
#include "interface.h"

HumanPlayer::HumanPlayer() : Player()
{
	DEBUG << "NEW HUMAN\n";
	human = true;
}

HumanPlayer::~HumanPlayer()
{
	DEBUG << "BYE HUMAN\n";
}


void HumanPlayer::play(Rules &rules, Interface &i) {
	(void)rules;
	while (!this->played)
	{
		i.checkEvent(this);
	}
    putStone(coordPlayed.x, coordPlayed.y);
	return;
}


void HumanPlayer::playSimpleSwap(Gomoku *gomoku, Rules &rules, Interface &interface) {
	(void)rules;
	sf::Vector2<int>	tmp(0,0);
	
	interface.setRulesText("Do you want play :\nWhite now\nor black after ?", WRULESX , WRULESY);
	interface.getSpriteList().push_back(interface.getSpriteBlackBox());
	interface.getSpriteList().push_back(interface.getSpriteWhiteBox());
	interface.update();
	while(1) {
		tmp = interface.checkEventSwap();
		if (interface.onWhiteBoxP2(tmp.x, tmp.y)) {
			interface.setRulesText("You choose white\nIt's your turn", WRULESX , WRULESY);
			break;
		}
		else if (interface.onBlackBoxP2(tmp.x, tmp.y)) {
			interface.setRulesText("P2 choose black\nP1 is now white", WRULESX , WRULESY);
			gomoku->swapPlayer();
			gomoku->setCurrentPlayer(gomoku->aBlackPlayer());
			break;
		}
	}
	interface.getSpriteList().pop_back();
	interface.getSpriteList().pop_back();
	interface.update();
}

void HumanPlayer::playSwapTwoStep1(Gomoku *gomoku, Rules &rules, Interface &interface) {
	(void)rules;
	sf::Vector2<int>	tmp(0,0);
	while(1) {
		tmp = interface.checkEventSwap();
		if (interface.onWhiteBoxP1(tmp.x, tmp.y)) {
			break;
		}
		else if (interface.onBlackBoxP1(tmp.x, tmp.y)) {
			gomoku->swapPlayer();
			gomoku->setCurrentPlayer(gomoku->aBlackPlayer());
			break;
		}
		/*else if (interface.onPlayTwoMore()) {

		}*/
	}
}

void HumanPlayer::playSwapTwoStep2(Gomoku *gomoku, Rules &rules, Interface &interface) {
	(void)rules;
	sf::Vector2<int>	tmp(0,0);
	while(1) {
		tmp = interface.checkEventSwap();
		if (interface.onWhiteBoxP1(tmp.x, tmp.y)) {
			break;
		}
		else if (interface.onBlackBoxP1(tmp.x, tmp.y)) {
			gomoku->swapPlayer();
			gomoku->setCurrentPlayer(gomoku->aBlackPlayer());
			break;
		}
	}
}
