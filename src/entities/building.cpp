#include "building.hpp"

Building::Building(int id, Position top_left_position, int size,
                   bool is_invulnerable, int now_health, int type, bool is_friendly)
    : id_(id),
      top_left_position_(top_left_position),
      size_(size),
      is_invulnerable_(is_invulnerable),
      now_health_(now_health),
      type_(type),
      is_friendly_(is_friendly) {
    if (size < 1)
        size_ = 1;
    if (now_health < 0)
        now_health_ = 0;
}

bool Building::operator == (const Building &another_building) const {
    return id_ == another_building.Id();  
}


Position Building::TopLeftPosition() const { return top_left_position_; }
int Building::Size() const { return size_; }
bool Building::IsInvulnerable() const { return is_invulnerable_; }
int Building::NowHealth() const { return now_health_; }
int Building::Type() const { return type_; }
bool Building::IsFriendly() const { return is_friendly_; }
int Building::Id() const { return id_; }


void Building::SetTopLeftPosition(Position position) {
    top_left_position_ = position;
}

void Building::SetSize(int size) {
    if (size < 1)
        size = 1;
    size_ = size;
}

void Building::SetInvulnerable(bool value) {
    is_invulnerable_ = value;
}

void Building::SetNowHealth(int value) {
    if (value < 0)
        value = 0;
    now_health_ = value;
}

void Building::SetType(int value) {
    type_ = value;
}

void Building::Heal(int value) {
    SetNowHealth(NowHealth()+value);
}

void Building::Hit(int value) {
    SetNowHealth(NowHealth()-value);
}