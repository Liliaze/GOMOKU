#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"
#include "gomoku.h"
#include "heuristicBoard.h"

class Interface;
class Rules;

class Player
{
	friend class Gomoku;

	protected:

		HeuristicBoard myHeuristic;
		HeuristicBoard ennemyHeuristic;

		int nbCapture;
		Gomoku *gomoku;
		Player *enemy;
		Stone stoneColor;
		sf::Sprite	*stoneSprite;
		bool	played;
		sf::Vector2<int> coordPlayed; // MEMO : x et y ==> coordonnée du board dans Gomoku

		void putStone(int x, int y);
		void unPutStone(int x, int y);
		sf::Vector2<int> canteen[10];
	
	public:

		Player();
		virtual ~Player();
		bool	human;

		virtual void play(Rules &rules, Interface &i )= 0;
		virtual void playForHelp(Rules &rules, Interface &i) {(void)rules;(void)i;return;}
		virtual void playSimpleSwap(Gomoku *gomoku, Rules &rules, Interface &i)=0;
		virtual void playSwapTwoStep1(Gomoku *gomoku, Rules &rules, Interface &i)=0;
		virtual void playSwapTwoStep2(Gomoku *gomoku, Rules &rules, Interface &i)=0;
		virtual void observe(Rules &rules, int x, int y, std::vector<std::pair<unsigned char, unsigned char>> &captured);
		virtual void observeMyCapture(std::vector<std::pair<unsigned char, unsigned char>> &captured);
		inline Stone getColor() {return stoneColor;}
		inline bool	getHuman() {return human;}
		inline HeuristicBoard *getMyHeuristic() {return &myHeuristic;}
		inline HeuristicBoard *getEnnemyHeuristic() {return &ennemyHeuristic;}
		inline Player *getEnemy() {return enemy;}
		inline sf::Sprite *getSpriteStone() {return stoneSprite;}
		inline int getNbCapture() {return nbCapture;}
		inline sf::Vector2<int> getCoordPlayed(void) {return (coordPlayed);}
		inline sf::Vector2<int> getCoordCanteen(int index) {return (canteen[index]);}
		inline void setGomoku(Gomoku *gomoku) {this->gomoku = gomoku; myHeuristic.gomoku = gomoku; ennemyHeuristic.gomoku = gomoku;}
		inline void	setEnemy(Player *en) {this->enemy = en;}
		inline void	setMyHeuristic(HeuristicBoard h) {this->myHeuristic = h;}
		inline void	setEnnemyHeuristic(HeuristicBoard h) {this->ennemyHeuristic = h;}
		inline void	setNbCapture(int nb) {this->nbCapture = nb;}
		inline void setCoordPlayed(int x, int y) {coordPlayed.x = x; coordPlayed.y = y;}
		inline void	setSpriteStone(sf::Sprite *stone) {this->stoneSprite = stone;}
		inline void	setPlayed(bool state) {played = state;}
		inline void	setCanteen(sf::Vector2<int> toCopy[10]) {int i = 0; while(i<10){canteen[i] = toCopy[i];i++;}}
		inline virtual void setColor(Stone color) {this->stoneColor = color; myHeuristic.stone = color; ennemyHeuristic.stone = (color == WHITE ? BLACK : WHITE);}
};


#endif
