#include "cell.hpp"
#include <stdexcept>

Cell::Cell(int type, bool passable, bool is_visited, int movement_cost)
    : type_(type),
      passable_(passable),
      is_visited_(is_visited),
      movement_cost_(movement_cost) {}


int Cell::Type() const { return type_; }
bool Cell::IsVisited() const { return is_visited_; }
int Cell::MovementCost() const { return movement_cost_; }

void Cell::SetType(int type) {
    if (type < 0)
        throw std::invalid_argument("Cell type cannot be negative");
    type_ = type;
}


void Cell::SetVisited(bool visited) {
    is_visited_ = visited;
}

bool Cell::Passable() const {
    return passable_;
}

void Cell::SetPassable(bool passable) {
    passable_ = passable;
}

void Cell::SetMovementCost(int value){
    if(value < -2){
        movement_cost_ = -2;
    }else{
        movement_cost_ = value;
    }
}