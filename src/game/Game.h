#pragma once
#include "Map.h"
#include "Core.h"
#include "AGui.h"
#include "APlayer.h"
#include "Referee.h"
#include "InitSfml.h"
#include <map> 

struct Config {
    bool doublethree_rule;
    bool fivebreak_rule;
    int ai_player_pos; // Set -1 if no ai
};

class Game {
public:
    Game();
    virtual ~Game();
    
    // Members
    int start();
    int restart();

private:
    Map         _map;
    Core        _core;
    InitSfml    _init_sfml;
    AGui*       _guis[2];
    AGui*       _gui; // Current Gui
    APlayer *   _currentPlayer;
    APlayer *   _players[2];
    int         _player_nb;
    Config      _conf;
    Referee     _referee;
    
    void initPlayers();
    inline void nextPlayer();
    void accept();
};

