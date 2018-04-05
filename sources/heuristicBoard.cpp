#include "heuristicBoard.h"

const long long HeuristicBoard::levels[6] = {L0, L1, L2, L3, L4, HAS_WON};

HeuristicBoard::HeuristicBoard()
{
//	DEBUG << "new 1\n";
	bzero(heuristic, sizeof(short int[GW][GH]));
	totalCaptured = 0;
	fiveLine = 0;
	stone = 0;
	gomoku = NULL;
	score = 0;

}

HeuristicBoard::HeuristicBoard(HeuristicBoard *copyFrom) {
//	DEBUG << "memcopy\n";
//	DEBUG << "copy : " << heuristic << "/" << copyFrom->heuristic << "\n";
	totalCaptured = copyFrom->totalCaptured;
	fiveLine = copyFrom->fiveLine;
	stone = copyFrom->stone;
	gomoku = copyFrom->gomoku;
	score = copyFrom->score;
	memcpy((void*)heuristic, (void*)(copyFrom->heuristic), sizeof(short int[GW][GH]));
//	failure("quit");
//	DEBUG << "end copy\n";
}

HeuristicBoard::~HeuristicBoard()
{
}

void HeuristicBoard::getAdjacent(char x, char y, char vx, char vy, char &before, char &after, char &beforeWall, char &afterWall)
{
	before = 1;
	after = 1;
	beforeWall = 0;
	afterWall = 0;
	char xi = vx;
	char yi = vy;
//	DEBUG << "stone : " << stone << "\n";
//	DEBUG << "SEARCH ADJACENT : ";
	while (1) {
//		DEBUG << "+";
		if (gomoku->getStone(x+xi,y+yi) == stone)
			after += 1;
		else {
			if (gomoku->getStone(x+xi,y+yi) != FREE) {
				afterWall = 1;
			}
			break;
		}
		xi += vx;
		yi += vy;
	}
//	DEBUG << "\n";
	xi = vx;
	yi = vy;
	while (1) {
		if (gomoku->getStone(x-xi,y-yi) == stone)
			before += 1;
		else {
			if (gomoku->getStone(x-xi,y-yi) != FREE) {
				beforeWall = 1;
			}
			break;
		}
		xi += vx;
		yi += vy;
	}
}

void HeuristicBoard::clearOne(char x, char y, char vx, char vy, char shift, short int mask)
{
	char before, beforeWall;
	char after, afterWall;

	getAdjacent(x, y, vx, vy, before, after, beforeWall, afterWall);
	if (after > 4 || before > 4)
		REMOVE_THREAT(GET_THREAT(heuristic[(unsigned char)x][(unsigned char)y], mask, shift));
	else
		REMOVE_THREAT(4);
}

HeuristicBoard& HeuristicBoard::clear(unsigned char x, unsigned char y)
{
//	DEBUG << "clear\n";
	if (GET_HH(heuristic[x][y]) < 5)
		REMOVE_THREAT(GET_HH(heuristic[x][y]));
	else
		clearOne_h(x, y, 1, 0);

	if (GET_VH(heuristic[x][y]) < 5)
		REMOVE_THREAT(GET_VH(heuristic[x][y]));
	else
		clearOne_v(x, y, 0, 1);

	if (GET_DLH(heuristic[x][y]) < 5)
		REMOVE_THREAT(GET_DLH(heuristic[x][y]));
	else
		clearOne_dl(x, y, -1, 1);

	if (GET_DRH(heuristic[x][y]) < 5)
		REMOVE_THREAT(GET_DRH(heuristic[x][y]));
	else
		clearOne_dr(x, y, 1, 1);

	heuristic[x][y] = 0;
	return *this;
}

void HeuristicBoard::fiveValue(unsigned char &value, unsigned char heuristic) {
	if (value > 4 && heuristic < 4)
		value = 4;
}


void HeuristicBoard::searchFreeThree(char x, char y, char vx, char vy, char shift, short int mask, unsigned char &result)
{
	char before, beforeWall;
	char after, afterWall;
	unsigned char tmp;

	getAdjacent(x, y, vx, vy, before, after, beforeWall, afterWall);
	if (!beforeWall && !afterWall) {
		tmp = GET_THREAT(heuristic[(unsigned char)x-(before*vx)][(unsigned char)y-(before*vy)], mask, shift);
		if (tmp + after == 3) {
			result += 1;
			return;
		}
		tmp = GET_THREAT(heuristic[(unsigned char)x+(after*vx)][(unsigned char)y+(after*vy)], mask, shift);
		if (tmp + before == 3) {
			result += 1;
		}
	}
}

void HeuristicBoard::updateThreat(char x, char y, char vx, char vy, char shift, short int mask)
{
	char before, beforeWall;
	char after, afterWall;
	unsigned char tmp;

	getAdjacent(x, y, vx, vy, before, after, beforeWall, afterWall);
	if (before + after >= 6)
		fiveLine += 1;
	if (!beforeWall) {
		tmp = GET_THREAT(heuristic[(unsigned char)x-(before*vx)][(unsigned char)y-(before*vy)], mask, shift);
		REMOVE_THREAT(tmp);
		tmp += after;
		heuristic[(unsigned char)x-(before*vx)][(unsigned char)y-(before*vy)] = SET_THREAT(heuristic[(unsigned char)x-(before*vx)][(unsigned char)y-(before*vy)], tmp, mask, shift);
		fiveValue(tmp, GET_THREAT(heuristic[(unsigned char)x][(unsigned char)y], mask, shift));
		ADD_THREAT(tmp);
	}
	if (!afterWall) {
		tmp = GET_THREAT(heuristic[(unsigned char)x+(after*vx)][(unsigned char)y+(after*vy)], mask, shift);
		REMOVE_THREAT(tmp);
		tmp += before;
		heuristic[(unsigned char)x+(after*vx)][(unsigned char)y+(after*vy)] = SET_THREAT(heuristic[(unsigned char)x+(after*vx)][(unsigned char)y+(after*vy)], tmp, mask, shift);
		fiveValue(tmp, GET_THREAT(heuristic[(unsigned char)x][(unsigned char)y], mask, shift));
		ADD_THREAT(tmp);
	}
}

void HeuristicBoard::removeEnnemyThreat(char x, char y, char vx, char vy, char shift, short int mask)
{
	char before, beforeWall;
	char after, afterWall;

	getAdjacent(x, y, vx, vy, before, after, beforeWall, afterWall);
	heuristic[(unsigned char)x][(unsigned char)y] = SET_THREAT(heuristic[(unsigned char)x][(unsigned char)y], (after+before-2), mask, shift);
	ADD_THREAT((after+before-2) > 4 ? 4 : (after+before-2));
}

void HeuristicBoard::removeThreat(char x, char y, char vx, char vy, char shift, short int mask)
{
	char before, beforeWall;
	char after, afterWall;
	unsigned char tmp, oldLength;

	getAdjacent(x, y, vx, vy, before, after, beforeWall, afterWall);
	if (before + after >= 6)
		fiveLine -= 1;
	oldLength = before + after - 1;
	if (!beforeWall) {
		tmp = GET_THREAT(heuristic[(unsigned char)x-(before*vx)][(unsigned char)y-(before*vy)], mask, shift);
		if (tmp) {
			REMOVE_THREAT((tmp > 4 && oldLength <= 5) ? 4 : tmp);
			tmp -= after;
			heuristic[(unsigned char)x-(before*vx)][(unsigned char)y-(before*vy)] = SET_THREAT(heuristic[(unsigned char)x-(before*vx)][(unsigned char)y-(before*vy)], tmp, mask, shift);
			ADD_THREAT((tmp > 4 && before <= 5) ? 4 : tmp);
		}
	}
	if (!afterWall) {
		tmp = GET_THREAT(heuristic[(unsigned char)x+(after*vx)][(unsigned char)y+(after*vy)], mask, shift);
		if (tmp) {
			REMOVE_THREAT((tmp > 4 && oldLength <= 5) ? 4 : tmp);
			tmp -= before;
			heuristic[(unsigned char)x+(after*vx)][(unsigned char)y+(after*vy)] = SET_THREAT(heuristic[(unsigned char)x+(after*vx)][(unsigned char)y+(after*vy)], tmp, mask, shift);
			ADD_THREAT((tmp > 4 && after <= 5) ? 4 : tmp);
		}
	}
	heuristic[(unsigned char)x][(unsigned char)y] = SET_THREAT(heuristic[(unsigned char)x][(unsigned char)y], (after+before-2), mask, shift);
	ADD_THREAT((after+before-2) > 4 ? 4 : (after+before-2));
}

void HeuristicBoard::beCaptured(unsigned char x, unsigned char y)
{
	removeThreat_h(x, y, 0, 1);
	removeThreat_v(x, y, 1, 0);
	removeThreat_dr(x, y, 1, 1);
	removeThreat_dl(x, y, -1, 1);
}

void HeuristicBoard::capture(unsigned char x, unsigned char y)
{
	removeEnnemyThreat_h(x, y, 0, 1);
	removeEnnemyThreat_v(x, y, 1, 0);
	removeEnnemyThreat_dr(x, y, 1, 1);
	removeEnnemyThreat_dl(x, y, -1, 1);
	totalCaptured += 1;
}

HeuristicBoard& HeuristicBoard::put(unsigned char x, unsigned char y)
{
	updateThreat_h(x, y, 0, 1);
	updateThreat_v(x, y, 1, 0);
	updateThreat_dr(x, y, 1, 1);
	updateThreat_dl(x, y, -1, 1);
	return clear(x, y);
}

bool HeuristicBoard::checkDoubleFreeThree(unsigned char x, unsigned char y)
{
	unsigned char n = 0;
	searchFreeThree_h(x, y, 0, 1, n);
	searchFreeThree_v(x, y, 1, 0, n);
	searchFreeThree_dr(x, y, 1, 1, n);
	searchFreeThree_dl(x, y, -1, 1, n);
	return (n >= 2);
}

int HeuristicBoard::getBestLevel(unsigned char x, unsigned char y) {
	int r = 0;
	int tmp = 0;
	tmp = GET_HH(heuristic[x][y]);
	if (tmp > r)
		r = tmp;
	tmp = GET_VH(heuristic[x][y]);
	if (tmp > r)
		r = tmp;
	tmp = GET_DRH(heuristic[x][y]);
	if (tmp > r)
		r = tmp;
	tmp = GET_DLH(heuristic[x][y]);
	if (tmp > r)
		r = tmp;
	return r;
}

void HeuristicBoard::print(int lastX, int lastY) {
	DEBUG << "\n";
	int level;
	for (int j = 0; j < GH; j++) {
		for (int i = 0; i < GW; i++) {
			level = getBestLevel(i, j);
			if (gomoku->getStone(i, j) != FREE) {
				gomoku->printStone(i, j, lastX, lastY);
			} else if (level == 1) {
				DEBUG << BLUE << level << DEFAULT_COLOR;
			} else if (level == 2) {
				DEBUG << PURPLE << level << DEFAULT_COLOR;
			} else if (level == 3) {
				DEBUG << YELLOW << level << DEFAULT_COLOR;
			} else if (level == 4) {
				DEBUG << RED << level << DEFAULT_COLOR;
			} else if (level == 0) {
				DEBUG << "." << DEFAULT_COLOR;
			} else {
				DEBUG << BLACK_COLOR << HLL_RED << level << DEFAULT_COLOR;
			}
				
		}
		DEBUG << "\n";
	}
}
