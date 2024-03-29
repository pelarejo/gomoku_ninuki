#include "Menu.h"
#include "Exceptions.h"
#include "Game.h"

static int selected_boolean(bool field) {
    return (field) ? 2 : 1;
}

Menu::Menu(sf::RenderWindow& mainWindow, std::string const& title)
    : AGui(0, 0, AGui::GOBAN), _mainWindow(mainWindow), _selected(0),
    _config()
{
    _mainWindow.clear(sf::Color::Black);
    if (!_font.loadFromFile("./font/arial.ttf"))
        throw Exceptions("Can't load arial font");
    _title = new GText(title, &_font, GText::TITLE);

    std::vector<std::string> playerV;
    playerV.push_back("AI");
    playerV.push_back("HUMAN");
    std::vector<bool> boolV;
    boolV.push_back(false);
    boolV.push_back(true);
    std::vector<float> timeV;
    timeV.push_back(0.5);
    timeV.push_back(1);
    timeV.push_back(3);
    timeV.push_back(5);
    timeV.push_back(10);
    timeV.push_back(20);
    timeV.push_back(30);
    timeV.push_back(40);
    timeV.push_back(50);
    timeV.push_back(60);

    _options.push_back(new Options("Continue ?", _mainWindow, &_font, selected_boolean(_config.continue_game)));
    _options[0]->addValues<bool>(boolV);
    _options.push_back(new Options("Player1 (Black)", _mainWindow, &_font, selected_boolean(_config.human_player_1)));
    _options[1]->addValues<std::string>(playerV);
    _options.push_back(new Options("Player2 (White)", _mainWindow, &_font, selected_boolean(_config.human_player_2)));
    _options[2]->addValues<std::string>(playerV);
    int pos = find(timeV.begin(), timeV.end(), _config.ai_time) - timeV.begin();
    _options.push_back(new Options("AI Time", _mainWindow, &_font, pos + 1));
    _options[3]->addValues<float>(timeV);
    _options.push_back(new Options("Five-Break", _mainWindow, &_font, selected_boolean(_config.fivebreak_rule)));
    _options[4]->addValues<bool>(boolV);
    _options.push_back(new Options("Double-Three", _mainWindow, &_font, selected_boolean(_config.doublethree_rule)));
    _options[5]->addValues<bool>(boolV);
    _options.push_back(new Options("Help", _mainWindow, &_font, selected_boolean(_config.help)));
    _options[6]->addValues<bool>(boolV);

    //if debug : push + d'options

    int i = 1;
    std::for_each(_options.begin(), _options.end(), [&](Options * option){
        option->setY(_mainWindow.getSize().y * i++ / (_options.size() + 1));
    });
    setSelected(0);
//    _options[_selected]->focus();
}

Menu::~Menu() {
    delete _title;
    _options.clear();
}

Config const&        Menu::config() {
    saveConfig();
    _config.debug_config();
    return _config;
}
bool Menu::chooseOptionValue() {
    validate();
    drawAll();
    return true;
}
void    Menu::setTitle(std::string const& title) {
    _title->setString(title);
    drawAll();
}
std::vector<Options*> const&  Menu::getOptions() {
    return _options;
}

/* Getters */

bool Menu::getInput(EventManager& events) {
    sf::Event current;
    if (_mainWindow.isOpen() == false)
        return false;
    while (_mainWindow.pollEvent(current) == true)
    {
        switch (current.type) {
            case sf::Event::KeyPressed:
                handleKeys(current, events);
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

/* Members */

bool Menu::refresh() {
    _mainWindow.display();
    return true;
}

bool Menu::drawFrame(char c, const Rectangle& rect) { // Not used
    return false;
}

bool Menu::drawAll() { // This is general drawing func
    _mainWindow.clear(sf::Color::Black);
    _mainWindow.draw(*_title);
    for (std::vector<Options *>::iterator it = _options.begin(); it < _options.end(); ++it)
        (*it)->draw();
    return true;
}

bool Menu::cursorUp() {
    selectUp();
    drawAll();
    return true;
}

bool Menu::cursorDown() {
    selectDown();
    drawAll();
    return true;
}

bool Menu::cursorLeft() {
    selectLeft();
    drawAll();
    return true;
}

bool Menu::cursorRight() {
    selectRight();
    drawAll();
    return true;
}

bool Menu::cursorMouse(int pos_x, int pos_y) {
    // la fleeemmmmeeee
    return false;
}


bool Menu::newWindow(const Rectangle& rect, const std::string& msg) {
    return false;
}

/* PRIVATES */

void    Menu::setSelected(unsigned int i)
{
    if (i >= 0 && i < _options.size())
    {
        _options[_selected]->unfocus();
        _selected = i;
        _options[_selected]->focus();
    }
}
void    Menu::selectUp()
{
    setSelected(_selected - 1);
}
void    Menu::selectDown()
{
    setSelected(_selected + 1);
}
void    Menu::selectLeft()
{
    selectedOption()->selectLeft();
}
void    Menu::selectRight()
{
    selectedOption()->selectRight();
}
void    Menu::validate()
{
    selectedOption()->validate();
}

Options*     Menu::selectedOption() const {
    assert(_options.size() > 0);
    assert(_selected >= 0 && _selected < _options.size());
    return _options[_selected];
}
void Menu::saveConfig()
{
    assert(_options.size() == 7);
    _config.continue_game = _options[0]->validatedValue<bool>();
    _config.human_player_1 = (_options[1]->validatedValue<std::string>() == "HUMAN");
    _config.human_player_2 = (_options[2]->validatedValue<std::string>() == "HUMAN");
    _config.ai_time = _options[3]->validatedValue<float>();
    _config.fivebreak_rule = _options[4]->validatedValue<bool>();
    _config.doublethree_rule = _options[5]->validatedValue<bool>();
    _config.help = _options[6]->validatedValue<bool>();
}

bool Menu::handleKeys(const sf::Event& current, EventManager& events) {
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
