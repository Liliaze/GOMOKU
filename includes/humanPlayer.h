#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "utils.h"

#include "player.h"

class HumanPlayer : public virtual Player
{
	private:
	
	public:

		HumanPlayer();
		~HumanPlayer();

		virtual void play(Rules &rules, Interface &i);
		virtual void playSimpleSwap(Gomoku *gomoku, Rules &rules, Interface &i);
		virtual bool playSwapTwoStep1(Gomoku *gomoku, Rules &rules, Interface &i);
		virtual void playSwapTwoStep2(Gomoku *gomoku, Rules &rules, Interface &i);
};

#endif