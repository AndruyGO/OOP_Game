#include "io/application.hpp"
#include "core/game.hpp"
#include "core/game_field.hpp"
#include "entities/player_robot.hpp"
#include "entities/enemy_robot.hpp"

#include <vector>
#include <iostream>

int main() {
    std::vector<std::vector<int>> type_map = 
    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 0, 0, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 2, 2, 1, 1, 1, 1, 1, 0},
     {0, 1, 2, 2, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    std::vector<std::vector<bool>> passage_map = 
    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 0, 0, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    std::vector<std::vector<int>> movement_costs_map = 
    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 2, 2, 1, 1, 1, 1, 1, 0},
     {0, 1, 2, 2, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
     
    

    GameField field(type_map, passage_map, movement_costs_map, Position(2, 2));
    field.AddFrame();
    PlayerRobot player(1, 100, 100, 50, 1, 3, 1, 3);
    player.SetPosition(field.PlayerStartPosition());

    std::vector<EnemyRobot> enemies;
    enemies.emplace_back(2, 100, 100, 50, false, 1);
    enemies.back().SetPosition({7, 7});

    Game game(field, player, enemies);
    Application app(game);
    app.Run();
    return 0;
}