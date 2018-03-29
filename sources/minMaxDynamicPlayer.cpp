#include "minMaxDynamicPlayer.h"
#include "rules.h"

const unsigned char MinMaxDynamicPlayer::depthWidths[10] = {20, 20, 10, 10, 10, 10, 10, 6, 6, 6};

Choice::Choice(unsigned char x, unsigned char y, HeuristicBoard *myHeuristic, HeuristicBoard *ennemyHeuristic, bool ennemy) : x(x), y(y), myHeuristic(myHeuristic), ennemyHeuristic(ennemyHeuristic)
{
	if (!ennemy) {
		this->myHeuristic.put(x, y);
		this->ennemyHeuristic.clear(x, y);
	} else {
		this->myHeuristic.clear(x, y);
		this->ennemyHeuristic.put(x, y);
	}
}

Choice::Choice() : x(0), y(0), myHeuristic(), ennemyHeuristic()
{
}

Choice::~Choice()
{
}


MinMaxDynamicPlayer::MinMaxDynamicPlayer(int d) : Player(), minMaxDepth(d)
{
	if (d > 10)
		failure("max depth = 10");
	/*if (d == 10)
		playerType = IA_HARD;
	else if (d < 10 && d >= 5)
		playerType = IA_NORMAL;
	else
		playerType = IA_EASY;
	*/
	DEBUG << "NEW MINMAXPLAYER\n";

}

MinMaxDynamicPlayer::~MinMaxDynamicPlayer()
{
	DEBUG << "BYE MINMAXPLAYER\n";
}

std::multimap<long long, Choice> MinMaxDynamicPlayer::heuristicMap(Rules &rules, Player *player, bool last, int depth, HeuristicBoard &myOrigin, HeuristicBoard &ennemyOrigin, bool focus) {
	std::multimap<long long, Choice> result;
	for (unsigned char j = 0; j < GH; j++) {
		for (unsigned char i = 0; i < GW; i++) {
			if ((!focus || gomoku->isFocus(i, j)) && rules.canPutStone(*player, i, j)) {

				if (player == this) {
					Choice choice(i, j, &myOrigin, &ennemyOrigin);
					gomoku->checkCapture(*this, i, j, *enemy, choice.captured);
					for (auto it = choice.captured.begin(); it != choice.captured.end(); ++it) {
						choice.ennemyHeuristic.beCaptured(it->first, it->second);
						choice.myHeuristic.capture(it->first, it->second);
					}
					result.insert(std::pair<long long, Choice>(-heuristic(choice.myHeuristic, choice.ennemyHeuristic, last, depth), choice));
				} else {
					Choice choice(i, j, &myOrigin, &ennemyOrigin, true);
					gomoku->checkCapture(*enemy, i, j, *this, choice.captured);
					for (auto it = choice.captured.begin(); it != choice.captured.end(); ++it) {
						choice.ennemyHeuristic.capture(it->first, it->second);
						choice.myHeuristic.beCaptured(it->first, it->second);
					}
					result.insert(std::pair<long long, Choice>(heuristic_e(choice.myHeuristic, choice.ennemyHeuristic, last, depth), choice));
				}
			}
		}
	}
	return result;
}

bool MinMaxDynamicPlayer::win(int &rx, int &ry, Rules &rules) {
	for (unsigned char j = 0; j < GH; j++) {
		for (unsigned char i = 0; i < GW; i++) {
			if (rules.canPutStone(*this, i, j)) {
				if (myHeuristic.getBestLevel(i, j) >= 4) {
					rx = i;
					ry = j;
					return true;
				}
			}
		}
	}
	return false;
}

void MinMaxDynamicPlayer::startMinMax(int &rx, int &ry, Rules &rules)
{
	long long option;
	long long best = MIN_LONG;
	long long maxBestOption = MIN_LONG;
	complexity = 0;
	std::multimap<long long, Choice> choices = heuristicMap(rules, this, ennemyHeuristic.fiveLine, minMaxDepth, myHeuristic, ennemyHeuristic, true);
	rx = choices.begin()->second.x;
	ry = choices.begin()->second.y;
	unsigned char i = 0;
//	if (ennemyHeuristic.score >= L4) {
//		i = depthWidths[0];
//	}
	for (auto it = choices.begin(); it != choices.end(); ++it) {
		if (i > depthWidths[0]) {
			DEBUG << DARK_YELLOW << "break\n" << DEFAULT_COLOR;
			break;
		}
		DEBUG << YELLOW << "try : " << (int)it->second.x << "/" << (int)it->second.y << DEFAULT_COLOR << "\n";
		gomoku->setStone(getColor(), it->second.x, it->second.y);
		for (auto it2 = it->second.captured.begin(); it2 != it->second.captured.end(); ++it2) {
			gomoku->setStone(FREE, it2->first, it2->second);
		}
		complexity++;
		option = min(minMaxDepth, MAX_LONG, maxBestOption, rules, it->second.myHeuristic, it->second.ennemyHeuristic);
		if (option >= BIG) {
			DEBUG << GREEN << "I CAN WIN : " <<  option << DEFAULT_COLOR << "\n";
		} else if (option <= -BIG) {
			DEBUG << RED << "I WILL LOOSE : " << option << DEFAULT_COLOR << "\n";
		} else {
			DEBUG << DARK_YELLOW << "score : " << option << DEFAULT_COLOR << "\n";
		}
		gomoku->setStone(FREE, it->second.x, it->second.y);
		for (auto it2 = it->second.captured.begin(); it2 != it->second.captured.end(); ++it2) {
			gomoku->setStone(enemy->getColor(), it2->first, it2->second);
		}
		if (option > best) {
			best = option;
			rx = it->second.x;
			ry = it->second.y;
//			if (best >= WIN_STATE) {
//				DEBUG << "WIN BREAK\n";
//				break;
//			}
		}
		if (option > maxBestOption) {
			maxBestOption = option;
		}
		i++;
	}
	DEBUG << "complexity : " << complexity << "\n";
}

long long MinMaxDynamicPlayer::min(int depth, long long minBestOption, long long maxBestOption, Rules &rules, HeuristicBoard myH, HeuristicBoard ennemyH)
{
	if (depth <= 0 || ennemyH.fiveLine || myH.totalCaptured >= 10) {
		return heuristic(myH, ennemyH, ennemyH.fiveLine, depth+1);
	}
	long long option;
	long long best = MAX_LONG;
	std::multimap<long long, Choice> choices = heuristicMap(rules, enemy, myH.fiveLine, depth, myH, ennemyH);
	unsigned char i = 0;
//	if (myH.score >= L4) {
//		i = depthWidths[minMaxDepth - depth];
//	}
	for (auto it = choices.begin(); it != choices.end(); ++it) {
		if (i > depthWidths[minMaxDepth - depth]) {
			break;
		}
		gomoku->setStone(enemy->getColor(), it->second.x, it->second.y);
		for (auto it2 = it->second.captured.begin(); it2 != it->second.captured.end(); ++it2) {
			if (gomoku->getStone(it2->first, it2->second) != getColor()) DEBUG << RED << "WRONG CAPTURE !\n" << DEFAULT_COLOR;
			gomoku->setStone(FREE, it2->first, it2->second);
		}
		complexity++;
		option = max(depth-1, minBestOption, maxBestOption, rules, it->second.myHeuristic, it->second.ennemyHeuristic);
		gomoku->setStone(FREE, it->second.x, it->second.y);
		for (auto it2 = it->second.captured.begin(); it2 != it->second.captured.end(); ++it2) {
			gomoku->setStone(getColor(), it2->first, it2->second);
		}
		if (option < best) {
			best = option;
			if (option < minBestOption) {
				minBestOption = option;
			}
			if (maxBestOption >= minBestOption - 1) {
				return best;
			}
		}
		i++;
	}
	return best;
}

long long MinMaxDynamicPlayer::max(int depth, long long minBestOption, long long maxBestOption, Rules &rules, HeuristicBoard myH, HeuristicBoard ennemyH)
{
	if (depth <= 0 || myH.fiveLine || ennemyH.totalCaptured >= 10) {
		return heuristic_e(myH, ennemyH, myH.fiveLine, depth+1);
	}
	long long option;
	long long best = MIN_LONG;
	std::multimap<long long, Choice> choices = heuristicMap(rules, this, ennemyH.fiveLine, depth, myH, ennemyH);
	unsigned char i = 0;
//	if (ennemyH.score >= L4) {
//		i = depthWidths[minMaxDepth - depth];
//	}
	for (auto it = choices.begin(); it != choices.end(); ++it) {
		if (i > depthWidths[minMaxDepth - depth]) {
			break;
		}
		gomoku->setStone(getColor(), it->second.x, it->second.y);
		for (auto it2 = it->second.captured.begin(); it2 != it->second.captured.end(); ++it2) {
			if (gomoku->getStone(it2->first, it2->second) != enemy->getColor()) DEBUG << RED << "WRONG CAPTURE !\n" << DEFAULT_COLOR;
			gomoku->setStone(FREE, it2->first, it2->second);
		}
		complexity++;
		option = min(depth-1, minBestOption, maxBestOption, rules, it->second.myHeuristic, it->second.ennemyHeuristic);
		gomoku->setStone(FREE, it->second.x, it->second.y);
		for (auto it2 = it->second.captured.begin(); it2 != it->second.captured.end(); ++it2) {
			gomoku->setStone(enemy->getColor(), it2->first, it2->second);
		}
		if (option > best) {
			best = option;
			if (option > maxBestOption) {
				maxBestOption = option;
			}
			if (maxBestOption >= minBestOption - 1) {
				return best;
			}
		}
		i++;
	}
	return best;
}


void MinMaxDynamicPlayer::play(Rules &rules, Interface &i) {
	(void)i;
	int x = -1;
	int y = -1;
//	if (!win(x, y, rules)) {
		DEBUG << "BEFORE PLAY\n";
		startMinMax(x, y, rules);
		DEBUG << "AFTER PLAY\n";
//	}
	DEBUG << "PUT : " << x << "/" << y << "\n";
	putStone(x, y);
	DEBUG << "SET\n";
	setCoordPlayed(x, y);
}

long long MinMaxDynamicPlayer::heuristic(HeuristicBoard &myH, HeuristicBoard &ennemyH, bool last, int depth) {
	if (last && !canAvoidDefeat(myH, ennemyH)) {
		return (-BIG * (depth));
	}
	if (ennemyH.fiveLine || ennemyH.totalCaptured >= 10) {
		return -BIG * (depth);
	}
	if (myH.fiveLine || myH.totalCaptured >= 10) {
		return BIG * (depth);
	}
	return (myH.score + CAPTURE_BONUS(myH.totalCaptured) - (ennemyH.score + CAPTURE_BONUS(ennemyH.totalCaptured)) );
}

bool MinMaxDynamicPlayer::canAvoidDefeat(HeuristicBoard &myH, HeuristicBoard &ennemyH) {
	if (!ennemyH.fiveLine || myH.totalCaptured >= 10)
		return true;
	return false;
}

long long MinMaxDynamicPlayer::heuristic_e(HeuristicBoard &myH, HeuristicBoard &ennemyH, bool last, int depth) {
	if (last && !canAvoidDefeat_e(myH, ennemyH)) {
		return (BIG * (depth));
	}
	if (myH.fiveLine || myH.totalCaptured >= 10) {
		return BIG * (depth);
	}
	if (ennemyH.fiveLine || ennemyH.totalCaptured >= 10) {
		return -BIG * (depth);
	}
	return (myH.score + CAPTURE_BONUS(myH.totalCaptured) - (ennemyH.score + CAPTURE_BONUS(ennemyH.totalCaptured)) );
}

bool MinMaxDynamicPlayer::canAvoidDefeat_e(HeuristicBoard &myH, HeuristicBoard &ennemyH) {
	if (!myH.fiveLine || ennemyH.totalCaptured >= 10)
		return true;
	return false;
}
