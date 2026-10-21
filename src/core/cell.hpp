#ifndef CELL_H
#define CELL_H

#include "../statuses/all_statuses.hpp"
#include <list>

class Cell{
private:
    int type_;
    bool is_visited_;
    int movement_cost_;
    std::list<Status*> statuses_;
    std::list<Status*> items_;
public:
    
    enum class TypeOfCell {
        kWall,
        kGrass,
        kSwamp
    };

    Cell(int type, bool is_visited = false, int movement_cost_ = 1);
    ~Cell() = default;

    int Type() const;
    bool Passable() const;
    bool IsVisited() const;
    int MovementCost() const;

    void SetVisited(bool visited);
    void SetType(int type);
    void SetMovementCost(int value);

    std::list<Status*> &GetStatuses();
    std::list<Status*> &GetItems();
    const std::list<Status*> &GetStatuses() const;
    const std::list<Status*> &GetItems() const;
    Status *TakeRandomStatus();
    void AddStatus(Status::StatusType status_type);
};

#endif