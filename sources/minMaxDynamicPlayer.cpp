#include "minMaxDynamicPlayer.h"
#include "rules.h"

const unsigned char MinMaxDynamicPlayer::depthWidths[10] = {16, 12, 8, 8, 4, 4, 4, 4, 4, 4};
std::mutex MinMaxDynamicPlayer::mutex;

Choice::Choice(unsigned char x, unsigned char y, HeuristicBoard *myHeuristic, HeuristicBoard *ennemyHeuristic, bool ennemy) : x(x), y(y), myHeuristic(myHeuristic), ennemyHeuristic(ennemyHeuristic)
{
	(void)ennemy;
	this->myHeuristic.clear(x, y);
	this->ennemyHeuristic.put(x, y);
}

Choice::Choice(unsigned char x, unsigned char y, HeuristicBoard *myHeuristic, HeuristicBoard *ennemyHeuristic) : x(x), y(y), myHeuristic(myHeuristic), ennemyHeuristic(ennemyHeuristic)
{
	this->myHeuristic.put(x, y);
	this->ennemyHeuristic.clear(x, y);
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

void MinMaxDynamicPlayer::heuristicMap(std::multimap<long long, Choice> &result, Gomoku *gomoku, Rules &rules, Player *player, bool last, int depth, HeuristicBoard &myOrigin, HeuristicBoard &ennemyOrigin, bool focus) {
	for (unsigned char j = 0; j < GH; j++) {
		for (unsigned char i = 0; i < GW; i++) {
			if ((!focus || gomoku->isFocus(i, j)) && rules.canPutStone(*player, i, j)) {
				Choice choice(i, j, &myOrigin, &ennemyOrigin);
				gomoku->checkCapture(*this, i, j, *enemy, choice.captured);
				for (auto it = choice.captured.begin(); it != choice.captured.end(); ++it) {
					choice.ennemyHeuristic.beCaptured(it->first, it->second);
					choice.myHeuristic.capture(it->first, it->second);
				}
				result.insert(std::pair<long long, Choice>(-heuristic(choice.myHeuristic, choice.ennemyHeuristic, last, depth), choice));
			}
		}
	}
}

void MinMaxDynamicPlayer::heuristicMap_e(std::multimap<long long, Choice> &result, Gomoku *gomoku, Rules &rules, Player *player, bool last, int depth, HeuristicBoard &myOrigin, HeuristicBoard &ennemyOrigin, bool focus) {
	for (unsigned char j = 0; j < GH; j++) {
		for (unsigned char i = 0; i < GW; i++) {
			if ((!focus || gomoku->isFocus(i, j)) && rules.canPutStone(*player, i, j)) {
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

void MinMaxDynamicPlayer::startThread(int &rx, int &ry, long long &option, long long &best, long long &maxBestOption, std::multimap<long long, Choice> &choices, int threadIndex) {

	if ((size_t)threadIndex >= choices.size())
		return ;

	long long tmp;
	unsigned char i = threadIndex;

//	mutex.lock();
//	DEBUG << GREEN << "START THREAD " << threadIndex << DEFAULT_COLOR << "\n";
//	DEBUG << DARK_GREEN << "rule adress : " << (void*)copyRules << DEFAULT_COLOR << "\n";
//	DEBUG << DARK_GREEN << "gomoku rule adress : " << (void*)&(gomokuCopy.getRules()) << DEFAULT_COLOR << "\n";
//	mutex.unlock();

	Rules *copyRules = gomoku->getRules().copy();
	auto it = std::next(choices.begin(), threadIndex);
	while (1) {

	Gomoku gomokuCopy = Gomoku(this->gomoku, *copyRules);
	copyRules->setGomoku(&gomokuCopy);

		it->second.myHeuristic.setGomoku(&gomokuCopy);
		it->second.ennemyHeuristic.setGomoku(&gomokuCopy);

		if (i > depthWidths[0]) {
			//mutex.lock();
			//DEBUG << DARK_YELLOW << "break\n" << DEFAULT_COLOR;
			//mutex.unlock();
			break;
		}

		gomokuCopy.setStone(getColor(), it->second.x, it->second.y);
		for (auto it2 = it->second.captured.begin(); it2 != it->second.captured.end(); ++it2) {
			gomokuCopy.setStone(FREE, it2->first, it2->second);
		}
		complexity++;

		tmp = min(&gomokuCopy, usedDepth, MAX_LONG, maxBestOption, *copyRules, it->second.myHeuristic, it->second.ennemyHeuristic);

	//	gomokuCopy.setStone(FREE, it->second.x, it->second.y);
	//	for (auto it2 = it->second.captured.begin(); it2 != it->second.captured.end(); ++it2) {
	//		gomokuCopy.setStone(enemy->getColor(), it2->first, it2->second);
	//	}

		mutex.lock();
		DEBUG << YELLOW << "try : " << (int)it->second.x << "/" << (int)it->second.y << DEFAULT_COLOR << " - " << (int)i << "\n";
		if (tmp >= BIG) {
			DEBUG << GREEN << "I CAN WIN : " <<  tmp << DEFAULT_COLOR << "\n";
		} else if (tmp <= -BIG) {
			DEBUG << RED << "I WILL LOOSE : " << tmp << DEFAULT_COLOR << "\n";
		} else {
			DEBUG << DARK_YELLOW << "score : " << tmp << DEFAULT_COLOR << "\n";
		}

		option = tmp;
		if (option > best) {
			best = option;
			rx = it->second.x;
			ry = it->second.y;
		}
		if (option > maxBestOption) {
			maxBestOption = option;
		}
		mutex.unlock();

		i += NUM_THREADS;
		if ((size_t)i >= choices.size()) {
			break ;
		} else {
			it = std::next(it, NUM_THREADS);
		}
	}
	delete copyRules;
}

void MinMaxDynamicPlayer::startMinMax(int &rx, int &ry, Rules &rules)
{
	long long option;
	long long best = MIN_LONG;
	long long maxBestOption = MIN_LONG;

	if (rules.getTurnCounter() < STARTING_ROUND && minMaxDepth > STARTING_DEPTH)
		usedDepth = STARTING_DEPTH;
	else
		usedDepth = minMaxDepth;

	complexity = 0;
	std::multimap<long long, Choice> choices;
	heuristicMap(choices, gomoku, rules, this, ennemyHeuristic.fiveLine, usedDepth, myHeuristic, ennemyHeuristic, true);
	rx = choices.begin()->second.x;
	ry = choices.begin()->second.y;
	std::thread threads[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS; i++) {
		threads[i] = std::thread(&MinMaxDynamicPlayer::startThread, this, std::ref(rx), std::ref(ry), std::ref(option), std::ref(best), std::ref(maxBestOption), std::ref(choices), i);
	}
	for (auto& th : threads) th.join();

	DEBUG << "complexity : " << complexity << "\n";
}

long long MinMaxDynamicPlayer::min(Gomoku *gomokuOrigin, int depth, long long minBestOption, long long maxBestOption, Rules &rules, HeuristicBoard myH, HeuristicBoard ennemyH)
{
	if (depth <= 0 || ennemyH.fiveLine || myH.totalCaptured >= 10) {
		return heuristic(myH, ennemyH, ennemyH.fiveLine, depth+1);
	}
	long long option;
	long long best = MAX_LONG;
	std::multimap<long long, Choice> choices;
	heuristicMap_e(choices, gomokuOrigin, rules, enemy, myH.fiveLine, depth, myH, ennemyH);
	unsigned char i = 0;
//	if (myH.score >= L4) {
//		i = depthWidths[usedDepth - depth];
//	}
	for (auto it = choices.begin(); it != choices.end(); ++it) {

		if (i > depthWidths[usedDepth - depth]) {
			break;
		}

		Gomoku gomoku = Gomoku(gomokuOrigin, rules);
		rules.setGomoku(&gomoku);
		it->second.myHeuristic.setGomoku(&gomoku);
		it->second.ennemyHeuristic.setGomoku(&gomoku);

		gomoku.setStone(enemy->getColor(), it->second.x, it->second.y);
		for (auto it2 = it->second.captured.begin(); it2 != it->second.captured.end(); ++it2) {
//			if (gomoku->getStone(it2->first, it2->second) != getColor()) DEBUG << RED << "WRONG CAPTURE !\n" << DEFAULT_COLOR;
			gomoku.setStone(FREE, it2->first, it2->second);
		}
		complexity++;
		option = max(&gomoku, depth-1, minBestOption, maxBestOption, rules, it->second.myHeuristic, it->second.ennemyHeuristic);

	//	gomoku->setStone(FREE, it->second.x, it->second.y);
	//	for (auto it2 = it->second.captured.begin(); it2 != it->second.captured.end(); ++it2) {
	//		gomoku->setStone(getColor(), it2->first, it2->second);
	//	}
	
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

long long MinMaxDynamicPlayer::max(Gomoku *gomokuOrigin, int depth, long long minBestOption, long long maxBestOption, Rules &rules, HeuristicBoard myH, HeuristicBoard ennemyH)
{
	if (depth <= 0 || myH.fiveLine || ennemyH.totalCaptured >= 10) {
		return heuristic_e(myH, ennemyH, myH.fiveLine, depth+1);
	}
	long long option;
	long long best = MIN_LONG;
	std::multimap<long long, Choice> choices;
	heuristicMap(choices, gomokuOrigin, rules, this, ennemyH.fiveLine, depth, myH, ennemyH);
	unsigned char i = 0;
//	if (ennemyH.score >= L4) {
//		i = depthWidths[usedDepth - depth];
//	}
	for (auto it = choices.begin(); it != choices.end(); ++it) {
		if (i > depthWidths[usedDepth - depth]) {
			break;
		}

		Gomoku gomoku = Gomoku(gomokuOrigin, rules);
		rules.setGomoku(&gomoku);
		it->second.myHeuristic.setGomoku(&gomoku);
		it->second.ennemyHeuristic.setGomoku(&gomoku);

		gomoku.setStone(getColor(), it->second.x, it->second.y);

		for (auto it2 = it->second.captured.begin(); it2 != it->second.captured.end(); ++it2) {
//			if (gomoku->getStone(it2->first, it2->second) != enemy->getColor()) DEBUG << RED << "WRONG CAPTURE !\n" << DEFAULT_COLOR;
			gomoku.setStone(FREE, it2->first, it2->second);
		}
		complexity++;
		option = min(&gomoku, depth-1, minBestOption, maxBestOption, rules, it->second.myHeuristic, it->second.ennemyHeuristic);

	//	gomoku->setStone(FREE, it->second.x, it->second.y);
	//	for (auto it2 = it->second.captured.begin(); it2 != it->second.captured.end(); ++it2) {
	//		gomoku->setStone(enemy->getColor(), it2->first, it2->second);
	//	}

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
	startMinMax(x, y, rules);
	putStone(x, y);
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
