#include "AI.h"
#include "Exceptions.h"
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */


AI::AI(Map& map, Referee& referee, Stone::E_COLOR color)
    : APlayer(color, APlayer::AI), _map(map), _referee(referee) {
}

AI::~AI() {

}


//Members
void AI::setTimeLimit(float t)
{
    _timeLimit = t;
}
void AI::setOpponent(APlayer * player) {
    _opponent = player;
    _opColor = player->getColor();
}


// On vérifie si on a pas dépassé le temps maximal
inline void        AI::checkTime() {
    if (((float)clock() / CLOCKS_PER_SEC) - (_playBeginTime / CLOCKS_PER_SEC) > _timeLimit)
        throw Exceptions("AI timeLimitSec exceeded");
}

Stone AI::plays() {
    Stone stone(-1, -1, _color);
    int depth = 1;

    try
    {
        while (true)
        {
            _playBeginTime = (float)clock();
            stone = calc(depth++);
        }
    }
    catch (const Exceptions& e)
    {
        depth -= 1;
    }

    if (stone.x() == -1)
        throw Exceptions("AI CAN'T PLAY :(");
    std::cout << "AI depth " << depth-1 << std::endl;
    return stone;
}


// int AI::eval(Map& map, Referee::E_STATE ret, char captured, char opponentCaptured) {
//     char stonesPlayed = map.getPlayed();

//     if (ret == Referee::E_STATE::END_WHITE)
//         return _color == Stone::E_COLOR::WHITE ? 1000 - stonesPlayed : -1000 + stonesPlayed;
//     if (ret == Referee::E_STATE::END_BLACK)
//         return _color == Stone::E_COLOR::BLACK ? 1000 - stonesPlayed : -1000 + stonesPlayed;

//     int takenStones = captured - _captured;
//     int opponentTakenStones = opponentCaptured - _opponent->getCaptured();
//     if (takenStones)
//         return 100 * takenStones - stonesPlayed;
//     if (opponentTakenStones)
//         return -100 * opponentTakenStones + stonesPlayed;
//     return (0);
// }

// int AI::eval(Map& map, Referee::E_STATE ret) {
//     char stonesPlayed = map.getPlayed();

//     if (ret == Referee::E_STATE::END_WHITE)
//         return _color == Stone::E_COLOR::WHITE ? 1000 - stonesPlayed : -1000 + stonesPlayed;
//     if (ret == Referee::E_STATE::END_BLACK)
//         return _color == Stone::E_COLOR::BLACK ? 1000 - stonesPlayed : -1000 + stonesPlayed;

//     int takenStones = map.getCapturedBy(_color) - _captured;
//     int opponentTakenStones = map.getCapturedBy(_opColor) - _opponent->getCaptured();
//     if (takenStones)
//         return 100 * takenStones - stonesPlayed;
//     if (opponentTakenStones)
//         return -100 * opponentTakenStones + stonesPlayed;
//     return (0);
// }

// int AI::calcMax(Map& map, int depth, Referee::E_STATE ret, int alpha, int beta) {

//     if (depth == 0 || Referee::gameHasEnded(ret))
//         return eval(map, ret);

//     int score;
//     //int max = -AI_INFINITY;

//     //On parcourt les cases du Goban
//     for (int y = 0; y < Map::_MAPSIZE_Y; ++y)
//     {
//         for (int x = 0; x < Map::_MAPSIZE_X; ++x)
//         {
//             checkTime();

//             if (map[y][x].isEmpty())
//             {
//                 // Copy de la map et des nombres de pierres capturées
//                 Map map_tmp = map;

//                 // On crée la pierre et on joue le coup
//                 char fake = 0;
//                 Referee::E_STATE ret = _referee.check(Stone(y, x, _color), map_tmp, fake);

//                 // Si le coup est valide on évalue (Pas de double trois)
//                 if (ret != Referee::E_STATE::INVALID)
//                 {
//                     score = calcMin(map_tmp, depth-1, ret, alpha, beta);

//                     // Si le score est plus petit on le sauvegarde
//                     // if (score > max)
//                     //     max = score;
//                     if (score > alpha)
//                     {
//                         alpha = score;
//                         if (alpha >= beta)
//                             return alpha;
//                     }
//                 }
//                 // On annule le coup (Ici rien pour l'instant car copie de la Map et des éléments)
//             }
//         }
//     }
//     //return max;
//     return alpha;
// }

// int AI::calcMin(Map& map, int depth, Referee::E_STATE ret, int alpha, int beta) {

//     if (depth == 0 || Referee::gameHasEnded(ret))
//         return eval(map, ret);

//     int score;
//     //int min = AI_INFINITY;

//     //On parcourt les cases du Goban
//     for (int y = 0; y < Map::_MAPSIZE_Y; ++y)
//     {
//         for (int x = 0; x < Map::_MAPSIZE_X; ++x)
//         {
//             checkTime();

//             if (map[y][x].isEmpty())
//             {
//                 // Copy de la map et des nombres de pierres capturées
//                 Map map_tmp = map;

//                 // On crée la pierre et on joue le coup
//                 char fake = 0;
//                 Referee::E_STATE ret = _referee.check(Stone(y, x, _opColor), map_tmp, fake);

//                 // Si le coup est valide on évalue (Pas de double trois)
//                 if (ret != Referee::E_STATE::INVALID)
//                 {
//                     score = calcMax(map_tmp, depth-1, ret, alpha, beta);

//                     // Si le score est plus petit on le sauvegarde
//                     // if (score < min)
//                     //     min = score;
//                     if (score < beta)
//                     {
//                         beta = score;
//                         if (beta <= alpha)
//                             return beta;
//                     }
//                 }
//                 // On annule le coup (Ici rien pour l'instant car copie de la Map et des éléments)
//             }
//         }
//     }
//     //return min;
//     return beta;
// }


int AI::eval(Map& map, Referee::E_STATE ret, Stone::E_COLOR color) {
    char stonesPlayed = map.getPlayed();

    if (ret == Referee::E_STATE::END_WHITE)
        return _color == Stone::E_COLOR::WHITE ? 1000 - stonesPlayed : -1000 + stonesPlayed;
    if (ret == Referee::E_STATE::END_BLACK)
        return _color == Stone::E_COLOR::BLACK ? 1000 - stonesPlayed : -1000 + stonesPlayed;


    int takenStones =  map.getCapturedBy(color) - (color == _color ? _captured : _opponent->getCaptured());
    int opponentTakenStones = map.getCapturedBy(Referee::OP_COLOR[color]) - (color == _color ? _captured : _opponent->getCaptured());
    if (takenStones)
        return 100 * takenStones - stonesPlayed;
    if (opponentTakenStones)
        return -100 * opponentTakenStones + stonesPlayed;
    return (0);
}

int AI::calcMinMax(Map& map, int depth, Referee::E_STATE ret, Stone::E_COLOR color, int alpha, int beta) {

    if (depth == 0 || Referee::gameHasEnded(ret))
        return eval(map, ret, color);

    int score;
    int current = -AI_INFINITY;
    for (int y = 0; y < Map::_MAPSIZE_Y; ++y)
    {
        for (int x = 0; x < Map::_MAPSIZE_X; ++x)
        {
            checkTime();

            if (map[y][x].isEmpty())
            {
                // Copy de la map et des nombres de pierres capturées
                Map map_tmp = map;

                // On crée la pierre et on joue le coup
                char fake = 0;
                Referee::E_STATE ret = _referee.check(Stone(y, x, color), map_tmp, fake);

                // Si le coup est valide on évalue (Pas de double trois)
                if (ret != Referee::E_STATE::INVALID)
                {
                    score = -calcMinMax(map_tmp, depth-1, ret, Referee::OP_COLOR[color], -beta, -alpha);

                    if (score >= current)
                    {
                        current = score;
                        if (score >= alpha)
                        {
                            alpha = score;
                            if (/*alpha*/score >= beta)
                                return alpha;
                        }
                    }
                }
            }
        }
    }
    return /*alpha*/current;
}

Stone AI::calc(int depth) {
    int score;
    int max_y=-1,max_x=-1;
    //int max = -AI_INFINITY;
    int alpha = -AI_INFINITY;
    int beta = AI_INFINITY;

    //On parcourt les cases du Goban
    for (int y=0; y<Map::_MAPSIZE_Y; ++y)
    {
        for (int x=0; x<Map::_MAPSIZE_X; ++x)
        {
            checkTime();

            if (_map[y][x].isEmpty())
            {
                // Copy de la map et des nombres de pierres capturées
                Map map_tmp = _map;

                // On crée la pierre et on joue le coup
                char fake = 0;
                Referee::E_STATE ret = _referee.check(Stone(y, x, _color), map_tmp, fake);

                // Si le coup est valide on évalue (Pas de double trois)
                if (ret != Referee::E_STATE::INVALID)
                {
                    //score = calcMin(map_tmp, depth - 1, ret, alpha, beta);
                    score = -calcMinMax(map_tmp, depth - 1, ret, _opColor, -beta, -alpha);

                    // Si ce score est plus grand
                    //if (score > max)/*Moins optimise mais aleatoire: if (score > max || (score == max && rand()%2))*/
                    if (score > alpha)
                    {
                        std::cout << "depth:"<<depth<<"[" << alpha << "("<<max_y<<";"<<max_x<<")->" << score << "("<<y<<";"<<x<< ")]" << std::endl;
                        //On le choisit
                        //max = score;
                        alpha = score;
                        // On sauvegarde les coordonnées du coup optimum
                        max_y = y;
                        max_x = x;
                    }
                }
                // On annule le coup (Ici rien pour l'instant car copie de la Map et des éléments)
            }
        }
    }
    // On retourne la pierre optimale
    return Stone(max_y, max_x, _color);
}
