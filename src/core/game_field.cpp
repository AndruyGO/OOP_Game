#include <stdexcept>
#include "game_field.hpp"
#include "position.hpp"
#include "cell.hpp"


GameField::GameField(const std::vector<std::vector<int>> &layout,
        const std::vector<std::vector<int>> &movement_costs,
        const Position &player_start_position) :
        player_start_position_(player_start_position) {

    if (layout.empty())
        throw std::invalid_argument("Empty field");
    W_ = layout[0].size();
    H_ = layout.size();
    game_field_.resize(H_, std::vector<Cell> (W_, Cell(0, 0)));
    for(int y = 0; y < H_; y++){
        for(int x = 0; x < W_; x++){
            game_field_[y][x].SetType(layout[y][x]);
            game_field_[y][x].SetMovementCost(movement_costs[y][x]);
        }
    }
    AddFrame();
}
GameField::GameField(int H, int W) {
    W_ = W;
    H_ = H;
    if(H < kMinHeight) H_ = kMinHeight;
    if(H > kMaxHeight) H_ = kMaxHeight;
    if(W < kMinWidth) W_ = kMinWidth;
    if(W > kMaxWidth) W_ = kMaxWidth;
    
    game_field_.resize(H_, std::vector<Cell> (W_, Cell(0, 0)));
    for(int y = 0; y < H_; y++){
        for(int x = 0; x < W_; x++){
            game_field_[y][x].SetType(0);
            game_field_[y][x].SetMovementCost(1);
        }
    }
    AddFrame();

    player_start_position_ = Position(1, 1);    

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