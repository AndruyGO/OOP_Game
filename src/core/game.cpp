#include <random>
#include <set>
#include <algorithm>
#include "game.hpp"
#include "game_field.hpp"
#include "direction.hpp"


Game::Game(const GameField &game_field,
    const PlayerRobot &player_robot,
    const std::vector<EnemyRobot> &robots)
  : game_field_(game_field),
    player_robot_(player_robot),
    robots_(robots.begin(), robots.end()),
    is_players_move_(true),
    game_status_(GameStatuses::kGameGoing) {

        game_field_.OpenCells(game_field_.PlayerStartPosition(), 
                              player_robot_.VisibilityRadius());

        player_robot_.SetPosition(game_field_.PlayerStartPosition());
    }

const GameField& Game::Field() const { return game_field_; }
const PlayerRobot& Game::Player() const { return player_robot_; }
const std::list<EnemyRobot>& Game::Robots() const { return robots_; }

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
bool Game::TryMove(Robot &robot, Position next_position) {
    if(game_field_.IsInside(next_position) && 
        game_field_.GetCell(next_position).Passable() &&
        game_field_.GetCell(next_position).MovementCost() <= robot.MovesRemain()){
        
        auto another_robot = RobotOnPosition(next_position);
        if(another_robot == nullptr){
            robot.SetPosition(next_position);
        }else{
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


void Game::SetGameStatus(GameStatuses status){
    game_status_ = status;
}

Game::GameStatuses Game::GameStatus() const {
    return game_status_;
}