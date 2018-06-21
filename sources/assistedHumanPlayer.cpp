#include "assistedHumanPlayer.h"
#include "interface.h"
#include "rules.h"

AssistedHumanPlayer::AssistedHumanPlayer(std::array<unsigned char, 11> dw) : Player(), MinMaxDynamicPlayer(dw), HumanPlayer() {
}

AssistedHumanPlayer::~AssistedHumanPlayer() {
}

void AssistedHumanPlayer::playForHelp(Rules &rules, Interface &i) {
    int x = -1;
	int y = -1;
	startMinMax(x, y, rules, i);
	i.putStoneToHelp(x, y);
	return;
}

void AssistedHumanPlayer::play(Rules &rules, Interface &i) {
	HumanPlayer::play(rules,i);
}

void AssistedHumanPlayer::playSimpleSwap(Gomoku *gomoku, Rules &rules, Interface &interface) {
	sf::Vector2<int>	tmp(0,0);

	MinMaxDynamicPlayer::playSimpleSwap(gomoku,rules,interface);
	if (getColor() == WHITE) {
		interface.setRulesText("IA advises WHITE\nMake your choice", WRULESX , WRULESY);
	}
	else {
		gomoku->swapPlayer(true);
		gomoku->setCurrentPlayer(gomoku->aWhitePlayer());
		interface.setRulesText("IA advises BLACK\nMake your choice", WRULESX , WRULESY);
	}
	HumanPlayer::playSimpleSwap(gomoku, rules, interface);
	return;
}

void AssistedHumanPlayer::playSwapTwoStep1(Gomoku *gomoku, Rules &rules, Interface &interface) {
	MinMaxDynamicPlayer::playSwapTwoStep1(gomoku,rules,interface);
	if (getColor() == WHITE && !rules.getTwoMoreStone()) {
		interface.setRulesText("IA advises WHITE\nMake your choice", WRULESX , WRULESY);
	}
	else if (getColor() == BLACK){
		gomoku->swapPlayer(true);
		gomoku->setCurrentPlayer(gomoku->aWhitePlayer());
		interface.setRulesText("IA advises BLACK\nMake your choice", WRULESX , WRULESY);
	}
	else {
		rules.setTwoMoreStone(false);
		interface.setRulesText("IA advises :\n'put 2 more stone'", WRULESX , WRULESY);
	}
	HumanPlayer::playSwapTwoStep1(gomoku,rules,interface);
	return;
}

void AssistedHumanPlayer::playSwapTwoStep2(Gomoku *gomoku, Rules &rules, Interface &interface) {
	playSimpleSwap(gomoku, rules, interface);
}