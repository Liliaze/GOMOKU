#include "proRules.h"

ProRules::ProRules() : 	Rules()
{
	initStartingFocus();
}

void ProRules::initStartingFocus() {
	char schema[GW][GH] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,1,1,1,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,1,1,1,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,1,1,1,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};
	for (int i = 0; i < GW; i++) {
		for (int j = 0; j < GH; j++) {
				startingFocus[i][j] = schema[i][j];
		}
	}
}

ProRules::~ProRules()
{
}

Rules *ProRules::copy() {
	ProRules *cpy = new ProRules();
	cpy->setGomoku(gomoku);
	cpy->setTurnCounter(turnCounter);
	return cpy;
}

bool ProRules::canPutStone(HeuristicBoard *heuristic, int x, int y) {
	if (turnCounter == 0){
		if (x == 9 && y == 9)
			return true;
		else
			return false;
	}
	if (turnCounter == 2){
		if ((x <= 11 && x >= 7 && y <= 11 && y >= 7))
			return false;
	}
	if (heuristic->checkDoubleFreeThree((unsigned char)x, (unsigned char)y))
		return false;
	if (gomoku->getStone(x, y) == FREE)
		return true;
	return false;
}

void	ProRules::specificRules(Interface &interface) {
	if (turnCounter > 3)
		return;
	else if (turnCounter <= 2){
		interface.getShapeList().push_back(interface.getPtrFiveByFiveRect());
	}
	if (turnCounter == 0)
		interface.setRulesText("Play :\nin Middle = H8.", BRULESX, BRULESY);
	else if (turnCounter == 1)
		interface.setRulesText("Play :\nwhere you want", WRULESX, WRULESY);
	else if (turnCounter == 2)
		interface.setRulesText("Play :\noutside of square", BRULESX, BRULESY);
	else if (turnCounter == 3)
		interface.setRulesText("No specific rules", WRULESX, WRULESY);
	return;
}
