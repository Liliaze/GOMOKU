#ifndef ASSISTEDHUMANPLAYER_H
# define ASSISTEDHUMANPLAYER_H

#include "player.h"
#include "humanPlayer.h"
#include "minMaxDynamicPlayer.h"
//#include "helper.h"

class AssistedHumanPlayer : public MinMaxDynamicPlayer, public HumanPlayer
{
    public :
        AssistedHumanPlayer(std::array<unsigned char, 11> dw);
        ~AssistedHumanPlayer();

        virtual void play(Rules &rules, Interface &i);
        virtual void playForHelp(Rules &rules, Interface &i);
		virtual void playSimpleSwap(Gomoku *gomoku, Rules &rules, Interface &i);
		virtual void playSwapTwoStep1(Gomoku *gomoku, Rules &rules, Interface &i);
		virtual void playSwapTwoStep2(Gomoku *gomoku, Rules &rules, Interface &i);
};

#endif