#include "building.hpp"

Building::Building(int id, Position top_left_position, int size,
                   bool is_invulnerable, int health, int type, bool is_friendly)
    : id_(id),
      top_left_position_(top_left_position),
      size_(size),
      is_invulnerable_(is_invulnerable),
      health_(health),
      type_(type) {
    if (size < 1)
        size_ = 1;
    if (health < 0)
        health_ = 0;
}

Position Building::TopLeftPosition() const { return top_left_position_; }
int Building::Size() const { return size_; }
bool Building::IsInvulnerable() const { return is_invulnerable_; }
int Building::Health() const { return health_; }
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

void Building::SetHealth(int value) {
    if (value < 0)
        value = 0;
    health_ = value;
}

void Building::SetType(int value) {
    type_ = value;
}