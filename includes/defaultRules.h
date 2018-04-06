#ifndef DEFAULTRULES_H
#define DEFAULTRULES_H

#include "rules.h"

class DefaultRules : public Rules
{
	private:

	public:

		DefaultRules();
		~DefaultRules();


		virtual bool canPutStone(HeuristicBoard *h, int x, int y);
		virtual Rules *copy();
};


#endif
