#ifndef MINMAXDYNAMICPLAYER_H
#define MINMAXDYNAMICPLAYER_H

#include "player.h"

#define NUM_THREADS 4
#define STARTING_ROUND 3
#define STARTING_DEPTH 7

class Choice
{
	public:
		unsigned char x;
		unsigned char y;
		Gomoku gomoku;
		HeuristicBoard myHeuristic;
		HeuristicBoard ennemyHeuristic;
		std::vector<std::pair<unsigned char, unsigned char>> captured;
	
		Choice(unsigned char x, unsigned char y, HeuristicBoard *myHeuristic, HeuristicBoard *ennemyHeuristic, Player *player, Gomoku *gomoku, bool ennemy = false);
		~Choice();
};

class MinMaxDynamicPlayer : public virtual Player
{
	static std::mutex mutex;

	private:

		unsigned char depthWidths[11];
		unsigned long complexity;
		int usedDepth;
		int minMaxDepth;

		long long min(Gomoku *gomoku, int depth, long long minBestOption, long long maxBestOption, Rules &rules, HeuristicBoard myH, HeuristicBoard ennemyH);
		long long max(Gomoku *gomoku, int depth, long long minBestOption, long long maxBestOption, Rules &rules, HeuristicBoard myH, HeuristicBoard ennemyH);
		bool win(int &rx, int &ry, Rules &rules);
		void heuristicMap(std::multimap<long long, std::unique_ptr<Choice>> &result, Gomoku *gomoku, Rules &rules, bool last, int depht, HeuristicBoard &myOrigin, HeuristicBoard &ennemyOrigin);
		void heuristicMap_e(std::multimap<long long, std::unique_ptr<Choice>> &result, Gomoku *gomoku, Rules &rules, bool last, int depht, HeuristicBoard &myOrigin, HeuristicBoard &ennemyOrigin);
		bool canAvoidDefeat(HeuristicBoard &myH, HeuristicBoard &ennemyH);
		long long heuristic(HeuristicBoard &heuristic, HeuristicBoard &ennemyHeuristic, bool last, int depth);
		bool canAvoidDefeat_e(HeuristicBoard &myH, HeuristicBoard &ennemyH);
		long long heuristic_e(HeuristicBoard &heuristic, HeuristicBoard &ennemyHeuristic, bool last, int depth);
		void startThread(int &rx, int &ry, long long &option, long long &best, long long &maxBestOption, std::multimap<long long, std::unique_ptr<Choice>> &choices, int threadIndex);
		long long simulate(int &x, int &y, Rules &rules, Interface &i);
		void undoSimulation(int &x, int &y);


	public:

		MinMaxDynamicPlayer(std::array<unsigned char, 11> dw);
		~MinMaxDynamicPlayer();
		virtual void play(Rules &rules, Interface &interfacce);
		long long startMinMax(int &rx, int &ry, Rules &rules, Interface &i);
		void playToHelp(Rules &rules, Interface &interface);
		virtual void playSimpleSwap(Gomoku *gomoku, Rules &rules, Interface &i);
		virtual void playSwapTwoStep1(Gomoku *gomoku, Rules &rules, Interface &i);
		virtual void playSwapTwoStep2(Gomoku *gomoku, Rules &rules, Interface &i);
};

#endif
