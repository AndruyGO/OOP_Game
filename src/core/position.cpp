#include "position.hpp"

Position::Position(int x, int y)
  : x_(x),
    y_(y) {}

Position::Position()
  : x_(0),
    y_(0) {}



int Position::X() const { return x_; }
int Position::Y() const { return y_; }

void Position::SetPosition(int x, int y){
    x_ = x;
    y_ = y;
}

void Position::SetPosition(Position position){
    x_ = position.X();
    y_ = position.Y();
}

double Position::DistanceTo(Position pos) const{
    return DistanceTo(pos.X(), pos.Y());
}
double Position::DistanceTo(int x, int y) const{
    return std::sqrt((x-x_)*(x-x_) + (y-y_)*(y-y_));
}

Position Position::RePositionUp() {
    return Position ((*this).x_, --(*this).y_);
}

Position Position::RePositionDown() {
    return Position ((*this).x_, ++(*this).y_);
}

Position Position::RePositionLeft() {
    return Position (--(*this).x_, (*this).y_);
}

Position Position::RePositionRight() {
    return Position (++(*this).x_, (*this).y_);
}

Position Position::Up() const {
    return Position(x_, y_ - 1);
}
Position Position::Down() const {
    return Position(x_, y_ + 1);
}
Position Position::Left() const {
    return Position(x_ - 1, y_);
}
Position Position::Right() const {
    return Position(x_ + 1, y_);
}

Position Position::MoveToDirection(Direction direction){
    switch (direction)
    {
    case Direction::kUp:
        return (*this).Up();
    case Direction::kDown:
        return (*this).Down();
    case Direction::kLeft:
        return (*this).Left();
    case Direction::kRight:
        return (*this).Right();
    default:
        return *this;
    }
}

bool Position::operator ==(const Position &another_position) const {
    return x_ == another_position.X() && y_ == another_position.Y();
} 
bool Position::operator<(const Position &another_position) const {
    if (x_ != another_position.X()) {
        return x_ < another_position.X();
    }
    return y_ < another_position.Y();
}

bool Position::operator>(const Position &another_position) const {
    return another_position < *this;
}