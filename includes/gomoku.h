#ifndef GOMOKU_H
#define GOMOKU_H

#include "utils.h"
#include "colors.h"
#include "boardIterator.h"
class Interface;
class Player;
class MinMaxDynamicPlayer;
class Rules;

#define GW 19
#define GH 19
#define FOCUS 2

typedef enum Stone {
	BLACK = 1, WHITE = 2, FORBIDDEN = 3, FREE = 0, OUT_LIMIT = -1
} Stone;

class Gomoku
{
	private:
		
		bool clone;
		Player	*whitePlayer;
		Player	*blackPlayer;
		Player	*currentPlayer;
		Rules	*rules;
		Interface &interface;
		Stone	board[GW][GH];
		int		nbEatenStone;
		bool focus[GW][GH];

		bool	checkLine(Stone color, int x, int y);
		bool	leftDiagonal(Stone color, int x, int y);
		bool	rightDiagonal(Stone color, int x, int y);
		bool	verticalLine(Stone color, int x, int y);
		bool	horizontalLine(Stone color, int x, int y);
		
		void	checkLeft(Player &current, unsigned char x, unsigned char y, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured);
		void	checkRight(Player &current, unsigned char x, unsigned char y, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured);
		void	checkUp(Player &current, unsigned char x, unsigned char y, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured);
		void	checkDown(Player &current, unsigned char x, unsigned char y, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured);
		void	checkUpLeft(Player &current, unsigned char x, unsigned char y, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured);
		void	checkDownLeft(Player &current, unsigned char x, unsigned char y, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured);
		void	checkUpRight(Player &current, unsigned char x, unsigned char y, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured);
		void	checkDownRight(Player &current, unsigned char x, unsigned char y, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured);
		bool	checkBetween(Stone colorEnemy, unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
		void	capture(Player &current, sf::Sprite *spriteEnemy, int x1, int y1);
		void	captureAll(Player &current, sf::Sprite *spriteEnemy, std::vector<std::pair<unsigned char, unsigned char>> &captured);
		void	drawStone();
		void	initGomoku();
	
	public:

		Gomoku(Interface& interface);
		Gomoku(Gomoku* copyFrom);
		~Gomoku();

		inline Player	&getWhitePlayer() {return *whitePlayer;}
		inline Player	&getBlackPlayer() {return *blackPlayer;}
		inline Rules	&getRules() {return *rules;}
		inline Player	*aWhitePlayer() {return whitePlayer;}
		inline Player	*aBlackPlayer() {return blackPlayer;}
		inline Rules	*aRules() {return rules;}
		inline void		setWhitePlayer(Player *p) {whitePlayer = p;}
		inline void		setBlackPlayer(Player *p) {blackPlayer = p;}
		inline void		setCurrentPlayer(Player *p) {currentPlayer = p;}
		inline void		setRules(Rules *r) {rules = r; updateStartingFocus();}
		void			updateStartingFocus(void);
		void			updatePlayerInWhite(Player &p);
		void			updatePlayerInBlack(Player &p);
		void			updatePlayer(void);
		void			swapPlayer(void);
		void			updateRules(void);
		inline Player	*getCurrentPlayer() {return currentPlayer;}
		inline Stone**	getBoard() {return (Stone**)board;}
		inline bool**	getFocus() {return (bool**)focus;}
		inline Interface &getInterface() {return interface;}
		inline int		getNbEatenStone() {return nbEatenStone;}
		Stone			getStone(int x, int y);
		inline void		setStone(Stone stone, int x, int y) {board[x][y] = stone;}
		inline bool		isFocus(int x, int y) {return focus[x][y];}
		void			updateFocus(int x, int j);

		bool fiveStoneLine(Stone color, int &x, int &y);
		void checkCapture(Player &current, unsigned char x, unsigned char y, Player &enemy, std::vector<std::pair<unsigned char, unsigned char>> &captured);

		void printBoard(int lastX = -1, int lastY = -1);
		void printStone(int i, int j, int lastX = -1, int lastY = -1);
		void start();
		void end();
};


#endif
