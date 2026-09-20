#ifndef CELL_H
#define CELL_H

class Cell{
private:
    int type_;
    bool passable_;
    bool is_visited_;    
    int movement_cost_;
public:
    Cell(int type, bool passable, bool is_visited = false, int movement_cost_ = 1);
    ~Cell() = default;

    int Type() const;
    bool Passable() const;
    bool IsVisited() const;
    int MovementCost() const;

    void SetVisited(bool visited);
    void SetType(int type);
    void SetPassable(bool passbale);
    void SetMovementCost(int value);

};

#endif