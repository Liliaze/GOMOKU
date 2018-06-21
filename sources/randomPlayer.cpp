#include "randomPlayer.h"
#include "rules.h"
#include "interface.h"

RandomPlayer::RandomPlayer() : Player()
{
}

RandomPlayer::~RandomPlayer()
{
}

int increment(int n, int max) {
	n++;
	if (n == max)
		n = 0;
	return n;
}

void RandomPlayer::play(Rules &rules, Interface &i) {
	int x = rand() % GW;
	int y = rand() % GH;
	int ox = x;
	int oy = y;
	(void)i;
	do {
		y = oy;
		do {
			if (rules.canPutStone(*this, x, y)) {
				putStone(x, y);
				setCoordPlayed(x, y);
			}
			y = increment(y, GH);
		} while (y != oy);
		x = increment(x, GW);
	} while (x != ox);
	failure("impossible to put stone on board");
	played = true;
}
