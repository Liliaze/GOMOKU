#ifndef RULES_H
#define RULES_H

#include "gomoku.h"
#include "player.h"

typedef enum End {
	BLACK_WIN = 1, WHITE_WIN = 2, EQUALITY = 3, NO_END = 0
} End;

class Rules
{
	friend class Gomoku;

	protected:

		unsigned int turnCounter;
		Gomoku *gomoku;
		
		inline void setGomoku(Gomoku *gomoku) {this->gomoku = gomoku;}
		bool win(Player &player);
		bool equality(Player &nextToPlay);
		void eat(Player &player);
	
	public:

		Rules();
		~Rules();

		virtual bool canPutStone(Player &player, int x, int y) = 0;

		End checkEnd(Player &nextToPlay);
		void checkEat();
};


#endif

