#include "assistedHumanPlayer.h"
#include "interface.h"
#include "rules.h"

AssistedHumanPlayer::AssistedHumanPlayer(std::array<unsigned char, 11> dw) : Player(), MinMaxDynamicPlayer(dw), HumanPlayer() {
    if (human) {
        DEBUG << "I'm human and robot, i'm the better of humanity\n";
    }else{
        DEBUG << "erreur constructeur, échec humanisation\n";}
}

AssistedHumanPlayer::~AssistedHumanPlayer() {
    DEBUG << "BYE ASSISTED HUMAN\n";
}

void AssistedHumanPlayer::playForHelp(Rules &rules, Interface &i) {
   DEBUG << "Need help? Wait...\n";
    int x = -1;
	int y = -1;
	startMinMax(x, y, rules, i);
	i.putStoneToHelp(x, y);
   DEBUG << "Try this, the best\n";
	return;
}

void AssistedHumanPlayer::play(Rules &rules, Interface &i) {
   DEBUG << "l'humain doit jouer :\n";
	HumanPlayer::play(rules,i);
   DEBUG << "merci d'avoir joue :\n";
}

void AssistedHumanPlayer::playSimpleSwap(Gomoku *gomoku, Rules &rules, Interface &interface) {
	(void)rules;
	sf::Vector2<int>	tmp(0,0);
	while(1) {
		tmp = interface.checkEventSwap();
		if (interface.onWhiteBoxP2(tmp.x, tmp.y)) {
			break;
		}
		else if (interface.onBlackBoxP2(tmp.x, tmp.y)) {
			gomoku->swapPlayer();
			gomoku->setCurrentPlayer(gomoku->aBlackPlayer());
			break;
		}
	}
}

void AssistedHumanPlayer::playSwapTwoStep1(Gomoku *gomoku, Rules &rules, Interface &interface) {
	(void)rules;
	sf::Vector2<int>	tmp(0,0);
	while(1) {
		tmp = interface.checkEventSwap();
		if (interface.onWhiteBoxP1(tmp.x, tmp.y)) {
			break;
		}
		else if (interface.onBlackBoxP1(tmp.x, tmp.y)) {
			gomoku->swapPlayer();
			gomoku->setCurrentPlayer(gomoku->aBlackPlayer());
			break;
		}
		/*else if (interface.onPlayTwoMore()) {

		}*/
	}
}

void AssistedHumanPlayer::playSwapTwoStep2(Gomoku *gomoku, Rules &rules, Interface &interface) {
	(void)rules;
	sf::Vector2<int>	tmp(0,0);
	while(1) {
		tmp = interface.checkEventSwap();
		if (interface.onWhiteBoxP1(tmp.x, tmp.y)) {
			break;
		}
		else if (interface.onBlackBoxP1(tmp.x, tmp.y)) {
			gomoku->swapPlayer();
			gomoku->setCurrentPlayer(gomoku->aBlackPlayer());
			break;
		}
	}
}
