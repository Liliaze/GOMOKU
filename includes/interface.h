#ifndef INTERFACE_H
#define INTERFACE_H

#include "gomoku.h"
#include "player.h"
#include "menu.h"

# define WIDTH	1200
# define HEIGHT	900
# define BOARD_LEFT	240
# define BOARD_RIGHT	960
# define BOARD_UP	90
# define BOARD_DOWN	810
# define MARGE	10
# define YES_LEFT	150
# define YES_RIGHT	500
# define YES_UP	450
# define YES_DOWN	555
# define NO_LEFT	650
# define NO_RIGHT	995
# define NO_UP	450
# define NO_DOWN	555
# define SWAPBOX_UP	700
# define SWAPBOX_DOWN	745
# define BLACKBOXP1_LEFT	5
# define BLACKBOXP1_RIGHT	100
# define WHITEBOXP1_LEFT	110
# define WHITEBOXP1_RIGHT	200
# define BLACKBOXP2_LEFT	991
# define BLACKBOXP2_RIGHT	1090
# define WHITEBOXP2_LEFT	1100
# define WHITEBOXP2_RIGHT	1195
# define BLACKCANTEENX 240
# define BLACKCANTEENY 40
# define WHITECANTEENX 960
# define WHITECANTEENY 860
# define HELPERX	35
# define HELPERY	35
# define CHRONOX		1025
# define CHRONOY		40
# define TIMEX			1086
# define TIMEY			142
# define BTIMERX		25
# define BTIMERY		285
# define WTIMERX		1005
# define WTIMERY		285
# define BTIMEX			107
# define BTIMEY			590
# define WTIMEX			1089
# define WTIMEY			590
# define NBTURNX		1089
# define NBTURNY		216
# define BRULESX		107
# define BRULESY		640
# define WRULESX		1089
# define WRULESY		640
# define CENTER_MAX_DISTANCE 20

typedef enum State {
	WELCOME = 1, MENU = 2, GAME = 3, WHITEWIN = 4, BLACKWIN = 5, EQUAL = 6, AGAIN = 7, PAUSE = 8, GOODBYE = 0
} State;

class Interface {

	friend class Gomoku;
	friend class Menu;

	public:
		Interface();
		~Interface();

		void	drawGame(void);
		void	update(void);
		void	putStone(sf::Sprite, int, int);
		void	putStoneToHelp(int gomokuX, int gomokuY);
		void	printCoordBoard(void);
		void	cleanInterface(void);
		void	checkClickLeft(Player *current, int x, int y);
		void	setStoneOnClick(Player &player, int x, int y);
		void	checkEvent(Player *current);
		sf::Vector2<int>	checkEventSwap();
		
		void	setState(State newState);
		void	capture(Player &current, sf::Sprite *spriteEnemy, int x, int y);

		inline	State				&getState(void) {return (state);}
		inline	sf::RenderWindow	&getWindow(void) {return this->_window;}
		inline	sf::Event			&getEvent(void) {return this->_event;}
		inline	sf::Vector2<int>		&getCoordBoard(int x, int y){return (coordBoard[x][y]);}	
		inline	float				getTimeOfGameInSeconds(void) {return this->_timeOfGame.asSeconds();}
		inline	void				setTimeOfGame(sf::Time newTime) {this->_timeOfGame = newTime;}
		inline	float				getTimeToPlayInSeconds(void) {return this->_timeToPlay.asSeconds();}
		inline	sf::RectangleShape	*getPtrFiveByFiveRect(void) {return &(this->fiveByFiveRect);}
		inline	sf::RectangleShape	*getPtrSevenBySevenRect(void) {return &(this->sevenBySevenRect);}
		inline	sf::Sprite			*getSpriteWhiteStone(void) {return &(this->_whiteStone);}
		inline	sf::Sprite			*getSpriteBlackStone(void) {return &(this->_blackStone);}
		inline	sf::Sprite			&getSpriteBlackBox(void) {return this->_blackBoxSprite;}
		inline	sf::Sprite			&getSpriteWhiteBox(void) {return this->_whiteBoxSprite;}
		inline	sf::Text			*getPtrRulesText(void) {return &(this->rulesText);}
		inline	void				setTimeToPlay(sf::Time newTime) {this->_timeToPlay = newTime;}
		inline	std::list<sf::Shape*>	&getShapeList() {return this->_allShape;}
		inline	std::list<sf::Sprite>	&getSpriteList() {return this->_allSprite;}
		inline void					setRulesText(sf::String text, int screenX, int screenY) {this->rulesText.setString(text);this->rulesText.setPosition(screenX, screenY);}
		bool	onBoard(int x, int y);
		bool	onAgainYes(int x, int y);
		bool	onAgainNo(int x, int y);
		bool	onBlackBoxP1(int x, int y);
		bool	onWhiteBoxP1(int x, int y);
		bool	onBlackBoxP2(int x, int y);
		bool	onWhiteBoxP2(int x, int y);
		bool	onVisualAid(int x, int y);
		sf::Vector2<int> whiteCanteen[10];
		sf::Vector2<int> blackCanteen[10];

	protected:
		Menu				menu;
		Gomoku 				*gomoku;
		
		State				state;
		sf::RenderWindow	_window;
		sf::Event			_event;
		sf::Clock			_clockTurn;
		sf::Clock			_clockOfGame;
		sf::Time			_timeToPlay;
		sf::Time			_timeOfGame;
		float				previousTime;
		std::list<sf::Sprite>			_allSprite;
		std::list<sf::Text*>			_allText;
		std::list<sf::Sprite>			_allHelpSprite;
		std::list<sf::Shape*>			_allShape;
		sf::Vector2<int>	coordBoard[GW][GH];
		sf::Vector2<int>	turnCoordInterfaceInGomokuBoardIndex(int screenX, int screenY);
		
		sf::Sprite			_whiteStone;
		sf::Sprite			_blackStone;
		
		inline void setGomoku(Gomoku *gomoku) {this->gomoku = gomoku;}
		//std::map<std::string, bool> _screenStatus;

	private:
		void	welcomeScreen(void);
		void	menuScreen(void);
		void	whiteWinScreen(void);
		void	blackWinScreen(void);
		void	equalScreen(void);
		void	againScreen(void);
		void	gameScreen(void);
		void	goodByeScreen(void);
		void	startPauseScreen(void);
		void	stopPauseScreen(void);
		void	captureZone(void);
		void	timer(void);
		void	loadTexture(void);
		void	loadSprite(void);
		void	loadSoundBuffer(void);
		void	loadSoundAndOpenMusic(void);
		void	loadText(void);
		void	loadShape(void);
		void	setText(sf::Text *text, sf::Font &font, int size, sf::Color color, int posX, int posY, sf::String str);
		void	makeSprite(sf::Sprite &s, sf::Texture &t, float sy, float sx, int px, int py);
		void	makeRectangleShape(sf::RectangleShape *shape, int sizeX, int sizeY, int thickness, sf::Color inside, sf::Color outside, int posX, int posY);
		void	initCoordBoard(void);
		void	initCoordCanteen(void);
		void	putPreviewStone(Player &current, int mouseX, int mouseY);
		void	unputPreviewStone(int mouseX, int mouseY);
		void	removeStone(int i, int j);
		void	removeText(sf::Vector2<int> coordOfText);
		void	updateTimerOfGame(void);
		void	updateTimerToPlay(void);
		void	updateNbOfTurn(void);
		void	updateVisualAid(void);
		void	updateHelperToPlay(void);
		void	updateRulesText(void);
		void	putHelpSprite(sf::Sprite sprite, int x, int y);
		//to previzualize stone
		bool				previewStone;
		bool				viewWinner;
		bool				visualAid;
		sf::Vector2<int>	coordPreviewStone;
		sf::Sprite			previewStoneFree;
		sf::Sprite			previewStoneForbidden;

		sf::Sprite			_backgroundSprite;
		sf::Sprite			_boardGameSprite;
		sf::Sprite			_goodByeSprite;
		sf::Sprite			_helloSprite;
		sf::Sprite			_againSprite;
		sf::Sprite			_blackWinSprite;
		sf::Sprite			_whiteWinSprite;
		sf::Sprite			_equalitySprite;
		sf::Sprite			_boxSprite;
		sf::Sprite			_boxSelectSprite;
		sf::Sprite			_againYesSprite;
		sf::Sprite			_againNoSprite;
		sf::Sprite			_blackBoxSprite;
		sf::Sprite			_whiteBoxSprite;
		sf::Sprite			_help1Sprite;
		sf::Sprite			_help2Sprite;
		sf::Sprite			_help3Sprite;
		sf::Sprite			_help4Sprite;
		sf::Sprite			_help5Sprite;
		sf::Sprite			_help5PlusSprite;
		sf::Sprite			_chronoSprite;
		sf::Sprite			_blackTimerSprite;
		sf::Sprite			_whiteTimerSprite;
		sf::Sprite			_bestSprite;

		sf::Texture			_stoneWhiteTexture;
		sf::Texture			_stoneBlackTexture;
		sf::Texture			_backgroundTexture;
		sf::Texture			_boardGameTexture;
		sf::Texture			_goodByeTexture;
		sf::Texture			_helloTexture;
		sf::Texture			_againTexture;
		sf::Texture			_whiteWinTexture;
		sf::Texture			_blackWinTexture;
		sf::Texture			_equalityTexture;
		sf::Texture			_againYesTexture;
		sf::Texture			_againNoTexture;
		sf::Texture			_blackBoxTexture;
		sf::Texture			_whiteBoxTexture;
		sf::Texture			_boxTexture;
		sf::Texture			_help1Texture;
		sf::Texture			_help2Texture;
		sf::Texture			_help3Texture;
		sf::Texture			_help4Texture;
		sf::Texture			_help5Texture;
		sf::Texture			_help5PlusTexture;
		sf::Texture			_chronoTexture;
		sf::Texture			_blackTimerTexture;
		sf::Texture			_whiteTimerTexture;
		sf::Texture			_bestTexture;


		sf::SoundBuffer		bipSB;
		sf::SoundBuffer		captureSB;
		sf::SoundBuffer		victorySB;

		sf::Sound			bipSound;
		sf::Sound			captureSound;
		sf::Sound			victorySound;
		sf::Music			ambiance1;
		sf::Music			ambiance2;

		sf::Text			whiteTimeToPlayText;
		sf::Text			blackTimeToPlayText;
		sf::Text			timeOfGameText;
		sf::Text			nbTurnText;
		/*sf::Text			help1;
		sf::Text			help2;
		sf::Text			help3;
		sf::Text			help4;
		sf::Text			help5;*/
		sf::Text			visualAidText;
		sf::Text			rulesText;

		sf::RectangleShape	fiveByFiveRect;
		sf::RectangleShape	sevenBySevenRect;

};

#endif
