#ifndef NOOBIA_H
#define NOOBIA_H

#include "utils.h"
#include "player.h"

class NoobIA : public Player
{
	private:

	
	public:

		NoobIA();
		~NoobIA();

		virtual void play(Rules &rules, Interface &i);
		virtual void playSimpleSwap(Gomoku *gomoku, Rules &rules, Interface &i);
		virtual void playSwapTwoStep1(Gomoku *gomoku, Rules &rules, Interface &i);
		virtual void playSwapTwoStep2(Gomoku *gomoku, Rules &rules, Interface &i);
};

#endif