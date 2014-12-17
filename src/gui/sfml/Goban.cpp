#include "Goban.h"

Goban::Goban(const Map& m, sf::RenderWindow& mainWindow)
: AGui(m.sizeY(), m.sizeX(), AGui::GAME), _m(m), _mainWindow(mainWindow) {
    this->_stone_tx.loadFromFile("./texture/stone.png");
    this->_stone_black_tx.loadFromFile("./texture/stone_black.png");
    this->_stone_white_tx.loadFromFile("./texture/stone_white.png");
    this->_hand_tx.loadFromFile("./texture/hand.png");
    this->_goban_tile_tx.loadFromFile("./texture/goban_tile.png");
    this->_goban_tile_tx.setRepeated(true);
    this->_background_tile_tx.loadFromFile("./texture/background_tile.png");
    this->_background_tile_tx.setRepeated(true);
    
    this->_background_sp.setTexture(this->_background_tile_tx);
    this->_goban_sp.setTexture(this->_goban_tile_tx);
    this->_curs_sp.setTexture(this->_hand_tx);
    this->resize();
}

Goban::~Goban() {
}

bool Goban::getInput(EventManager& events) {
    sf::Event current;
    if (this->_mainWindow.isOpen() == false)
        return false;
    while (this->_mainWindow.pollEvent(current) == true)
    {
        switch (current.type) {
            case sf::Event::KeyPressed:
                this->handleKeys(current, events);
                break;
            case sf::Event::Closed:
                events.setKey(EventManager::E_KEYS::QUIT);
                break;
            default:
                break;
        }
    }
    return true;
}

// Members
bool Goban::refresh() {
    this->drawAll();
    this->_mainWindow.display();    
    return true;
}

bool Goban::drawFrame(char c, const Rectangle& rect) {
    return false;
}

bool Goban::drawAll() {
    Stone::E_COLOR c;
    sf::Sprite stone;
    std::list<sf::Sprite> stones;
    const Stone::E_COLOR* map = this->_m.displayMap();    
    
    stone.setOrigin(this->_stone_white_tx.getSize().x / 2,
            this->_stone_white_tx.getSize().y / 2);
    stone.setScale(this->_SQUARE_SIZE.x / this->_stone_white_tx.getSize().x,
            this->_SQUARE_SIZE.y / this->_stone_white_tx.getSize().y);
    
    for (int y = 0; y < this->_map_size_y; ++y) {
        for (int x = 0; x < this->_map_size_x; ++x) {
            c = map[(y * this->_map_size_x) + x];
            if (c == Stone::E_COLOR::NONE)
                continue;
            stones.push_back(stone);
            if (c == Stone::E_COLOR::BLACK)
                stones.back().setTexture(this->_stone_black_tx);
            else
                stones.back().setTexture(this->_stone_white_tx);
            stones.back().setPosition(
                    this->_OFFSET_X + x * this->_SQUARE_SIZE.x,
                    this->_OFFSET_Y + y * this->_SQUARE_SIZE.y);
        }
    }
    this->_mainWindow.clear(sf::Color::Black);
    this->_mainWindow.draw(this->_background_sp);
    this->_mainWindow.draw(this->_goban_sp);
    for (std::list<sf::Sprite>::const_iterator it = stones.begin(); it != stones.end(); ++it) {
        this->_mainWindow.draw(*it);
    }

    this->_curs_sp.setPosition(
            this->_OFFSET_X + this->_curs.X * this->_SQUARE_SIZE.x,
            this->_OFFSET_Y + this->_curs.Y * this->_SQUARE_SIZE.y);
    this->_mainWindow.draw(this->_curs_sp);

    return true;
}

bool Goban::cursorUp() {
    this->_curs.Y--;
    if (this->_curs.Y < 0)
        this->_curs.Y = this->_map_size_y - 1;
    this->drawAll();
    return true;
}

bool Goban::cursorDown() {
    this->_curs.Y++;
    if (this->_curs.Y >= this->_map_size_y)
        this->_curs.Y = 0;
    this->drawAll();
    return true;
}

bool Goban::cursorLeft() {
    this->_curs.X--;
    if (this->_curs.X < 0)
        this->_curs.X = this->_map_size_x - 1;
    this->drawAll();
    return true;
}

bool Goban::cursorRight() {
    this->_curs.X++;
    if (this->_curs.X >= this->_map_size_x)
        this->_curs.X = 0;
    this->drawAll();
    return true;
}

bool Goban::cursorMouse(int pos_x, int pos_y) {
    return false;
}

bool Goban::newWindow(const Rectangle& rect, const std::string& msg) {
    return false;
}

/* PRIVATES */

bool Goban::handleKeys(const sf::Event& current, EventManager& events) {
    switch (current.key.code) {
        case (sf::Keyboard::Up):
            events.setKey(EventManager::E_KEYS::UP);
            break;
        case (sf::Keyboard::Down):
            events.setKey(EventManager::E_KEYS::DOWN);
            break;
        case (sf::Keyboard::Left):
            events.setKey(EventManager::E_KEYS::LEFT);
            break;
        case (sf::Keyboard::Right):
            events.setKey(EventManager::E_KEYS::RIGHT);
            break;
        case (sf::Keyboard::Return):
            events.setKey(EventManager::E_KEYS::ACCEPT);
            break;
        case (sf::Keyboard::Escape):
            events.setKey(EventManager::E_KEYS::QUIT);
            break;
        case (sf::Keyboard::Z):
        case (sf::Keyboard::W):
            events.setKey(EventManager::E_KEYS::BLACK);
            break;
        case (sf::Keyboard::X):
            events.setKey(EventManager::E_KEYS::WHITE);
            break;
        default:
            break;
    }
    return true;
}

void Goban::resize() {
    sf::Vector2f target_size = this->_mainWindow.getView().getSize();

    this->_background_sp.setTextureRect(sf::IntRect(0, 0, target_size.x, target_size.y));

    sf::Vector2u size = this->_goban_tile_tx.getSize();
    this->_goban_sp.setTextureRect(sf::IntRect(0, 0, size.x * (Map::_MAPSIZE_X - 1), size.y * (Map::_MAPSIZE_Y - 1)));

    this->_goban_sp.setScale(target_size.x / this->_goban_sp.getLocalBounds().width * 0.90,
            target_size.y / this->_goban_sp.getLocalBounds().height * 0.90);
    this->_goban_sp.setPosition(
            target_size.x / 2 - this->_goban_sp.getGlobalBounds().width / 2,
            target_size.y / 2 - this->_goban_sp.getGlobalBounds().height / 2 - 25);
    
    this->_curs_sp.setScale(0.75 * target_size.x / this->_goban_sp.getLocalBounds().width
            ,0.75 * target_size.y / this->_goban_sp.getLocalBounds().height);
    
//    this->_OFFSET_X = 57 * this->_goban_sp.getScale().x;
//    this->_OFFSET_Y = 57 * this->_goban_sp.getScale().y;
    this->_OFFSET_X = this->_goban_sp.getGlobalBounds().left;
    this->_OFFSET_Y = this->_goban_sp.getGlobalBounds().top;
    this->_SQUARE_SIZE = sf::Vector2f(
            this->_goban_tile_tx.getSize().x * this->_goban_sp.getScale().x,
            this->_goban_tile_tx.getSize().y * this->_goban_sp.getScale().y);
}