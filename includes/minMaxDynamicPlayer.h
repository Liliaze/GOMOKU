#ifndef MINMAXDYNAMICPLAYER_H
#define MINMAXDYNAMICPLAYER_H

#include "player.h"

#define NUM_THREADS 1
#define STARTING_ROUND 4
#define STARTING_DEPTH 6

class Choice
{
	public:
		unsigned char x;
		unsigned char y;
		HeuristicBoard myHeuristic;
		HeuristicBoard ennemyHeuristic;
		std::vector<std::pair<unsigned char, unsigned char>> captured;
	
		Choice(unsigned char x, unsigned char y, HeuristicBoard *myHeuristic, HeuristicBoard *ennemyHeuristic, bool ennemy);
		Choice(unsigned char x, unsigned char y, HeuristicBoard *myHeuristic, HeuristicBoard *ennemyHeuristic);
		Choice();
		~Choice();
};

class MinMaxDynamicPlayer : public Player
{
	static const unsigned char depthWidths[10];
	static std::mutex mutex;

	private:

		unsigned long complexity;
		int usedDepth;
		int minMaxDepth;

		long long min(Gomoku *gomoku, int depth, long long minBestOption, long long maxBestOption, Rules &rules, HeuristicBoard myH, HeuristicBoard ennemyH);
		long long max(Gomoku *gomoku, int depth, long long minBestOption, long long maxBestOption, Rules &rules, HeuristicBoard myH, HeuristicBoard ennemyH);
		void startMinMax(int &rx, int &ry, Rules &rules);
		bool win(int &rx, int &ry, Rules &rules);
		void heuristicMap(std::multimap<long long, Choice> &result,  Gomoku *gomoku, Rules &rules, Player *player, bool last, int depht, HeuristicBoard &myOrigin, HeuristicBoard &ennemyOrigin, bool focus = false);
		void heuristicMap_e(std::multimap<long long, Choice> &result,  Gomoku *gomoku, Rules &rules, Player *player, bool last, int depht, HeuristicBoard &myOrigin, HeuristicBoard &ennemyOrigin, bool focus = false);
		bool canAvoidDefeat(HeuristicBoard &myH, HeuristicBoard &ennemyH);
		long long heuristic(HeuristicBoard &heuristic, HeuristicBoard &ennemyHeuristic, bool last, int depth);
		bool canAvoidDefeat_e(HeuristicBoard &myH, HeuristicBoard &ennemyH);
		long long heuristic_e(HeuristicBoard &heuristic, HeuristicBoard &ennemyHeuristic, bool last, int depth);
		void startThread(int &rx, int &ry, long long &option, long long &best, long long &maxBestOption, std::multimap<long long, Choice> &choices, int threadIndex);


	public:

		MinMaxDynamicPlayer(int d);
		~MinMaxDynamicPlayer();
		virtual void play(Rules &rules, Interface &interfacce);
};

#endif
