#include "cell.hpp"
#include <stdexcept>
#include <random>

Cell::Cell(int type, bool is_visited, int movement_cost)
    : type_(type),
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
    return !(movement_cost_ == -1);
}

void Cell::SetMovementCost(int value){
    if(value < -2){
        movement_cost_ = -1;
    }else{
        movement_cost_ = value;
    }
}

const std::list<Status*> &Cell::GetStatuses() const {
    return statuses_;
}
const std::list<Status*> &Cell::GetItems() const {
    return items_;
}
std::list<Status*> &Cell::GetStatuses() {
    return statuses_;
}
std::list<Status*> &Cell::GetItems() {
    return items_;
}

Status * Cell::TakeRandomStatus() {
    static std::mt19937 rng(std::random_device{}());
    if(statuses_.size() == 0) return nullptr;

    std::uniform_int_distribution<int> rand(0, statuses_.size() - 1);
    
    auto it = statuses_.begin();
    int n = rand(rng);
    for(int i = 0; i < n; i++) it++;
    auto res = *it;
    statuses_.erase(it);
    return *it;
}


void Cell::AddStatus(Status::StatusType status_type) {
 
    switch (status_type) {
        case Status::StatusType::kBurning:  statuses_.push_back(new Burning()); break;
        case Status::StatusType::kOverload: statuses_.push_back(new Overload()); break;
        case Status::StatusType::kShield:   statuses_.push_back(new Shield()); break;
        case Status::StatusType::kSlowness: statuses_.push_back(new Slowness()); break;
        case Status::StatusType::kItem:
            // add item *
            break;
    }
}