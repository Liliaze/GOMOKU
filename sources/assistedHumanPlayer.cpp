#include "assistedHumanPlayer.h"
#include "interface.h"
#include "rules.h"

AssistedHumanPlayer::AssistedHumanPlayer(std::array<unsigned char, 11> dw) : Player(), MinMaxDynamicPlayer(dw), HumanPlayer() {
    if (human) {
        DEBUG << "I'm human and robot, i'm the better of humanity\n";
    }else{
        DEBUG << "erreur constructeur, échec humanisation\n";}
}

AssistedHumanPlayer::~AssistedHumanPlayer() {
    DEBUG << "BYE ASSISTED HUMAN\n";
}

void AssistedHumanPlayer::playForHelp(Rules &rules, Interface &i) {
   DEBUG << "Need help? Wait...\n";
    int x = -1;
	int y = -1;
	startMinMax(x, y, rules, i);
	i.putStoneToHelp(x, y);
   DEBUG << "Try this, the best\n";
	return;
}

void AssistedHumanPlayer::play(Rules &rules, Interface &i) {
   DEBUG << "l'humain doit jouer :\n";
	HumanPlayer::play(rules,i);
   DEBUG << "merci d'avoir joue :\n";
}

void AssistedHumanPlayer::playSimpleSwap(Gomoku *gomoku, Rules &rules, Interface &interface) {
	sf::Vector2<int>	tmp(0,0);

	MinMaxDynamicPlayer::playSimpleSwap(gomoku,rules,interface);
	if (getColor() == WHITE) {
		interface.setRulesText("IA advises WHITE\nMake your choice", WRULESX , WRULESY);
	}
	else {
		gomoku->resetColorPlayer();
		gomoku->setCurrentPlayer(gomoku->aWhitePlayer());
		interface.setRulesText("IA advises BLACK\nMake your choice", WRULESX , WRULESY);
	}
	interface.getSpriteList().push_back(interface.getSpriteBlackBox());
	interface.getSpriteList().push_back(interface.getSpriteWhiteBox());
	interface.update();
	while(1) {
		tmp = interface.checkEventSwap();
		if (interface.onWhiteBoxP2(tmp.x, tmp.y)) {
			interface.setRulesText("P2 choose white\nP2 must play", WRULESX , WRULESY);
			break;
		}
		else if (interface.onBlackBoxP2(tmp.x, tmp.y)) {
			interface.setRulesText("P2 choose BLACK\nP1 is now white\nP1 must play", WRULESX , WRULESY);
			gomoku->swapPlayer();
			gomoku->setCurrentPlayer(gomoku->aBlackPlayer());
			break;
		}
	}
	interface.getSpriteList().pop_back();
	interface.getSpriteList().pop_back();
	interface.update();
	return;
}

bool AssistedHumanPlayer::playSwapTwoStep1(Gomoku *gomoku, Rules &rules, Interface &interface) {
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
	return false;
}

void AssistedHumanPlayer::playSwapTwoStep2(Gomoku *gomoku, Rules &rules, Interface &interface) {
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
