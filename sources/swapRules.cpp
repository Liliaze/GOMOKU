#include "swapRules.h"

SwapRules::SwapRules() : 	Rules()
{
}

SwapRules::~SwapRules()
{
}

Rules *SwapRules::copy() {
	SwapRules *cpy = new SwapRules();
	cpy->setGomoku(gomoku);
	cpy->setTurnCounter(turnCounter);
	return cpy;
}

bool SwapRules::canPutStone(HeuristicBoard *heuristic, int x, int y) {
	if (heuristic->checkDoubleFreeThree((unsigned char)x, (unsigned char)y))
		return false;
	if (gomoku->getStone(x, y) == FREE)
		return true;
	return false;
}

void	SwapRules::specificRules(Interface &interface) {
	if (turnCounter > 4){
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
		gomoku->resetColorPlayer();
		gomoku->setCurrentPlayer(gomoku->aBlackPlayer());
	}
	else if (turnCounter == 3){
		interface.setRulesText("Play White now\nor black after", WRULESX , WRULESY);
		gomoku->getCurrentPlayer()->playSimpleSwap(gomoku, *this, interface);
	}
	else if (turnCounter == 4){
		interface.getPtrRulesText()->setString("No specific rules");
	}
	return;
}
