
#ifndef POS_H
#define POS_H
#include <cmath>
#include "direction.hpp"

class Position{
private:
    int x_, y_;
public:
    Position(int x, int y);
    Position();
    ~Position() = default;

    int X() const;
    int Y() const;
    void SetPosition(int x, int y);
    void SetPosition(Position position);
    double DistanceTo(int x, int y) const;
    double DistanceTo(Position pos) const;

    Position Up();
    Position Down();
    Position Left();
    Position Right();

    Position MoveToDirection(Direction direction);

    bool operator ==(const Position &another_position) const;
    
};

#endif