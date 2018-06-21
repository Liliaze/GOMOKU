#include "menu.h"
#include "player.h"
#include "humanPlayer.h"
#include "minMaxDynamicPlayer.h"
#include "assistedHumanPlayer.h"
#include "randomPlayer.h"
#include "noobIA.h"
#include "defaultRules.h"
#include "proRules.h"
#include "longProRules.h"
#include "swapRules.h"
#include "swapTwoRules.h"

using namespace sf;

Menu::Menu() {
    choiceP1 = HUMAN;
    choiceP2 = HUMAN;
    variante = CLASSIQUE;
    if (!arial.loadFromFile("./sprite/arial_black.ttf")){
        DEBUG << "Error load Font Arial\n";
    }
    else {
        DEBUG << "SET\n";
        textBoxP1.setFont(arial);
        textBoxP2.setFont(arial);
        textBoxVariante.setFont(arial);
        textBoxP1.setCharacterSize(24);
        textBoxP2.setCharacterSize(24);
        textBoxVariante.setCharacterSize(24);
        textBoxP1.setFillColor(Color::Red);
        textBoxP2.setFillColor(Color::Black);
        textBoxVariante.setFillColor(Color::Blue);
        updateText(textBoxP1, choiceP1);
        updateText(textBoxP2, choiceP2);
        updateText(textBoxVariante, variante);
        textBoxP1.setPosition(XBOX_MIDDLE, YBOX_P1_MIDDLE);
        textBoxP2.setPosition(XBOX_MIDDLE, YBOX_P2_MIDDLE);
        textBoxVariante.setPosition(XBOX_MIDDLE, YBOX_VARIANTE_MIDDLE);
    }
    if (!backgroundMenuTexture.loadFromFile("./sprite/menu.png")) {
        DEBUG << "Error load Texture Menu\n";
    }
    else {
        backgroundMenuTexture.setSmooth(true);
        backgroundMenuSprite.setTexture(backgroundMenuTexture);
    }
    DEBUG << "MENU READY\n";
}

Menu::~Menu() {
}

void Menu::setMiddle(Text &text)
{
    FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.width/2.0f, textRect.height/2.0f);
}

void    Menu::setTextString(Text &text, TextChoice &textChoice) {
    switch (textChoice) {
        case HUMAN:
            text.setString("HUMAN");
            break;
        case RANDOM:
            text.setString("RANDOM PLAYER");
            break;
        case IA_HARD:
            text.setString("IA HARD");
            break;
        case IA_EASY:
            text.setString("IA EASY");
            break;
        case IA_NORMAL:
            text.setString("IA NORMAL");
            break;
        case AH_HARD:
            text.setString("Assisted Human Hard");
            break;
        case AH_EASY:
            text.setString("Assisted Human Easy");
            break;
        case AH_NORMAL:
            text.setString("Assisted Human Normal");
            break;
        case CLASSIQUE:
            text.setString("CLASSIQUE");
            break;
        case PRO:
            text.setString("PRO");
            break;
        case LONG_PRO:
            text.setString("LONG PRO");
            break;
        case SWAP:
            text.setString("SWAP");
            break;
        case SWAP2:
            text.setString("SWAP2");
            break;
        default:
            DEBUG << "not Text???? wtf\n";
            exit(0);
            break;
    }
}

void    Menu::updateText(Text &text, TextChoice &textChoice) {
    setTextString(text,textChoice);
    setMiddle(text);
}

void    Menu::switchTextBox(Text &text, TextChoice &textC) {
    if (textC == HUMAN)
        textC = RANDOM;
    else if (textC == RANDOM)
        textC = IA_HARD;
    else if (textC == IA_HARD)
        textC = IA_NORMAL;
    else if (textC == IA_NORMAL)
        textC = IA_EASY;
    else if (textC == IA_EASY)
        textC = AH_HARD;
    else if (textC == AH_HARD)
        textC = AH_NORMAL;
    else if (textC == AH_NORMAL)
        textC = AH_EASY;
    else if (textC == AH_EASY)
        textC = HUMAN;
    else if (textC == CLASSIQUE)
        textC = PRO;
    else if (textC == PRO)
        textC = LONG_PRO;
    else if (textC == LONG_PRO)
        textC = SWAP;
    else if (textC == SWAP)
        textC = SWAP2;
    else if (textC == SWAP2)
        textC = CLASSIQUE;
    else
        DEBUG << "ERROR SWITCH TEXT IN MENU\n";
    updateText(text, textC);
}

bool    Menu::onP1(int x, int y) {
    if (x <= XBOX_END && x >= XBOX_START && y <= P1_DOWN && y >= P1_UP)
        return (true);
    else
        return (false);
}
bool    Menu::onP2(int x, int y) {
    if (x <= XBOX_END && x >= XBOX_START && y <= P2_DOWN && y >= P2_UP)
        return (true);
    else
        return (false);
}
bool    Menu::onVariante(int x, int y) {
    if (x <= XBOX_END && x >= XBOX_START && y <= VARIANTE_DOWN && y >= VARIANTE_UP)
        return (true);
    else
        return (false);
}

bool    Menu::onGo(int x, int y) {
    if (x <= XBOX_END && x >= XBOX_START && y <= GO_DOWN && y >= GO_UP)
        return (true);
    else
        return (false);
}

void    Menu::go(Gomoku* gomoku) {
    delete &(gomoku->getWhitePlayer());
    delete &(gomoku->getBlackPlayer());
    delete &(gomoku->getRules());
    gomoku->setRules(updateRules(variante));
    gomoku->setWhitePlayer(updatePlayer(choiceP1));
    gomoku->setBlackPlayer(updatePlayer(choiceP2));
	gomoku->getBlackPlayer().setGomoku(gomoku);
	gomoku->getWhitePlayer().setGomoku(gomoku);
}

Rules*      Menu::updateRules(TextChoice &textC) {
    if (textC == CLASSIQUE)
        return new DefaultRules();
    else if (textC == PRO)
        return new ProRules();
    else if (textC == LONG_PRO)
        return new LongProRules();
    else if (textC == SWAP)
        return new SwapRules();
    else if (textC == SWAP2)
        return new SwapTwoRules();
    else {
        DEBUG << "IN MENU, UPDATERULES, TEXT UNKNOW";
        exit(1);
    }
}

Player*    Menu::updatePlayer(TextChoice &textC) {
    if (textC == HUMAN)
        return new HumanPlayer();
    else if (textC == RANDOM)
        return new NoobIA();
    else if (textC == IA_HARD) {
        //return new MinMaxDynamicPlayer({11, 9, 7, 5, 5, 5, 3, 3, 3, 3, 0});
		DEBUG << "IA HARD\n";
        return new MinMaxDynamicPlayer({11, 11, 7, 7, 7, 3, 3, 1, 1, 1, 0});
    } else if (textC == IA_NORMAL)
        return new MinMaxDynamicPlayer({7, 7, 5, 5, 3, 3, 0});
    else if (textC == IA_EASY)
        return new MinMaxDynamicPlayer({7, 3, 0});
    else if (textC == AH_HARD)
        return new AssistedHumanPlayer({11, 9, 7, 5, 5, 5, 3, 3, 3, 3, 0});
    else if (textC == AH_NORMAL)
        return new AssistedHumanPlayer({7, 7, 5, 5, 3, 3, 0});
    else if (textC == AH_EASY)
        return new AssistedHumanPlayer({7, 3, 0});
    else {
        DEBUG << "ERROR SWITCH PLAYER IN GO MENU\n";
        exit(1);
    }
	return NULL;
}
