#include "menu.h"
#include "player.h"
#include "humanPlayer.h"
#include "minMaxDynamicPlayer.h"
#include "randomPlayer.h"
#include "noobIA.h"

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
        textBoxP1.setColor(Color::Red);
        textBoxP2.setColor(Color::Black);
        textBoxVariante.setColor(Color::Blue);
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
        case CLASSIQUE:
            text.setString("CLASSIQUE");
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
        textC = HUMAN;
    else if (textC == CLASSIQUE)
        ;//TO DO : update texte si d'autres variantes de jeu à venir
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
    gomoku->setWhitePlayer(updatePlayer(choiceP1));
    gomoku->setBlackPlayer(updatePlayer(choiceP2));
	gomoku->getBlackPlayer().setGomoku(gomoku);
	gomoku->getWhitePlayer().setGomoku(gomoku);
}

Player*    Menu::updatePlayer(TextChoice &textC) {
    if (textC == HUMAN)
        return new HumanPlayer();
    else if (textC == RANDOM)
        return new NoobIA();
    else if (textC == IA_HARD)
        return new MinMaxDynamicPlayer({7, 7, 7, 5, 5, 5, 3, 3, 3, 3, 0});
        //return new MinMaxDynamicPlayer({11, 11, 7, 7, 7, 3, 3, 1, 1, 1, 0});
    else if (textC == IA_NORMAL)
        return new MinMaxDynamicPlayer({7, 7, 5, 5, 3, 3, 0});
    else if (textC == IA_EASY)
        return new MinMaxDynamicPlayer({7, 3, 0});
    else {
        DEBUG << "ERROR SWITCH PLAYER IN GO MENU\n";
        exit(1);
    }
	return NULL;
}
