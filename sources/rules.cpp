#include "rules.h"
#include "minMaxDynamicPlayer.h"

Rules::Rules() : turnCounter(0)
{
	initStartingFocus();
	twoMoreStone = false;
}

void Rules::initStartingFocus() {
	char schema[GW] = {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0};
	for (int i = 0; i < GW; i++) {
		for (int j = 0; j < GH; j++) {
			if (schema[i] == 1 && schema[j] == 1)
				startingFocus[i][j] = 1;
			else
				startingFocus[i][j] = 0;
		}
	}
}

Rules::~Rules() {
}

bool Rules::canAvoidDefeat(Player *player, Player *ennemy) {
	for (unsigned char j = 0; j < GH; j++) {
		for (unsigned char i = 0; i < GW; i++) {
			if (canPutStone(*player, i, j)) {
					Choice choice(i, j, player->getMyHeuristic(), player->getEnnemyHeuristic(), player, gomoku, false);
					gomoku->checkCapture(*player, i, j, *ennemy, choice.captured);
					for (auto it = choice.captured.begin(); it != choice.captured.end(); ++it) {
						choice.ennemyHeuristic.beCaptured(it->first, it->second);
						choice.myHeuristic.capture(it->first, it->second);
					}
					if (!choice.ennemyHeuristic.fiveLine || choice.myHeuristic.totalCaptured >= NB_CAPTURE_TO_WIN) {
						DEBUG << BLUE << "avoid with " << (int)i << "/" << (int)j << DEFAULT_COLOR << "\n";
						return true;
					}
			}
		}
	}
	return false;
}

bool Rules::win(Player &player, bool avoidable) {
	int x = 0, y = 0;
	if (gomoku->fiveStoneLine(player.getColor(), x, y) &&
		!(avoidable && canAvoidDefeat(player.getEnemy(), &player)) )
		return true;
	return false;
}

bool Rules::equality(Player &nextToPlay) {
	for (int i = 0; i < GW; i++) {
		for (int j = 0; j < GH; j++) {
			if (canPutStone(nextToPlay, i, j) == true)
				return false;
		}
	}
	return true;
}

End Rules::playerWon(Player &player) {
	if (player.getColor() == WHITE) {
		DEBUG << LIGHT_GREY << HLL_RED << "White player win" << DEFAULT_COLOR << "\n";
		return WHITE_WIN;
	}
	else {
		DEBUG << DARK_BLACK << HLIGHT_RED << "Black player win" << DEFAULT_COLOR << "\n";
		return BLACK_WIN;
	}
}

End Rules::checkEnd(Player &nextToPlay) {
	Player *lastPlayer = nextToPlay.getEnemy();

	if (lastPlayer->getNbCapture() >= (int)NB_CAPTURE_TO_WIN)
		return playerWon(*lastPlayer);

	if (win(nextToPlay, false))
		return playerWon(nextToPlay);

	if (win(*lastPlayer, true))
		return playerWon(*lastPlayer);

	else if (equality(nextToPlay)) {
		DEBUG << DARK_BLUE << HLIGHT_BLUE << "no winner" << DEFAULT_COLOR << "\n";
		return EQUALITY;
	}
	return NO_END;
}

bool Rules::canPutStone(Player &player, int x, int y) {
	return canPutStone(player.getMyHeuristic(), x, y);
}
