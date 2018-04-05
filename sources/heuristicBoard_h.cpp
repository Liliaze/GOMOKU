#include "heuristicBoard.h"

void HeuristicBoard::clearOne_h(char x, char y, char vx, char vy)
{
	char before, beforeWall;
	char after, afterWall;

	getAdjacent(x, y, vx, vy, before, after, beforeWall, afterWall);
	if (after > 4 || before > 4)
		REMOVE_THREAT(GET_HH(heuristic[(unsigned char)x][(unsigned char)y]));
	else
		REMOVE_THREAT(4);
}

void HeuristicBoard::searchFreeThree_h(char x, char y, char vx, char vy, unsigned char &result)
{
	char before, beforeWall;
	char after, afterWall;
	unsigned char tmp;

	getAdjacent(x, y, vx, vy, before, after, beforeWall, afterWall);
	if (!beforeWall && !afterWall) {
		tmp = GET_HH(heuristic[(unsigned char)x-(before*vx)][(unsigned char)y-(before*vy)]);
		if (tmp + after == 3) {
			result += 1;
			return;
		}
		tmp = GET_HH(heuristic[(unsigned char)x+(after*vx)][(unsigned char)y+(after*vy)]);
		if (tmp + before == 3) {
			result += 1;
		}
	}
}


void HeuristicBoard::updateThreat_h(char x, char y, char vx, char vy)
{
	char before, beforeWall;
	char after, afterWall;
	unsigned char tmp;

	getAdjacent(x, y, vx, vy, before, after, beforeWall, afterWall);
	if (before + after >= 6)
		fiveLine += 1;
	if (!beforeWall) {
		tmp = GET_HH(heuristic[(unsigned char)x-(before*vx)][(unsigned char)y-(before*vy)]);
		REMOVE_THREAT(tmp);
		tmp += after;
		heuristic[(unsigned char)x-(before*vx)][(unsigned char)y-(before*vy)] = SET_HH(heuristic[(unsigned char)x-(before*vx)][(unsigned char)y-(before*vy)], tmp);
		fiveValue(tmp, GET_HH(heuristic[(unsigned char)x][(unsigned char)y]));
		ADD_THREAT(tmp);
	}
	if (!afterWall) {
		tmp = GET_HH(heuristic[(unsigned char)x+(after*vx)][(unsigned char)y+(after*vy)]);
		REMOVE_THREAT(tmp);
		tmp += before;
		heuristic[(unsigned char)x+(after*vx)][(unsigned char)y+(after*vy)] = SET_HH(heuristic[(unsigned char)x+(after*vx)][(unsigned char)y+(after*vy)], tmp);
		fiveValue(tmp, GET_HH(heuristic[(unsigned char)x][(unsigned char)y]));
		ADD_THREAT(tmp);
	}
}

void HeuristicBoard::removeEnnemyThreat_h(char x, char y, char vx, char vy)
{
	char before, beforeWall;
	char after, afterWall;

	getAdjacent(x, y, vx, vy, before, after, beforeWall, afterWall);
	heuristic[(unsigned char)x][(unsigned char)y] = SET_HH(heuristic[(unsigned char)x][(unsigned char)y], (after+before-2));
	ADD_THREAT((after+before-2) > 4 ? 4 : (after+before-2));
}

void HeuristicBoard::removeThreat_h(char x, char y, char vx, char vy)
{
	char before, beforeWall;
	char after, afterWall;
	unsigned char tmp, oldLength;

	getAdjacent(x, y, vx, vy, before, after, beforeWall, afterWall);
	if (before + after >= 6)
		fiveLine -= 1;
	oldLength = before + after - 1;
	if (!beforeWall) {
		tmp = GET_HH(heuristic[(unsigned char)x-(before*vx)][(unsigned char)y-(before*vy)]);
		if (tmp) {
			REMOVE_THREAT((tmp > 4 && oldLength <= 5) ? 4 : tmp);
			tmp -= after;
			heuristic[(unsigned char)x-(before*vx)][(unsigned char)y-(before*vy)] = SET_HH(heuristic[(unsigned char)x-(before*vx)][(unsigned char)y-(before*vy)], tmp);
			ADD_THREAT((tmp > 4 && before <= 5) ? 4 : tmp);
		}
	}
	if (!afterWall) {
		tmp = GET_HH(heuristic[(unsigned char)x+(after*vx)][(unsigned char)y+(after*vy)]);
		if (tmp) {
			REMOVE_THREAT((tmp > 4 && oldLength <= 5) ? 4 : tmp);
			tmp -= before;
			heuristic[(unsigned char)x+(after*vx)][(unsigned char)y+(after*vy)] = SET_HH(heuristic[(unsigned char)x+(after*vx)][(unsigned char)y+(after*vy)], tmp);
			ADD_THREAT((tmp > 4 && after <= 5) ? 4 : tmp);
		}
	}
	heuristic[(unsigned char)x][(unsigned char)y] = SET_HH(heuristic[(unsigned char)x][(unsigned char)y], (after+before-2));
	ADD_THREAT((after+before-2) > 4 ? 4 : (after+before-2));
}
