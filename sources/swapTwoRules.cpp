#include "swapTwoRules.h"

SwapTwoRules::SwapTwoRules() : 	Rules()
{
}

SwapTwoRules::~SwapTwoRules()
{
}

Rules *SwapTwoRules::copy() {
	SwapTwoRules *cpy = new SwapTwoRules();
	cpy->setGomoku(gomoku);
	cpy->setTurnCounter(turnCounter);
	return cpy;
}

bool SwapTwoRules::canPutStone(HeuristicBoard *heuristic, int x, int y) {
	if (heuristic->checkDoubleFreeThree((unsigned char)x, (unsigned char)y))
		return false;
	if (gomoku->getStone(x, y) == FREE)
		return true;
	return false;
}

void	SwapTwoRules::specificRules(Interface &interface) {
	if (turnCounter > 6){
		return;
	}
	else if (turnCounter == 0)
		interface.setRulesText("Put first stone\nwhere you want.", BRULESX, BRULESY);
	else if (turnCounter == 1) {
		interface.setRulesText("Again you\nPut second stone\nwhere you want.", BRULESX, BRULESY);
		gomoku->swapPlayer();
		gomoku->setCurrentPlayer(gomoku->aBlackPlayer());
	}
	else if (turnCounter == 2){
		interface.setRulesText("Again, again you\nPut third stone\nwhere you want.", BRULESX, BRULESY);
		gomoku->swapPlayer(true);
		gomoku->setCurrentPlayer(gomoku->aBlackPlayer());
	}
	else if (turnCounter == 3){
		interface.setRulesText("Play White now\nor black after\nor put 2 more", WRULESX , WRULESY);
		gomoku->getCurrentPlayer()->playSwapTwoStep1(gomoku, *this, interface);
	}
	else if (turnCounter == 4){
		if (getTwoMoreStone()) {
			interface.setRulesText("P2 : two more stone\nput the second stone\nwhere you want", WRULESX, WRULESY);
			gomoku->swapPlayer();
			gomoku->setCurrentPlayer(gomoku->aWhitePlayer());
		}
		else
			interface.getPtrRulesText()->setString("No specific rules");
	}
	else if (turnCounter == 5 && getTwoMoreStone()) { //au joueur 1 de choisir sa couleur...
		gomoku->getCurrentPlayer()->playSwapTwoStep2(gomoku, *this, interface);
	}
	else if (turnCounter == 6){
		interface.getPtrRulesText()->setString("No specific rules");
	}
	return;
}
