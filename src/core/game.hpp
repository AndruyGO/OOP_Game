#ifndef GAME_H
#define GAME_H

#include "game_field.hpp"
#include "../entities/player_robot.hpp"
#include "../entities/enemy_robot.hpp"
#include "../entities/robot_factory.hpp"
#include "commands.hpp"
#include "direction.hpp"
#include "../abilities/ability.hpp"
#include <vector>
#include <list>

class Game {
private:
    GameField game_field_;
    PlayerRobot player_robot_;
    std::list<EnemyRobot> robots_;
    std::list<RobotFactory> factories_;
    bool is_players_move_;
    int max_id_;
public:
    enum class GameStatuses{
        kGameOver,
        kGamePassed,
        kGameGoing
    };
private:
    GameStatuses game_status_;

    public:

    Game(GameField game_field,
    PlayerRobot player_robot,
    std::vector<EnemyRobot> robots,
    std::vector<RobotFactory> factories);
    ~Game() = default;
    
    Game(Game&&) = default;
    Game& operator=(Game&&) = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void Move(Command cmd);

    const GameField& Field() const;
    const PlayerRobot& Player() const;
    PlayerRobot& Player();
    const std::list<EnemyRobot>& Robots() const;
    const std::list<RobotFactory>& Factories() const;

    bool ProcessPlayerCommans(Command cmd);
    bool TryMove(Robot &robot, Direction direction);
    void RobotMove(EnemyRobot &robot);
    void RobotsMove();
    bool TryMove(Robot &robot, Position next_position);
    Robot *RobotOnPosition(Position position);
    GameStatuses GameStatus() const;

    bool IsPlayersMove() const;
    void SwitchMove();
    bool IsWin() const;
    
    void Kill(Robot *robot);
    void SetGameStatus(GameStatuses status);
    void BuildingsMove();
    
    RobotFactory *FactoryOnPosition(Position position);
    bool SpawnRobot(EnemyRobot robot, Position pos);
    int GetId();

    void InteractWithRobot(Robot &actor, Robot &target);
    void InteractWithBuilding(Robot &actor, Building &target);
    
    void AddAbility(Robot &robot, Ability::AbilityType ability_type);
    void UseAbility(Robot &robot, Ability::AbilityType ability_type);
    
};

#endif