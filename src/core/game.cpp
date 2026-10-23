#include <random>
#include <set>
#include <queue>
#include <algorithm>
#include "game.hpp"
#include "game_field.hpp"
#include "direction.hpp"
#include <utility>

#include <iostream>
#include "../abilities/area_strike.hpp"
#include "../abilities/heal.hpp"
#include "../abilities/far_hit.hpp"
#include "../abilities/teleport.hpp"


Game::Game(GameField game_field,
    PlayerRobot player_robot,
    std::vector<EnemyRobot> robots,
    std::vector<RobotFactory> factories)
  : game_field_(std::move(game_field)),
    player_robot_(std::move(player_robot)),
    robots_(std::make_move_iterator(robots.begin()),
            std::make_move_iterator(robots.end())),
    factories_(factories.begin(), factories.end()),
    is_players_move_(true),
    game_status_(GameStatuses::kGameGoing),
    input_status_(Game::InputStatuses::kStandard) {

        game_field_.OpenCells(game_field_.PlayerStartPosition(), 
                              player_robot_.VisibilityRadius());
        ApplyStatuses(player_robot_, Status::StatusType::kNone, Status::StatusGroup::kBeforeMove);
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


void Game::Move(InputContainer cmd) {
    if(player_robot_.NowHealth() <= 0) {
        SetGameStatus(Game::GameStatuses::kGameOver);
        return;
    }

    if(GameStatus() == Game::GameStatuses::kGameGoing) {
        if(IsPlayersMove()){
            if(cmd.command == Command::kNone) return;
            ProcessPlayerCommans(cmd); 
        } else {
            ApplyStatuses(player_robot_, Status::StatusType::kNone, Status::StatusGroup::kAfterMove);
            RobotsMove();
            ApplyStatuses(player_robot_, Status::StatusType::kNone, Status::StatusGroup::kBeforeMove);
        }
    }
}

const GameField& Game::Field() const { return game_field_; }
const PlayerRobot& Game::Player() const { return player_robot_; }
PlayerRobot& Game::Player() { return player_robot_; }
const std::list<EnemyRobot>& Game::Robots() const { return robots_; }
const std::list<RobotFactory>& Game::Factories() const { return factories_; }

bool Game::IsPlayersMove() const {
    return is_players_move_;
}
void Game::SwitchMove() {
    is_players_move_ = !is_players_move_;
}

bool Game::ProcessPlayerCommans(InputContainer cmd) {
    bool is_executed = 0;

    if(InputStatus() == Game::InputStatuses::kStandard) {
    switch (cmd.command) {
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
            player_robot_.ReduceMovesRemain(1);
            break;
        case Command::kQuit:
            is_executed = 1;
            break;
        case Command::kNone:
            break;
        case Command::kUseAreaStrike:
            UseAbility(player_robot_, Ability::AbilityType::kAreaStrike); break;
        case Command::kUseHeal: 
            UseAbility(player_robot_, Ability::AbilityType::kHeal); break;
        case Command::kUseFarHit:
            if(!CanRobotUseAbility(player_robot_, Ability::AbilityType::kFarHit)) break;
            last_ability_ = Ability::AbilityType::kFarHit;
            input_status_ = Game::InputStatuses::kWaitingForMouseToAbility;
            break;
        case Command::kUseTeleport:
                if(!CanRobotUseAbility(player_robot_, Ability::AbilityType::kTeleport)) break;
                last_ability_ = Ability::AbilityType::kTeleport;
                input_status_ = Game::InputStatuses::kWaitingForMouseToAbility;
                break;
        case Command::kSelectTarget:
            //std::cout << cmd.field_mouse_position.X() << " " << cmd.field_mouse_position.Y() << std::endl;
        default:
            break;
    }
    if (is_executed) {
        game_field_.OpenCells(player_robot_.NowPosition(),
                              player_robot_.VisibilityRadius());
    }
    if(player_robot_.MovesRemain() == 0) SwitchMove();
    return is_executed;

    } else if(InputStatus() == Game::InputStatuses::kWaitingForMouseToAbility) {
        if(cmd.command == Command::kSelectTarget) {
            input_status_ = Game::InputStatuses::kStandard;
            UseAbility(player_robot_, last_ability_, cmd.field_mouse_position);
            game_field_.OpenCells(player_robot_.NowPosition(),
                              player_robot_.VisibilityRadius());
            return true;
        }else if(cmd.command == Command::kCancel) {
            input_status_ = Game::InputStatuses::kStandard;
            return true;
        }
    } else if(InputStatus() == Game::InputStatuses::kWaitingForRankUp) {
        // do nothing
    }


    return false;
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

    if(game_field_.IsInside(next_position) && 
        game_field_.GetCell(next_position).Passable() &&
        game_field_.GetCell(next_position).MovementCost() <= robot.MovesRemain()){

        auto factory = FactoryOnPosition(next_position);
        auto another_robot = RobotOnPosition(next_position);
        if(another_robot == nullptr && factory == nullptr){
            robot.SetPosition(next_position);
            robot.ReduceMovesRemain(game_field_.GetCell(next_position).MovementCost());
            AddStatus(robot, game_field_.GetCell(next_position).TakeRandomStatus());
        }else if(another_robot != nullptr){
            InteractWithRobot(robot, *another_robot);
            robot.ReduceMovesRemain(1);
        }else if(factory != nullptr) {
            InteractWithBuilding(robot, *factory);
            robot.ReduceMovesRemain(1);
        } else {
            return false;
        }
        return true;
    }else{
        return false;
    }
}

bool Game::TryMove(Robot &robot, Direction direction) {
    Position next_position(robot.NowPosition());
    switch (direction) {
        case Direction::kUp:    next_position.RePositionUp();    break;
        case Direction::kDown:  next_position.RePositionDown();  break;
        case Direction::kLeft:  next_position.RePositionLeft();  break;
        case Direction::kRight: next_position.RePositionRight(); break;
        default: return false;
    }
    return TryMove(robot, next_position);

}

void Game::InteractWithBuilding(Robot &actor, Building &target) {
    if(actor.IsFriendly() == target.IsFriendly()){
        target.Heal(actor.kHealAmount);
    }else{
        target.Hit(actor.Damage());
        if(target.NowHealth() <= 0) {
            if(actor.Id() == player_robot_.Id()){
                
            }
            Kill(&target);
        }
    }
}
void Game::InteractWithRobot(Robot &actor, Robot &target) {
    if(actor.IsFriendly() == target.IsFriendly()){
        target.Heal(actor.kHealAmount);
    }else{
        target.Hit(actor.Damage());
        if(GetStatus(target, Status::StatusType::kShield) != nullptr) {
            GetStatus(target, Status::StatusType::kShield)->DoAction(target);
        }
        if(target.NowHealth() <= 0) {
            if(actor.Id() == player_robot_.Id()){
                player_robot_.AddNowXP(25);
                RankUp();
            }
            
            Kill(&target);
        }
    }
    
}

void Game::Kill(Building *building) {
    if(building == nullptr) return;

    factories_.erase(std::find(factories_.begin(), factories_.end(), *building));
    if(IsWin()){
        SetGameStatus(GameStatuses::kGamePassed);
    }
}

void Game::Kill(Robot *robot){
    if(robot == nullptr) return;
    
    if((*robot).Id() == player_robot_.Id()){
        SetGameStatus(GameStatuses::kGameOver);
    }else{
        robots_.erase(std::find(robots_.begin(), robots_.end(), *robot));
        player_robot_.AddNowXP(50);
        RankUp();
        if(IsWin()){
            SetGameStatus(GameStatuses::kGamePassed);
        }
    }
}

bool Game::IsWin() const {
    for(auto& robot : robots_){
        if(robot.IsFriendly() == 0){
            return false;
        }
    }
    for(auto& factory : factories_){
        if(factory.IsFriendly() == 0){
            return false;
        }
    }
    return true;
}


void Game::RobotMove(EnemyRobot& robot) {
    // 1. Статусы до хода и проверка на смерть (остается как было)
    ApplyStatuses(robot, Status::StatusType::kNone, Status::StatusGroup::kBeforeMove);
    if(robot.NowHealth() <= 0) {
        if(!robot.IsFriendly()) {
            player_robot_.AddNowXP(50);
            RankUp(); 
        }
        //Kill(&robot);
        return;
    }

    // 2. Использование способностей (до движения)
    double dist_to_player = player_robot_.NowPosition().DistanceTo(robot.NowPosition().X(), robot.NowPosition().Y());
    bool player_visible = dist_to_player <= robot.VisibilityRadius();

    // Лечение (Heal)
    if (GetAbility(robot, Ability::AbilityType::kHeal) != nullptr && 
        robot.NowHealth() < robot.MaxHealth() && 
        CanRobotUseAbility(robot, Ability::AbilityType::kHeal)) {
        UseAbility(robot, Ability::AbilityType::kHeal, robot.NowPosition());
    }

    // Телепортация (Teleport)
    std::uniform_int_distribution<int> chance_dist(0, 99);
    static std::mt19937 rng(std::random_device{}());

    if (GetAbility(robot, Ability::AbilityType::kTeleport) != nullptr && 
        CanRobotUseAbility(robot, Ability::AbilityType::kTeleport)) {
        
        if (player_visible) {
            // Игрок виден: телепортируемся как можно ближе к нему
            Position target = player_robot_.NowPosition();
            // Ищем свободную клетку рядом с игроком
            for (Direction d : {Direction::kUp, Direction::kDown, Direction::kLeft, Direction::kRight}) {
                Position p = target.MoveToDirection(d);
                if (game_field_.IsInside(p) && game_field_.GetCell(p).Passable() && RobotOnPosition(p) == nullptr) {
                    target = p;
                    break;
                }
            }
            UseAbility(robot, Ability::AbilityType::kTeleport, target);
            
        } else {
            // Игрок не виден: 30% шанс телепорта в случайное место
            
            
            if (chance_dist(rng) < 30) {
                std::vector<Position> valid_spots;
                for (int y = 0; y < game_field_.Height(); ++y) {
                    for (int x = 0; x < game_field_.Width(); ++x) {
                        Position p(x, y);
                        if (game_field_.IsInside(p) && game_field_.GetCell(p).Passable() && 
                            game_field_.GetCell(p).MovementCost() <= robot.Speed() &&
                            RobotOnPosition(p) == nullptr && 
                            FactoryOnPosition(p) == nullptr) {
                            valid_spots.push_back(p);
                        }
                    }
                }
                if (!valid_spots.empty()) {
                    std::uniform_int_distribution<int> spot_dist(0, valid_spots.size() - 1);
                    UseAbility(robot, Ability::AbilityType::kTeleport, valid_spots[spot_dist(rng)]);
                }
            }
        }
    }

    // 3. Движение
    // Пересчитываем видимость (после телепортации)
    dist_to_player = player_robot_.NowPosition().DistanceTo(robot.NowPosition().X(), robot.NowPosition().Y());
    player_visible = dist_to_player <= robot.VisibilityRadius();
    
    
    bool step_found = 0;
    bool random_move = chance_dist(rng) < 20;
    if (player_visible && !random_move) {

        // --- Поиск пути алгоритмом Дейкстры ---
        
        Position nex_pos = game_field_.NexMoveWithDijksrta(robot.NowPosition(), Player().NowPosition(),
                                                        robot.Speed(), robot.VisibilityRadius());
        
        if(nex_pos == Position(-1, -1)) {
            step_found = 0;
        } else {
            step_found = 1;
        }

        if(step_found) {
            while(TryMove(robot, nex_pos)) {
                nex_pos = game_field_.NexMoveWithDijksrta(
                    robot.NowPosition(), Player().NowPosition(),
                    robot.Speed(), robot.VisibilityRadius());
            }
        }
    }
    if(!player_visible || !step_found || random_move) {
            
        // Если игрок не виден, или путь не найден (окружен) — идем случайно
        while (robot.MovesRemain() > 0) {

            std::vector<Position> allowed_positions;
            for (Direction dir : {Direction::kUp, Direction::kDown, Direction::kLeft, Direction::kRight}) {
                Position next = robot.NowPosition().MoveToDirection(dir);
                if (!game_field_.IsInside(next)) continue;
                if (!game_field_.GetCell(next).Passable()) continue;
                if (game_field_.GetCell(next).MovementCost() > robot.MovesRemain()) continue;
                allowed_positions.push_back(next);
            }

            if (allowed_positions.empty()) {
                robot.SetMovesRemain(0);
                break;
            }
            
            static std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<int> dist_dist(0, allowed_positions.size() - 1);
            Position nex_pos = allowed_positions[dist_dist(rng)];
            TryMove(robot, nex_pos);
            
        }
    }
    // 4. Статусы после хода
    ApplyStatuses(robot, Status::StatusType::kNone, Status::StatusGroup::kAfterMove);
}
void Game::RobotsMove() {
    BuildingsMove();

    for (auto& robot : robots_) {
        robot.RecoverMoves();
        robot.AddNowEnergy(25);
        //while(robot.MovesRemain() > 0)
        RobotMove(robot);
    }
    robots_.remove_if([](const Robot &robot){return robot.NowHealth() == 0;});
    IsWin();
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

Game::InputStatuses Game::InputStatus() const {
    return input_status_;
}
void Game::SetInputStatus(InputStatuses status) {
    input_status_ = status;
}




void Game::BuildingsMove() {
    for(auto &factory : factories_){

        factory.Move();
        if(factory.RobotsInside() > 0){
            bool is_spawned = SpawnRobot(EnemyRobot(GetId(), 100, 100, 50, factory.IsFriendly(), 1, 3), factory.TopLeftPosition().Left());
            if(is_spawned) factory.ReduceRobotsInside(1);
        }
    }
}

bool Game::SpawnRobot(EnemyRobot robot, Position pos) {
    std::uniform_int_distribution<int> ability(0, 1);
    static std::mt19937 rng(std::random_device{}());
    int r = ability(rng);
    if(game_field_.GetCell(pos).Passable() == 1 && RobotOnPosition(pos) == nullptr &&
        FactoryOnPosition(pos) == nullptr) {
        
            if(r == 0) AddAbility(robot, Ability::AbilityType::kHeal);
        if(r == 1) AddAbility(robot, Ability::AbilityType::kTeleport);
        
        robots_.push_back(std::move(robot));
        robots_.back().SetPosition(pos);
        return 1;
    }

    return 0;
}

 Ability::AbilityType Game::LastAbility() const {
    return last_ability_;
 }


void Game::AddAbility(Robot &robot, Ability::AbilityType ability_type) {
    Ability *current_ability = GetAbility(robot, ability_type);
    if(current_ability == nullptr) {
        switch(ability_type){
            case Ability::AbilityType::kAreaStrike:
                robot.Abilities().push_back( new AreaStrike(1) );
                break;
            case Ability::AbilityType::kHeal:
                robot.Abilities().push_back( new Heal(1) );
                break;
            case Ability::AbilityType::kFarHit:
                robot.Abilities().push_back( new FarHit(1) );
                break;
            case Ability::AbilityType::kTeleport:
                robot.Abilities().push_back( new Teleport(1) );
                break;
        }
    } else {
        current_ability->Upgrade();
    }
}

bool Game::CanRobotUseAbility(Robot &robot, Ability::AbilityType ability_type) {
    if(GetStatus(robot, Status::StatusType::kOverload) != nullptr) return false;

    for(auto &ability : robot.Abilities()) {
        if (ability->GetAbilityType() == ability_type) {
            if(ability->EnergyCost() <= robot.NowEnergy()) return true;
            break;
        }
    }
    return false;
}

void Game::UseAbility(Robot &robot, Ability::AbilityType ability_type, Position pos) {
    if(GetStatus(robot, Status::StatusType::kOverload) != nullptr) return;

    for(auto &ability : robot.Abilities()) {
        if (ability->GetAbilityType() == ability_type) {
            ability->Use(robot, *this, pos);
            break;
        }
    }
}



const Ability *Game::GetAbility(Robot &robot, Ability::AbilityType ability_type) const {
    return GetAbility(robot, ability_type);
}

const Ability *Game::GetAbility(const Robot &robot, Ability::AbilityType ability_type) const {
    for(auto &ability : robot.Abilities()) {
        if (ability->GetAbilityType() == ability_type) {
            return ability;
        }
    }
    return nullptr;
}
Ability *Game::GetAbility(Robot &robot, Ability::AbilityType ability_type) {
    for(auto &ability : robot.Abilities()) {
        if (ability->GetAbilityType() == ability_type) {
            return ability;
        }
    }
    return nullptr;
}

void Game::RankUp() {
    if(!player_robot_.IsEnoughXP()) return;
    
    while(player_robot_.IsEnoughXP()) {
        SetInputStatus(InputStatuses::kWaitingForRankUp);
        player_robot_.RankUp();
    }
}



Status* Game::GetStatus(Robot &robot, Status::StatusType status_type, Status::StatusGroup status_group) {
    for(auto& status : robot.Statuses()) {
        if((status->GetStatusType() == status_type || 
        status_type == Status::StatusType::kNone ) &&
        (status->GetStatusGroup() == status_group || 
        status_group == Status::StatusGroup::kNone)) {
            return status;
        }  
    }
    return nullptr;
}

const Status* Game::GetStatus(Robot &robot, Status::StatusType status_type, Status::StatusGroup status_group) const {
    return GetStatus(robot, status_type, status_group);
}
const Status* Game::GetStatus(const Robot &robot, Status::StatusType status_type, Status::StatusGroup status_group) const {
    return GetStatus(robot, status_type, status_group);
}

void Game::AddStatus(Robot &robot, Status *new_status) {
    if(new_status == nullptr) return;
    Status *old_status = GetStatus(robot, new_status->GetStatusType());
    if(old_status == nullptr) {
        robot.Statuses().push_back(new_status);
    } else {
        old_status->AddToCount(new_status->Count());
        delete new_status;
        new_status = nullptr;
    }
    return;
}

void Game::AddStatus(Robot &robot, Status::StatusType status_type) {
    Status *status = GetStatus(robot, status_type);
    if(status == nullptr) { 
        switch (status_type) {
            case Status::StatusType::kBurning:  robot.Statuses().push_back(new Burning()); break;
            case Status::StatusType::kOverload: robot.Statuses().push_back(new Overload()); break;
            case Status::StatusType::kShield:   robot.Statuses().push_back(new Shield()); break;
            case Status::StatusType::kSlowness: robot.Statuses().push_back(new Slowness()); break;
        }
    } else {
        status->AddToCount(1);
    }
}

void Game::ApplyStatuses(Robot &robot, Status::StatusType status_type, Status::StatusGroup status_group) {
    for(auto& status : robot.Statuses()) {
        if((status->GetStatusType() == status_type || 
        status_type == Status::StatusType::kNone ) &&
        (status->GetStatusGroup() == status_group || 
        status_group == Status::StatusGroup::kNone)) {
            status->DoAction(robot);
        }  
    }
    RemoveEmptyStatuses(robot);
}

void Game::RemoveEmptyStatuses(Robot &robot) {
    auto& list = robot.Statuses();
    for (auto it = list.begin(); it != list.end(); ) {
        if ((*it)->Count() == 0) {
            delete *it;
            it = list.erase(it);
        } else {
            ++it;
        }
    }
}


