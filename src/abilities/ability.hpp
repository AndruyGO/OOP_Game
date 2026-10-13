#ifndef ABILITY_H
#define ABILITY_H


class Game;
class Robot;

#include "../core/position.hpp"

class Ability {
public:

    enum class AbilityType {
        kAreaStrike,
        kFarHit,
        kHeal,
        kTeleport
    };

    virtual ~Ability() = default;
    virtual int EnergyCost() const = 0;
    virtual void Upgrade() = 0;
    virtual void Use(Robot &owner, Game& game, Position pos) = 0;
    virtual AbilityType GetAbilityType() const = 0;
    virtual int Radius() const = 0;
    virtual int Level() const = 0;
};



#endif