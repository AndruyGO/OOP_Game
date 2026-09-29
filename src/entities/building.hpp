#ifndef BUILDING_H
#define BUILDING_H

#include "../core/position.hpp"

class Building {
private:
    int id_;
    Position top_left_position_;
    int size_;
    bool is_invulnerable_;
    int now_health_;
    int type_;
    bool is_friendly_;

public:
    Building(int id, Position top_left_position, int size, bool is_invulnerable,
        int now_health, int type, bool is_friendly);
    ~Building() = default;

    Position TopLeftPosition() const;
    int Size() const;
    bool IsInvulnerable() const;
    int NowHealth() const;
    int Type() const;
    bool IsFriendly() const;
    int Id() const;

    void SetTopLeftPosition(Position position);
    void SetSize(int size);
    void SetInvulnerable(bool value);
    void SetNowHealth(int value);
    void SetType(int value);

    void Heal(int value);
    void Hit(int value);
    
    bool operator == (const Building &another_building) const;

};

#endif