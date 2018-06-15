#include "noobIA.h"
#include "rules.h"
#include "interface.h"

NoobIA::NoobIA() : Player()
{
	DEBUG << "\nNEW NOOBIA PLAYER\n";
}

NoobIA::~NoobIA()
{
	DEBUG << "\nBYE NOOBIA\n";
}

void NoobIA::play(Rules &rules, Interface &i) {
	(void)i;
	while (!this->played)
	{
		int x = rand() % GW;
		int y = rand() % GH;
		if (rules.canPutStone(*this, x, y)) {
			setCoordPlayed(x, y);
			this->putStone(x, y);
			this->played = true;
		}
	}
	return;
}

void NoobIA::playSimpleSwap(Gomoku *gomoku, Rules &rules, Interface &i) {
	(void)rules;
	int x = rand() % GW;
	if (x%2) {
		i.getPtrRulesText()->setString("IA choose white");
		return;
	}
	else {
		gomoku->swapPlayer();
		gomoku->setCurrentPlayer(gomoku->aBlackPlayer());
		i.getPtrRulesText()->setString("IA choose black");
		return;
	}
}

void NoobIA::playSwapTwoStep1(Gomoku *gomoku, Rules &rules, Interface &i) {
	(void)rules;
	sf::Vector2<int>	tmp(0,0);
	int x = rand() % GW;
	if (x%3) {
		return;
	}
	else if (x%2){
		gomoku->swapPlayer();
		gomoku->setCurrentPlayer(gomoku->aBlackPlayer());
		i.getPtrRulesText()->setString("IA choose black");
		return;
	}
	else {
		i.getPtrRulesText()->setString("IA choose white");
	}
}

void NoobIA::playSwapTwoStep2(Gomoku *gomoku, Rules &rules, Interface &interface) {
	(void)rules;
	(void)gomoku;
	(void)interface;
}
