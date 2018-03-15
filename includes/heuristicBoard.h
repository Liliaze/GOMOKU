#ifndef HEURISTICBOARD_H
#define HEURISTICBOARD_H

#include "gomoku.h"

#define GET_HH(x)  ((x & 0xF000) >> 12)
#define GET_VH(x)  ((x & 0x0F00) >> 8)
#define GET_DLH(x) ((x & 0x00F0) >> 4)
#define GET_DRH(x) (x & 0x000F)

#define SET_HH(x,y)  ((x & 0x0FFF) | (y << 12))
#define SET_VH(x,y)  ((x & 0xF0FF) | (y << 8))
#define SET_DLH(x,y) ((x & 0xFF0F) | (y << 4))
#define SET_DRH(x,y) ((x & 0xFFF0) | (y))

#define GET_THREAT(x,m,s)  ((x & m) >> s)
#define SET_THREAT(x,y,m,s)  ((x & (~m)) | (y << s))
#define HORIZONTAL_SHIFT 12
#define VERTICAL_SHIFT 8
#define DLEFT_SHIFT 4
#define DRIGHT_SHIFT 0
#define HORIZONTAL_MASK 0xF000 
#define VERTICAL_MASK 0x0F00 
#define DLEFT_MASK 0x00F0 
#define DRIGHT_MASK 0x000F 

#define REMOVE_THREAT(x) score -= levels[((x) > 5 ? 5 : (x))]
#define ADD_THREAT(x) score += levels[((x) > 5 ? 5 : (x))]

#define WATCH(x,name) if (x < 0 || x >= GW) DEBUG << RED << "WARNING " << name << " : out of limit -> " << x << "\n" << DEFAULT_COLOR

#define L0 0
#define L1 1
#define L2 (long long)(18*18)
#define L3 (long long)L2*L2
#define L4 (long long)L3*L2
#define HAS_WON (long long)L4*L2
#define WIN_STATE (long long)L4*2
#define BIG (long long)HAS_WON*L2

class HeuristicBoard
{
	friend class Player;
	friend class MinMaxDynamicPlayer;
	//friend class HeuristicBoard;
	static const long long levels[6];

	private:

		unsigned char fiveLine;
		unsigned char totalCaptured;
		short int heuristic[GW][GH];
		Gomoku* gomoku;
		char stone;

		void getAdjacent(char x, char y, char vx, char vy, char &before, char &after, char &beforeWall, char &afterWall);
		int getBestLevel(unsigned char x, unsigned char y);
		void fiveValue(char x, char y, unsigned char &value, unsigned char heuristic);
		void updateThreat(char x, char y, char vx, char vy, char shift, short int mask);
		void removeEnnemyThreat(char x, char y, char vx, char vy, char shift, short int mask);
		void removeThreat(char x, char y, char vx, char vy, char shift, short int mask);
		void clearOne(char x, char y, char vx, char vy, char shift, short int mask);
		void searchFreeThree(char x, char y, char vx, char vy, char shift, short int mask, unsigned char &result);
	
	public:
		long long score;

		HeuristicBoard();
		HeuristicBoard(HeuristicBoard *copyFrom);
		~HeuristicBoard();
		HeuristicBoard &put(unsigned char x, unsigned char y, bool prediction = true);
		HeuristicBoard &clear(unsigned char x, unsigned char y);
		void beCaptured(unsigned char x, unsigned char y);
		void capture(unsigned char x, unsigned char y);
		void print(int lastX = -1, int lastY = -1);
		bool checkDoubleFreeThree(unsigned char x, unsigned char y);

};

#endif
