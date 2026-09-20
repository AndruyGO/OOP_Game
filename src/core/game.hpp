#ifndef GAME_H
#define GAME_H

#include "game_field.hpp"
#include "../entities/player_robot.hpp"
#include "../entities/enemy_robot.hpp"
#include "commands.hpp"
#include "direction.hpp"
#include <vector>
#include <list>

class Game {
private:
    GameField game_field_;
    PlayerRobot player_robot_;
    std::list<EnemyRobot> robots_;
    bool is_players_move_;

public:
    enum class GameStatuses{
        kGameOver,
        kGamePassed,
        kGameGoing
    };
private:
    GameStatuses game_status_;

    public:

    Game(const GameField &game_field,
    const PlayerRobot &player_robot,
    const std::vector<EnemyRobot> &robots);
    ~Game() = default;
    
    const GameField& Field() const;
    const PlayerRobot& Player() const;
    const std::list<EnemyRobot>& Robots() const;

    bool ProcessPlayerCommans(Command cmd);
    bool TryMove(Robot &robot, Direction direction);
    void RobotMove(EnemyRobot &robot);
    void RobotsMove();
    bool TryMove(Robot &robot, Position next_position);
    Robot *RobotOnPosition(Position position);
    GameStatuses GameStatus() const;

    bool IsPlayersMove() const;
    void SwitchMove();
    
    void Kill(Robot *robot);
    void SetGameStatus(GameStatuses status);

};

#endif