#include <stdexcept>
#include "game_field.hpp"
#include "position.hpp"
#include "cell.hpp"
#include <set>
#include <random>
#include <iostream>

GameField::GameField(const std::vector<std::vector<int>> &layout,
        const std::vector<std::vector<int>> &movement_costs,
        const Position &player_start_position, bool generate_random_statuses) :
        player_start_position_(player_start_position) {

    if (layout.empty())
        throw std::invalid_argument("Empty field");
    W_ = layout[0].size();
    H_ = layout.size();
    
    game_field_.clear();
    game_field_.resize(H_);
    for(int y = 0; y < H_; y++){
        game_field_[y].reserve(W_);
        for(int x = 0; x < W_; x++){

            game_field_[y].emplace_back(0, false, 1); 
            
            game_field_[y][x].SetType(layout[y][x]);
            game_field_[y][x].SetMovementCost(movement_costs[y][x]);
        }
    }
    AddFrame();
    
    if(generate_random_statuses == 0) return;
    // random statuses
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> rand(0, 100);
    std::uniform_int_distribution<int> status_num(0, 3);

    for(int y = 0; y < H_; y++){
        for(int x = 0; x < W_; x++){
            if(!game_field_[y][x].Passable()) continue;
            if(rand(rng) >= 90) {
                int n = status_num(rng);
                game_field_[y][x].AddStatus(GetStatusWithNumber(n));
            }
            if(rand(rng) >= 90) {
                int n = status_num(rng);
                game_field_[y][x].AddStatus(GetStatusWithNumber(n));
            }
        }
    }
}

GameField::GameField(int H, int W, bool generate_random_statuses) {
    W_ = W;
    H_ = H;
    if(H < kMinHeight) H_ = kMinHeight;
    if(H > kMaxHeight) H_ = kMaxHeight;
    if(W < kMinWidth) W_ = kMinWidth;
    if(W > kMaxWidth) W_ = kMaxWidth;
    
    game_field_.clear();
    game_field_.resize(H_); 
    for(int y = 0; y < H_; y++){
        game_field_[y].reserve(W_);
        for(int x = 0; x < W_; x++){
            game_field_[y].emplace_back(1, false, 1);
            
            game_field_[y][x].SetType(1);
            game_field_[y][x].SetMovementCost(1);
        }
    }
    AddFrame();
    
    player_start_position_.SetPosition(3, 3);    

    // random statuses
    if(generate_random_statuses == 0) return;
    
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> rand(0, 100);
    std::uniform_int_distribution<int> status_num(0, 3);

    for(int y = 0; y < H_; y++){
        for(int x = 0; x < W_; x++){
            if(!game_field_[y][x].Passable()) continue;
            if(rand(rng) >= 90) {
                int n = status_num(rng);
                game_field_[y][x].AddStatus(GetStatusWithNumber(n));
            }
            if(rand(rng) >= 90) {
                int n = status_num(rng);
                game_field_[y][x].AddStatus(GetStatusWithNumber(n));
            }
        }
    }

}


void GameField::AddFrame(){
    if (game_field_.empty())
        throw std::invalid_argument("Empty field");
    W_ = game_field_[0].size();
    H_ = game_field_.size();

    for(int x = 0; x < W_; x++){
        game_field_[0][x].SetType(0);
        game_field_[H_-1][x].SetType(0);
        game_field_[0][x].SetMovementCost(-1);
        game_field_[H_-1][x].SetMovementCost(-1);
    }
    for(int y = 0; y < H_; y++){
        game_field_[y][0].SetType(0);
        game_field_[y][W_-1].SetType(0);
        game_field_[y][0].SetMovementCost(-1);
        game_field_[y][W_-1].SetMovementCost(-1);
    }
}

int GameField::Height() const {
    return H_;
}
int GameField::Width() const {
    return W_;
}

Position GameField::PlayerStartPosition() const {
    return player_start_position_;
}

bool GameField::IsInside(int x, int y) const {
    return x >= 0 && x < W_ && y >= 0 && y < H_;
}

bool GameField::IsInside(Position position) const {
    return IsInside(position.X(), position.Y());
}

Cell& GameField::GetCell(int x, int y) {
    if (!IsInside(x, y))
        throw std::out_of_range("Cell coordinates out of bounds");
    return game_field_[y][x];
}

const Cell& GameField::GetCell(int x, int y) const {
    if (!IsInside(x, y))
        throw std::out_of_range("Cell coordinates out of bounds");
    return game_field_[y][x];
}
Cell& GameField::GetCell(Position position) {
    return GetCell(position.X(), position.Y());
}

const Cell& GameField::GetCell(Position position) const {
    if(!IsInside(position)) throw std::out_of_range("Cell coordinates out of bounds");
    return GetCell(position.X(), position.Y());
}


void GameField::OpenCells(Position position, int radius) {
    for(int x = position.X() - radius; x <= position.X() + radius; x++){
        for(int y = position.Y() - radius; y <= position.Y() + radius; y++){
            if(IsInside(x, y) && position.DistanceTo(x, y) <= radius){
                game_field_[y][x].SetVisited(true);
            }
        }
    }
}

Status::StatusType GameField::GetStatusWithNumber(int n) {
  
    switch (n) {
        case 0: return Status::StatusType::kBurning; break;
        case 1: return Status::StatusType::kOverload; break;
        case 2: return Status::StatusType::kShield; break;
        case 3: return Status::StatusType::kSlowness; break;
    }
    return Status::StatusType::kNone;
}




Position GameField::NexMoveWithDijksrta(Position start, Position end, int max_movement, int radius) {
    using Node = std::pair<std::pair<int, int>, std::pair<Position, Position>>;
    std::set<Node> priority_queue;
    //           dist,      now pos,  first pos after start_pos

    std::vector<std::vector<int>> best_dist(H_, std::vector<int>(W_, 999999));

    for(Position pos : {start.Up(), start.Right(), start.Left(), start.Down()}) {
        if(IsInside(pos) && GetCell(pos).Passable() &&
        GetCell(pos).MovementCost() <=  max_movement) {
            int cost = GetCell(pos).MovementCost();
            priority_queue.insert({{cost, 1}, {pos, pos}});
            best_dist[pos.Y()][pos.X()] = cost;
        }
    }

    while(!priority_queue.empty()) {
        Node p = *priority_queue.begin();
        priority_queue.erase(priority_queue.begin());

        if(p.second.first == end) {
            return p.second.second;
        }
        
        if(p.first.second < radius) {
            for(Position pos : {p.second.first.Up(), p.second.first.Right(), p.second.first.Left(), p.second.first.Down()}) {
                if(IsInside(pos) && GetCell(pos).Passable() &&
                GetCell(pos).MovementCost() <=  max_movement) {
                    
                    int new_dist = p.first.first + GetCell(pos).MovementCost();

                    if(new_dist < best_dist[pos.Y()][pos.X()]) {
                        best_dist[pos.Y()][pos.X()] = new_dist;
                        priority_queue.insert({{new_dist, p.first.second + 1}, {pos, p.second.second}});
                    }
                }
            }
        }
    }

    return Position(-1, -1);
}