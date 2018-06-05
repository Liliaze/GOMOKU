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
	if (turnCounter > 4)
		return;
	else if (turnCounter == 0)
		interface.setRulesText("Put 3 first stone,\nwhere you want.", BRULESX, BRULESY);
	else if (turnCounter == 1) {
		interface.setRulesText("Put 3 first stone,\nwhere you want.", BRULESX, BRULESY);
		gomoku->swapPlayer();
		gomoku->setCurrentPlayer(gomoku->aBlackPlayer());
	}
	else if (turnCounter == 2){
		interface.setRulesText("Put 3 first stone,\nwhere you want.", BRULESX, BRULESY);
		gomoku->updatePlayer();
	}
	else if (turnCounter == 3){
		bool waitChoice = false;
		interface.setRulesText("Do you want play now with blank ?", WRULESY / 2, BOARD_UP - 30);
		interface.getPtrRulesText()->setCharacterSize(30);
		interface.getSpriteList().push_back(interface.getSpriteYes());
		interface.getSpriteList().push_back(interface.getSpriteNo());
		interface.update();
		if (gomoku->getCurrentPlayer()->human){
			while(!waitChoice) {
				waitChoice = interface.checkEventSwap();
			}
		}
		else {
			//implémenter choix de stratégie IA.
		}
		interface.getSpriteList().pop_back();
		interface.getSpriteList().pop_back();
		interface.getPtrRulesText()->setCharacterSize(20);
		interface.update();
	}
	else if (turnCounter == 4)
		interface.setRulesText("No specific rules", BRULESX, BRULESY);
	return;
}
