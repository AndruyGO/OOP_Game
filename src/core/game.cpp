#include <random>
#include <set>
#include <algorithm>
#include "game.hpp"
#include "game_field.hpp"
#include "direction.hpp"

#include <iostream>


Game::Game(const GameField &game_field,
    const PlayerRobot &player_robot,
    const std::vector<EnemyRobot> &robots,
    const std::vector<RobotFactory> &factories)
  : game_field_(game_field),
    player_robot_(player_robot),
    robots_(robots.begin(), robots.end()),
    factories_(factories.begin(), factories.end()),
    is_players_move_(true),
    game_status_(GameStatuses::kGameGoing) {

        game_field_.OpenCells(game_field_.PlayerStartPosition(), 
                              player_robot_.VisibilityRadius());

        player_robot_.SetPosition(game_field_.PlayerStartPosition());

        max_id_ = std::max(max_id_, player_robot.Id());
        for(auto &robot : robots){
            max_id_ = std::max(max_id_, robot.Id());
        }
        for(auto &factory : factories){
            max_id_ = std::max(max_id_, factory.Id());
        }
        max_id_++;
    }

const GameField& Game::Field() const { return game_field_; }
const PlayerRobot& Game::Player() const { return player_robot_; }
const std::list<EnemyRobot>& Game::Robots() const { return robots_; }
const std::list<RobotFactory>& Game::Factories() const { return factories_; }

bool Game::IsPlayersMove() const {
    return is_players_move_;
}
void Game::SwitchMove() {
    is_players_move_ = !is_players_move_;
}

bool Game::ProcessPlayerCommans(Command cmd) {
    bool is_executed = 0;
    switch (cmd) {
        case Command::kMoveUp:
            is_executed = TryMove(player_robot_, Direction::kUp);
            break;
        case Command::kMoveDown:
            is_executed = TryMove(player_robot_, Direction::kDown);
            break;
        case Command::kMoveLeft:
            is_executed = TryMove(player_robot_, Direction::kLeft);
            break;
        case Command::kMoveRight:
            is_executed = TryMove(player_robot_, Direction::kRight);
            break;
        case Command::kWait:
            is_executed = 1;
            break;
        case Command::kQuit:
            is_executed = 1;
            break;
        case Command::kNone:
            break;
    }
    if (is_executed) {
        player_robot_.ReduceMovesRemain(game_field_.GetCell(player_robot_.NowPosition()).MovementCost());
        game_field_.OpenCells(player_robot_.NowPosition(),
                              player_robot_.VisibilityRadius());
    }
    if(player_robot_.MovesRemain() == 0) SwitchMove();
    return is_executed;
}

Robot *Game::RobotOnPosition(Position position) {
    if (player_robot_.NowPosition() == position)
        return &player_robot_;
    for (auto& robot : robots_) {
        if (robot.NowPosition() == position)
            return &robot;
    }
    return nullptr;
}

RobotFactory *Game::FactoryOnPosition(Position position) {
    
    for (auto& factory : factories_) {
        if ((factory.TopLeftPosition().X() <= position.X() &&
            position.X() <= factory.TopLeftPosition().X() + factory.Size() - 1) &&
            (factory.TopLeftPosition().Y() <= position.Y() &&
            position.Y() <= factory.TopLeftPosition().Y() + factory.Size() - 1))

            return &factory;
    }
    return nullptr;
}

bool Game::TryMove(Robot &robot, Position next_position) {
    auto factory = FactoryOnPosition(next_position);

    if(game_field_.IsInside(next_position) && 
        game_field_.GetCell(next_position).Passable() &&
        game_field_.GetCell(next_position).MovementCost() <= robot.MovesRemain() &&
        factory == nullptr){
        
        auto another_robot = RobotOnPosition(next_position);
        if(another_robot == nullptr && factory == nullptr){
            robot.SetPosition(next_position);
        }else if(another_robot != nullptr){
            robot.Interact(*another_robot);
            if(another_robot->NowHealth() <= 0) {
                if(robot.Id() == 1){
                    player_robot_.AddNowXP(25);
                    player_robot_.RankUp();
                }
                Kill(another_robot);
            }
        }
        return true;
    }else{
        return false;
    }
}

bool Game::TryMove(Robot &robot, Direction direction) {
    Position next_position(robot.NowPosition());
    switch (direction) {
        case Direction::kUp:    next_position.Up();    break;
        case Direction::kDown:  next_position.Down();  break;
        case Direction::kLeft:  next_position.Left();  break;
        case Direction::kRight: next_position.Right(); break;
        default: return false;
    }
    return TryMove(robot, next_position);

}
void Game::Kill(Robot *robot){
    if(robot == nullptr) return;

    if((*robot).Id() == player_robot_.Id()){
        SetGameStatus(GameStatuses::kGameOver);
    }else{
        robots_.erase(std::find(robots_.begin(), robots_.end(), *robot));
        if(robots_.empty()){
            SetGameStatus(GameStatuses::kGamePassed);
        }
    }
}


void Game::RobotMove(EnemyRobot& robot) {
    static std::mt19937 rng(std::random_device{}());

    std::vector<Position> allowed_positions;

    for (Direction dir : {Direction::kUp, Direction::kDown,
                          Direction::kLeft, Direction::kRight}) {
        Position next = robot.NowPosition().MoveToDirection(dir);
        if (!game_field_.IsInside(next))
            continue;
        if (!game_field_.GetCell(next).Passable())
            continue;
        allowed_positions.push_back(next);
    }

    if (allowed_positions.empty()) {
        robot.SetMovesRemain(0);
        return;
    }
    
    std::uniform_int_distribution<int> dist(0, allowed_positions.size() - 1);
    TryMove(robot, allowed_positions[dist(rng)]);
    robot.ReduceMovesRemain(game_field_.GetCell(allowed_positions[dist(rng)]).MovementCost());
}

void Game::RobotsMove() {
    BuildingsMove();

    for (auto& robot : robots_) {
        robot.RecoverMoves();
        robot.AddNowEnergy(25);
        while(robot.MovesRemain() > 0)
            RobotMove(robot);
    }
    SwitchMove();
    player_robot_.AddNowEnergy(25);
    player_robot_.RecoverMoves();
}

int Game::GetId() {
    return ++max_id_;
}

void Game::SetGameStatus(GameStatuses status){
    game_status_ = status;
}

Game::GameStatuses Game::GameStatus() const {
    return game_status_;
}




void Game::BuildingsMove() {
    for(auto &factory : factories_){

        factory.Move();
        if(factory.RobotsInside() > 0){
            bool is_spawned = SpawnRobot(EnemyRobot(GetId(), 100, 100, 50, 0, 1), factory.TopLeftPosition().Left());
            if(is_spawned) factory.ReduceRobotsInside(1);
        }
    }
}

bool Game::SpawnRobot(EnemyRobot robot, Position pos) {
    if(game_field_.GetCell(pos).Passable() == 1 && RobotOnPosition(pos) == nullptr &&
        FactoryOnPosition(pos) == nullptr) {
        
        robots_.push_back(robot);
        robots_.back().SetPosition(pos);
        return 1;
        }

    return 0;
}