#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <vector>
#include "cell.hpp"
#include "position.hpp"

class GameField
{
private:
    int W_, H_;
    Position player_start_position_;

    std::vector<std::vector<Cell>> game_field_;

    
public:
    GameField(const std::vector<std::vector<int>> &layout,
        const std::vector<std::vector<int>> &movement_costs,
        const Position &player_start_position, bool generate_random_statuses = 0);
    
    GameField(int H = 32, int W = 32, bool generate_random_statuses = 0);
    GameField(const GameField&) = delete;
    GameField& operator=(const GameField&) = delete;
    GameField(GameField&&) = default;
    GameField& operator=(GameField&&) = default;
    ~GameField() = default;

    Position PlayerStartPosition() const;
    void AddFrame();
    bool IsInside(int x, int y) const;
    bool IsInside(Position position) const;
    int Height() const;
    int Width() const;


    Cell& GetCell(int x, int y);
    const Cell& GetCell(int x, int y) const;

    Cell& GetCell(Position position);
    const Cell& GetCell(Position position) const;

    void OpenCells(Position position, int radius);

    Position NexMoveWithDijksrta(Position start, Position end, int max_movement, int radius);

    static constexpr int kMaxHeight = 60;
    static constexpr int kMaxWidth = 60;
    static constexpr int kMinHeight = 10;
    static constexpr int kMinWidth = 10;
private:
    Status::StatusType GetStatusWithNumber(int n);
    
};





#endif