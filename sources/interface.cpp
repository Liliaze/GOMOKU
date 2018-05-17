#include "interface.h"
#include "gomoku.h"
#include "player.h"
#include "rules.h"
#include "utils.h"
#include "menu.h"

using namespace sf;

Interface::Interface() : _window(sf::VideoMode(WIDTH, HEIGHT), "GOMOKU", Style::Titlebar | Style::Close)
{
   _window.setFramerateLimit(60);
   _window.setVerticalSyncEnabled(true);
   this->loadTexture();
   this->loadSprite();
   this->loadSoundBuffer();
   this->loadSoundAndOpenMusic();
   this->loadText();
   initCoordBoard();
   initCoordCanteen();
   this->setState(WELCOME);
   previewStone = false;
   viewWinner = false;
   DEBUG << "INTERFACE READY\n";
}

Interface::~Interface() {
}

//TO-DO : Ajout texture des autres States ici.
void    Interface::loadTexture(void) {
   if(!_stoneWhiteTexture.loadFromFile("./sprite/whiteStone.png")
    || !_stoneBlackTexture.loadFromFile("./sprite/blackStone.png")
    || !_backgroundTexture.loadFromFile("./sprite/background.png")
    || !_boardGameTexture.loadFromFile("./sprite/goban(2).png")
    || !_helloTexture.loadFromFile("./sprite/hello.png")
    || !_goodByeTexture.loadFromFile("./sprite/goodBye.png")
    || !_againTexture.loadFromFile("./sprite/again.png")
    || !_whiteWinTexture.loadFromFile("./sprite/white_win.png")
    || !_blackWinTexture.loadFromFile("./sprite/black_win.png")
    || !_equalityTexture.loadFromFile("./sprite/equality.png")
    || !_againYesTexture.loadFromFile("./sprite/againYesBox.png")
    || !_againNoTexture.loadFromFile("./sprite/againNoBox.png")
    || !_boxTexture.loadFromFile("./sprite/boxCube.png")
    || !_help1Texture.loadFromFile("./sprite/help1.png")
    || !_help2Texture.loadFromFile("./sprite/help2.png")
    || !_help3Texture.loadFromFile("./sprite/help3.png")
    || !_help4Texture.loadFromFile("./sprite/help4.png")
    || !_help5Texture.loadFromFile("./sprite/help5.png")
    || !_help5PlusTexture.loadFromFile("./sprite/help5Plus.png")
    || !_bestTexture.loadFromFile("./sprite/best.png")
    || !_chronoTexture.loadFromFile("./sprite/chrono.png")
    ) {
       DEBUG << "Error during import " << std::endl;
       exit(1);
   }
   else {
       _stoneWhiteTexture.setSmooth(true);
       _stoneBlackTexture.setSmooth(true);
       _backgroundTexture.setSmooth(true);
       _boardGameTexture.setSmooth(true);
       _goodByeTexture.setSmooth(true);
       _helloTexture.setSmooth(true);
       _againTexture.setSmooth(true);
       _whiteWinTexture.setSmooth(true);
       _blackWinTexture.setSmooth(true);
       _equalityTexture.setSmooth(true);
       _boxTexture.setSmooth(true);
       _help1Texture.setSmooth(true);
       _help2Texture.setSmooth(true);
       _help3Texture.setSmooth(true);
       _help4Texture.setSmooth(true);
       _help5Texture.setSmooth(true);
       _help5PlusTexture.setSmooth(true);
       _bestTexture.setSmooth(true);
       _chronoTexture.setSmooth(true);
   }
}

void    Interface::loadSprite(void) {
    makeSprite(_backgroundSprite, _backgroundTexture, 1, 1, 0, 0);
    makeSprite(_boardGameSprite, _boardGameTexture, 0.9f, 0.9f, (_backgroundTexture.getSize().x - (_boardGameTexture.getSize().x * 0.9)) / 2, (_backgroundTexture.getSize().y - (_boardGameTexture.getSize().y * 0.9)) / 2);
    makeSprite(_whiteStone, _stoneWhiteTexture, 0.825f, 0.825f, 0, 0);
    makeSprite(_blackStone, _stoneBlackTexture, 0.825f, 0.825f, 0, 0);
    makeSprite(_goodByeSprite, _goodByeTexture, 1, 1, 0, 0);
    makeSprite(_helloSprite, _helloTexture, 1, 1, 0, 0);
    makeSprite(_againSprite, _againTexture, 1, 1, 0, 0);
    makeSprite(_whiteWinSprite, _whiteWinTexture, 1, 1, 1, 1);
    makeSprite(_blackWinSprite, _blackWinTexture, 1, 1, 1, 1);
    makeSprite(_equalitySprite, _equalityTexture, 1, 1, 1, 1);
    makeSprite(_boxSprite, _boxTexture, 1, 1, HELPERX - 10, HELPERY - 10);
    makeSprite(_againYesSprite, _againYesTexture, 1, 1, YES_LEFT, YES_UP);
    makeSprite(_againNoSprite, _againNoTexture, 1, 1, NO_LEFT, NO_UP);
    makeSprite(_boxSelectSprite, _boxTexture, 1, 1, 0, 0);
    makeSprite(previewStoneFree, _stoneWhiteTexture,0.825f, 0.825f, 0, 0);
    makeSprite(previewStoneForbidden, _stoneWhiteTexture,0.825f, 0.825f, 0, 0);
    makeSprite(_help1Sprite, _help1Texture,0.82f, 0.82f, 0, 0);
    makeSprite(_help2Sprite, _help2Texture,0.82f, 0.82f, 0, 0);
    makeSprite(_help3Sprite, _help3Texture,0.82f, 0.82f, 0, 0);
    makeSprite(_help4Sprite, _help4Texture,0.82f, 0.82f, 0, 0);
    makeSprite(_help5Sprite, _help5Texture,0.82f, 0.82f, 0, 0);
    makeSprite(_help5PlusSprite, _help5PlusTexture,0.82f, 0.82f, 0, 0);
    makeSprite(_bestSprite, _bestTexture, 0.825f, 0.825f, 0, 0);
    makeSprite(_chronoSprite, _chronoTexture,0.8f, 0.8f, CHRONOX, CHRONOY);
    _whiteStone.setOrigin(_stoneWhiteTexture.getSize().x / _whiteStone.getScale().x / 2, _stoneWhiteTexture.getSize().y / _whiteStone.getScale().y / 2);
    _blackStone.setOrigin(_stoneBlackTexture.getSize().x / _blackStone.getScale().x / 2, _stoneBlackTexture.getSize().y / _blackStone.getScale().y / 2);
    _bestSprite.setOrigin(_bestTexture.getSize().x / _bestSprite.getScale().x / 2, _bestTexture.getSize().y / _bestSprite.getScale().y / 2);
    _help1Sprite.setOrigin(_help1Texture.getSize().x / _help1Sprite.getScale().x / 2, _help1Texture.getSize().y / _help1Sprite.getScale().y / 2);
    _help2Sprite.setOrigin(_help1Texture.getSize().x / _help1Sprite.getScale().x / 2, _help1Texture.getSize().y / _help1Sprite.getScale().y / 2);
    _help3Sprite.setOrigin(_help1Texture.getSize().x / _help1Sprite.getScale().x / 2, _help1Texture.getSize().y / _help1Sprite.getScale().y / 2);
    _help4Sprite.setOrigin(_help1Texture.getSize().x / _help1Sprite.getScale().x / 2, _help1Texture.getSize().y / _help1Sprite.getScale().y / 2);
    _help5Sprite.setOrigin(_help1Texture.getSize().x / _help1Sprite.getScale().x / 2, _help1Texture.getSize().y / _help1Sprite.getScale().y / 2);
    _help5PlusSprite.setOrigin(_help1Texture.getSize().x / _help1Sprite.getScale().x / 2, _help1Texture.getSize().y / _help1Sprite.getScale().y / 2);
    previewStoneFree.setOrigin(_stoneWhiteTexture.getSize().x / _whiteStone.getScale().x / 2, _stoneWhiteTexture.getSize().y / _whiteStone.getScale().y / 2);
    previewStoneForbidden.setOrigin(_stoneBlackTexture.getSize().x / _blackStone.getScale().x / 2, _stoneBlackTexture.getSize().y / _blackStone.getScale().y / 2);
    previewStoneFree.setColor(Color(0,255,0,125));
    previewStoneForbidden.setColor(Color(255,0,0,125));
    _boxSelectSprite.setColor(Color(125,175,125,200));
   DEBUG << "SPRITES LOADS\n";
}

void    Interface::makeSprite(Sprite &sprite, Texture &texture, float sizeX, float sizeY, int posX, int posY) {
    sprite.setTexture(texture);
    sprite.setScale(sizeX, sizeY);
    sprite.setPosition(posX, posY);
}

void    Interface::loadSoundBuffer(void) {
     if (!bipSB.loadFromFile("./sound/bip.wav")
        || !captureSB.loadFromFile("./sound/capture.wav")
        || !victorySB.loadFromFile("./sound/victory.wav")) {
         DEBUG << "ERROR DURING LOAD SOUND BUFFER\n";
         exit(1);
     }
}

void    Interface::loadSoundAndOpenMusic(void) {
    bipSound.setBuffer(bipSB);
    captureSound.setBuffer(captureSB);
    victorySound.setBuffer(victorySB);
    if (!ambiance1.openFromFile("./sound/ambiance1.wav")
        || !ambiance2.openFromFile("./sound/ambiance2.wav")) {
            DEBUG << "ERROR DURING OPEN MUSIC\n";
            exit(1);
        }
    else {
        /*bipSound.setVolume(50);
        captureSound.setVolume(50);
        testSound.setVolume(50);
        ambiance1.setVolume(30);
        ambiance2.setVolume(30);*/
        ambiance1.setLoop(true);
        ambiance2.setPlayingOffset(sf::seconds(30));
        ambiance2.play();
    }
}
void    Interface::setText(Text *text, Font &font, int size, Color color, int posX, int posY, String str) {
    text->setFont(font);
    text->setCharacterSize(size);
    text->setColor(color);
    text->setPosition(posX, posY);
    text->setString(str);
    menu.setMiddle(*text);
}

void    Interface::loadText(void) {
    Font &arial = menu.getArial();
    setText(&timeOfGameText, arial, 18, Color::Black,  CHRONOX + 65, CHRONOY + 135, "TIME 00.00\n\n\nTIME OF GAME");
    setText(&nbTurnText, arial,18, Color::Blue, 10, 450, "TURN :\n 00");
    setText(&timeToPlayText, arial, 18, Color(0, 125, 250), 10, 500, "Player's time :\n0,000");
    /*
    setText(&help1, arial, 24, Color::Blue, 300, 300, "1");
    setText(&help2, arial, 24, Color(255,0,128), 400, 300, "2");
    setText(&help3, arial, 24, Color::Yellow, 500, 300, "3");
    setText(&help4, arial, 24, Color::Red, 600, 300, "4");
    setText(&help5, arial, 24, Color::Green, 700, 300, "5");
    */
    setText(&visualAidText, arial, 18, Color::Red, HELPERX, HELPERY, "    To activ\nVisualHelper :\n     [false]");
    menu.setMiddle(timeOfGameText);
    timeToPlayText.setOrigin(0,0);
    nbTurnText.setOrigin(0,0);
    visualAidText.setOrigin(0,0);
}

void    Interface::initCoordBoard(void) {
    int i = 0;
    int j = 0;
    float step = 40.0f;//(float)((BOARD_RIGHT - BOARD_LEFT) / 18);
    float stepy = 0;
    float stepx = 0;
    int y = BOARD_UP;
    while (i < GW) {
        int x = BOARD_LEFT;
        j = 0;
        stepx = 0;
        while (j < GH) {
            coordBoard[i][j] = Vector2<int>((int)(x + stepx), (int)(y + stepy));
            stepx+= step;
            j++;
        }
        stepy += step;
        i++;
    }
}

void    Interface::initCoordCanteen(void) {
    int i = 0;
    float step = 40.0f;//(float)((BOARD_RIGHT - BOARD_LEFT) / 18);
    float stepx = 0;
    int y = BLACKCANTEENY;
    int x = BLACKCANTEENX;
    stepx = 0;
    while (i < NB_CAPTURE_TO_WIN) {
        blackCanteen[i] = Vector2<int>((int)(x + stepx), (int)(y));
            //this->putStone(this->_blackStone, blackCanteen[i].x, blackCanteen[i].y);
            stepx+= step;
        i++;
    }
    i = 0;
    stepx = 0;
    y = WHITECANTEENY;
    x = WHITECANTEENX;
    stepx = 0;
    while (i < NB_CAPTURE_TO_WIN) {
        whiteCanteen[i] = Vector2<int>((int)(x - stepx), (int)(y));
       // this->putStone(this->_whiteStone, whiteCanteen[i].x, whiteCanteen[i].y);
        stepx+= step;
        i++;
    }
}

void    Interface::printCoordBoard(void) {
    int i = 0;
    int j = 0;
    while (i < GW) {
        j = 0;
        while (j < GH) {
            DEBUG << coordBoard[i][j].x << "," << coordBoard[i][j].y << "|";
//            this->putStone(*(gomoku->getCurrentPlayer()->getSpriteStone()), coordBoard[i][j].x, coordBoard[i][j].y);
            j++;
        }
        DEBUG << "\n";
        i++;
    }
}

void    Interface::setStoneOnClick(Player &current, int clickx, int clicky) {
    Vector2<int> tmp = turnCoordInterfaceInGomokuBoardIndex(clickx, clicky);
    if (tmp.x == -1)
        return;
    if (gomoku->getRules().canPutStone(current, tmp.x, tmp.y)) {
        current.setCoordPlayed(tmp.x, tmp.y);
        current.setPlayed(true);
        bipSound.play();
    }
}

Vector2<int>    Interface::turnCoordInterfaceInGomokuBoardIndex(int mouseX, int mouseY) {
    int i = 0;
    int j = 0;
    Vector2<int> tmp;
    while (i < GW) {
        j = 0;
        while (j < GH) {
            if (mouseX <= coordBoard[i][j].x + CENTER_MAX_DISTANCE && mouseX >= coordBoard[i][j].x - CENTER_MAX_DISTANCE &&
                mouseY <= coordBoard[i][j].y + CENTER_MAX_DISTANCE && mouseY >= coordBoard[i][j].y - CENTER_MAX_DISTANCE) {
                tmp.x = i;
                tmp.y = j;
                return (tmp); 
                }
            j++;
            }
        i++;
    }
    tmp.x = -1;
    tmp.y = -1;
    return (tmp); 
}

void	Interface::capture(Player &current, sf::Sprite *spriteEnemy, int x1, int y1) {
    int i = getCoordBoard(x1,y1).x;
    int j = getCoordBoard(x1,y1).y;
    captureSound.play();
    removeStone(i,j);
    this->putStone(*spriteEnemy, current.getCoordCanteen(current.getNbCapture() - 1).x, current.getCoordCanteen(current.getNbCapture() - 1).y);
}

void    Interface::removeStone(int i, int j) {
    Vector2<int> pos;
    for (std::list<Sprite>::iterator it = _allSprite.begin(); it != _allSprite.end(); it++) {
        pos = (Vector2<int>)(*it).getPosition();
        if ((pos.x == i && pos.y == j)){
            it = _allSprite.erase(it);
            break;
        }
    }
}

void    Interface::removeText(Vector2<int> coord) {
    Vector2<int> pos;
    for (std::list<Text*>::iterator it = _allText.begin(); it != _allText.end(); it++) {
        pos = (Vector2<int>)((*it)->getPosition());
        if ((pos.x == coord.x && pos.y == coord.y)){
            it = _allText.erase(it);
            break;
        }
    }
}

void    Interface::setState(State newState)
{
    if (state == PAUSE && newState != PAUSE) {
        this->state = newState;
        stopPauseScreen();
        return;
    }
    else if (state == PAUSE) {
        this->state = newState;
        startPauseScreen();
        return;
    }
    this->state = newState;
    switch (state) {
            case MENU :
                menuScreen();
                break;
            case GAME :
                gameScreen();
                break;
            case WHITEWIN :
                whiteWinScreen();
                break;
            case BLACKWIN :
                blackWinScreen();
                break;
            case EQUAL :
                equalScreen();
                break;
            case AGAIN :
                againScreen();
                break;
            case GOODBYE :
                goodByeScreen();
                break;
            case WELCOME :
                welcomeScreen();
                break;
            default :
                DEBUG << "ERROR SCREEN STATE ???";
                exit(0);
                break;
    }
    this->update();
}

void    Interface::startPauseScreen(void) {
    DEBUG <<" PAUSE START";
}

void    Interface::stopPauseScreen(void) {
    DEBUG <<" PAUSE STOP";
}

void    Interface::drawGame(void) {
    //Affichage des sprites en premier:
    for (std::list<Sprite>::iterator it = _allSprite.begin(); it != _allSprite.end(); it++) {
        this->_window.draw(*it);
    }
    //Affichage des textes par dessus :
    for (std::list<Text*>::iterator it = _allText.begin(); it != _allText.end(); it++) {
        this->_window.draw(*(*it));
    }
    //En jeu si demandé affichage de l'aide au joueur :
    for (std::list<Sprite>::iterator it = _allHelpSprite.begin(); it != _allHelpSprite.end(); it++) {
        this->_window.draw(*it);
    }
}

void    Interface::cleanInterface(void) {
    _allText.clear();
    _allHelpSprite.clear();
    _allSprite.clear();
}

void    Interface::welcomeScreen(void) {
    _allSprite.push_back(_helloSprite);
    this->state = WELCOME;
    DEBUG << "WELCOME SCREEN\n";
    update();
    sleep(1);
}

void    Interface::goodByeScreen(void) {
    DEBUG << "GOODBYE SCREEN\n";
    this->state = GOODBYE;
    cleanInterface();
    _allSprite.push_back(_goodByeSprite);
    ambiance1.stop();
    ambiance2.stop();
}

void    Interface::menuScreen(void) {
    DEBUG << "MENU SCREEN\n";
    cleanInterface();
    _allSprite.push_back(menu.backgroundMenuSprite);
    _allText.push_back(&menu.textBoxP1);
    _allText.push_back(&menu.textBoxP2);
    _allText.push_back(&menu.textBoxVariante);
    this->state = MENU;
}

void    Interface::againScreen(void) {
    DEBUG << "AGAIN SCREEN\n";
    cleanInterface();
    _allSprite.push_back(_againSprite);
    _allSprite.push_back(_againYesSprite);
    _allSprite.push_back(_againNoSprite);
    ambiance1.stop();
    ambiance2.play();
    this->state = AGAIN;
}

void    Interface::gameScreen(void) {
    cleanInterface();
    setTimeOfGame(_clockOfGame.restart());
    setTimeToPlay(_clockTurn.restart());
    _allSprite.push_back(_backgroundSprite);
    _allSprite.push_back(_boardGameSprite);
    _allSprite.push_back(_chronoSprite);
    _allText.push_back(&timeOfGameText);
    _allText.push_back(&nbTurnText);
    _allText.push_back(&timeToPlayText);
    if (gomoku->getBlackPlayer().getHuman() == true || gomoku->getWhitePlayer().getHuman() == true) {
        _allText.push_back(&visualAidText);
        _allSprite.push_back(_boxSprite);
    }
    //_allText.push_back(&help1);
    //_allText.push_back(&help2);
    //_allText.push_back(&help3);
    //_allText.push_back(&help4);
    //_allText.push_back(&help5);
    ambiance2.stop();
    ambiance1.play();
    this->state = GAME;
}

void    Interface::whiteWinScreen(void) {
    DEBUG << "WHITE WIN SCREEN\n";
    victorySound.play();
    _allSprite.push_back(_whiteWinSprite);
    this->state = WHITEWIN;
}

void    Interface::blackWinScreen(void) {
    DEBUG << "BLACK WIN SCREEN\n";
    victorySound.play();
    _allSprite.push_back(_blackWinSprite);
    this->state = BLACKWIN;
}

void    Interface::equalScreen(void) {
    DEBUG << "EQUAL WIN SCREEN\n";
    victorySound.play();
    _allSprite.push_back(_equalitySprite);
    this->state = EQUAL;
}

void    Interface::timer(void) {

}

void    Interface::captureZone(void) {

}

void    Interface::putStone(Sprite stone, int x, int y) {
        stone.setPosition(x, y);
        _allSprite.push_back(stone);
}

void    Interface::putStoneToHelp(int i, int j) {
        Vector2<int> coord = coordBoard[i][j];
        _bestSprite.setPosition(coord.x, coord.y);
}

void    Interface::putPreviewStone(Player &current, int mouseX, int mouseY)
{
    Vector2<int> tmp = turnCoordInterfaceInGomokuBoardIndex(mouseX, mouseY);
    if (tmp.x == -1)
        return;
    Vector2<int> coord = getCoordBoard(tmp.x, tmp.y);
    if (gomoku->getStone(tmp.x, tmp.y) == FREE) {
        if ((gomoku->getRules()).canPutStone(current, tmp.x, tmp.y)) {
            this->putStone(this->previewStoneFree, coord.x, coord.y);
        }
        else {
            this->putStone(this->previewStoneForbidden, coord.x, coord.y);
        }
        this->previewStone = true;
        this->coordPreviewStone.x = coord.x;
        this->coordPreviewStone.y = coord.y;
    }
}

void    Interface::unputPreviewStone(int mouseX, int mouseY) {
        if (!this->previewStone)
            return;
        Vector2<int> tmp = turnCoordInterfaceInGomokuBoardIndex(mouseX, mouseY);
        if (tmp.x == -1)
            return;
        Vector2<int> coord = getCoordBoard(tmp.x, tmp.y);
        if (coord.x != coordPreviewStone.x || coord.y != coordPreviewStone.y) {
            this->previewStone = false;
            removeStone(this->coordPreviewStone.x, this->coordPreviewStone.y);
            this->coordPreviewStone.x = -1;
            this->coordPreviewStone.y = -1;
        }
}

void    Interface::checkEvent(Player *current) {
    while (_window.pollEvent(_event))
    {
        switch (_event.type)
        {
            case Event::Closed :
                gomoku->end();
                break;
            case Event::KeyPressed :
            {
                switch (_event.key.code)
                {
                    case Keyboard::Escape :
                        gomoku->end();
                        break;
                    case Keyboard::Q :
                        gomoku->end();
                        break;
                    default :
                        break;
                }
            }
            break;
            case Event::MouseMoved :
            {
                if (current && !current->getHuman())
                    break;
                if (state == GAME) {
                    int x = _event.mouseMove.x;
                    int y = _event.mouseMove.y;
                    if (onBoard(x,y)) {
                        if (!previewStone && current) {
                            putPreviewStone(*current, x, y);
                            update();
                        }
                        else
                            unputPreviewStone(x, y);

                    }
                    else if (previewStone)
                        unputPreviewStone(x, y);
                }
                else if (state == AGAIN) {
                    //TO DO : box YES or NO en surbrillance
                }
            }
            break;
            case Event::MouseButtonPressed :
            {
                switch (_event.mouseButton.button)
                {
                    case Mouse::Left :
                        this->checkClickLeft(current, _event.mouseButton.x, _event.mouseButton.y);
                        DEBUG << "click : xy(" <<_event.mouseButton.x << "," << _event.mouseButton.y << ")\n"; 
                    break;
                default :
                    break;
                }
            }
            break;
            default :
                break;
        }
    }
}

bool    Interface::onVisualAid(int x, int y)
{
    if (x <= BOARD_LEFT - 50 && 
        x >= HELPERX - 10 &&
        y >= HELPERY - 10 &&
        y <= 110) {
        return true;}
    else {
        return false;}
}

bool    Interface::onBoard(int x, int y)
{
    if (x <= BOARD_RIGHT + MARGE && 
        x >= BOARD_LEFT - MARGE &&
        y >= BOARD_UP - MARGE &&
        y <= BOARD_DOWN  + MARGE) 
        return true;
    else
        return false;
}

bool    Interface::onAgainYes(int x, int y) {
    if (x <= YES_RIGHT + MARGE && 
        x >= YES_LEFT - MARGE &&
        y >= YES_UP - MARGE &&
        y <= YES_DOWN  + MARGE)
        return true;
    else
        return false;
}

bool    Interface::onAgainNo(int x, int y) {
    if (x <= NO_RIGHT + MARGE && 
        x >= NO_LEFT - MARGE &&
        y >= NO_UP - MARGE &&
        y <= NO_DOWN  + MARGE) 
        return true;
    else
        return false;
}

void    Interface::checkClickLeft(Player *current, int x, int y)
{
    if (state == GAME)
    {
        if (onBoard(x, y) && current)
            this->setStoneOnClick(*current, x, y);
        else if (onVisualAid(x,y) && (gomoku->getBlackPlayer().getHuman() == true || gomoku->getWhitePlayer().getHuman() == true))
                updateVisualAid();
    }
    else if (state == WELCOME){
        //DEBUG << "click during Welcome\n";
    }
    else if (state == MENU){
        if (menu.onP1(x, y))
        {
            bipSound.play();
            menu.switchTextBox(menu.textBoxP1, menu.choiceP1);
        }
        else if (menu.onP2(x, y))
        {
            bipSound.play();
            menu.switchTextBox(menu.textBoxP2, menu.choiceP2);
        }
        else if (menu.onVariante(x, y))
        {
            //bipSound.play();
            menu.switchTextBox(menu.textBoxVariante, menu.variante);
        }
        else if (menu.onGo(x, y)){
            bipSound.play();
            menu.go(gomoku);//setPlayer...
            setState(GAME);
        }
    }
    else if (state == BLACKWIN || state == WHITEWIN || state == EQUAL){
        if (viewWinner) {
            setState(AGAIN);
            viewWinner = false;
        }
        else {
            removeStone(1,1);
            viewWinner = true;
        }
        DEBUG << "click during victory or equality screen\n";
    }
    else if (state == AGAIN){
        DEBUG << "click during Again\n";
        if (onAgainYes(x, y)){
            setState(MENU);
        }
        else if (onAgainNo(x, y)) {
            gomoku->end();
        }
    }
    else if (state == PAUSE){
        DEBUG << "click during Pause\n";
    }
    else if (state == GOODBYE){
        DEBUG << "click during End\n";
    }
    else
        DEBUG << "click during unknow state\n";
}

void    Interface::updateTimerOfGame(void) {
        setTimeOfGame(this->_clockOfGame.getElapsedTime());
        String str = "          " + intToString(((int)getTimeOfGameInSeconds())) + "\n\n\nTIME OF GAME";
        timeOfGameText.setString(str);
        //removeStone(timeOfGameText.getPosition().x, timeOfGameText.getPosition().y);
        //_allText.push_back(&timeOfGameText);
}

void    Interface::updateTimerToPlay(void) {
        String str = "Player's time :\n" + floatToString(getTimeToPlayInSeconds());
        timeToPlayText.setString(str);
}

void    Interface::updateNbOfTurn(void) {
        String str = "Nb of turn :\n" + intToString(((gomoku->getRules()).getTurnCounter()));
        nbTurnText.setString(str);
}

void    Interface::updateVisualAid(void) {
        String str = "    To Activ\nVisualHelper :\n     [";
        if (visualAid) {
            visualAid = false;
            visualAidText.setColor(Color::Red);
            _allHelpSprite.clear();
            str += "false]";
        }
        else {
            visualAidText.setColor(Color(50,200,50));
            visualAid = true;
            str += "TRUE]";
            updateHelperToPlay();
        }
        visualAidText.setString(str);
        bipSound.play();
        update();
}

void    Interface::putHelpSprite(Sprite sprite, int x, int y) {
            sprite.setPosition(coordBoard[x][y].x, coordBoard[x][y].y);
            _allHelpSprite.push_back(sprite);
            //_window.draw(text);
}

void    Interface::updateHelperToPlay() {
    if (!(gomoku->getCurrentPlayer()->getHuman()))
        return;
    _allHelpSprite.clear();
    //appeler ici l'heuristic du current player pour le parcourir et afficher les texts help1, help2 ect...
    HeuristicBoard currentHeuristic = gomoku->getCurrentPlayer()->getMyHeuristic();
    int level;
    for (short x = 0; x < GH; x++) {
        for (short y = 0; y < GW; y++) {
            level = currentHeuristic.getBestLevel(x,y);
            if (level == 0)
                continue;
            else if (level == 1)
                putHelpSprite(_help1Sprite, x, y);
            else if (level == 2)
                putHelpSprite(_help2Sprite, x, y);
            else if (level == 3)
                putHelpSprite(_help3Sprite, x, y);
            else if (level == 4)
                putHelpSprite(_help4Sprite, x, y);
            else if (level == 5)
                putHelpSprite(_help5Sprite, x, y);
            else if (level > 5)
                putHelpSprite(_help5PlusSprite, x, y);
        }
    }
    //affichage du sprite best au coordonné précédemment enregistré par le helper...
    gomoku->getCurrentPlayer()->getHelper()->play(gomoku->getRules(), *this);
    _allHelpSprite.push_back(_bestSprite);
}

void    Interface::updateAllGameText() {
    updateTimerOfGame();
    updateTimerToPlay();
    updateNbOfTurn();
}

void    Interface::update(void) {
        if (state == GAME)
            this->updateAllGameText();
        this->_window.clear();
        this->drawGame();
        //if ((state == GAME /* || state == BLACKWIN || state == WHITEWIN || state == EQUAL*/) && visualAid)
        //    updateHelperToPlay();
        this->_window.display();
}
