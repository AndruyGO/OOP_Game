#ifndef ABILITY_H
#define ABILITY_H


class Game;
class Robot;

class Ability {
public:

    enum class AbilityType {
        kAreaStrike,
        kFarHit,
        lHeal,
        kTeleport
    };

    virtual ~Ability() = default;
    virtual int EnergyCost() const = 0;
    virtual void Upgrade(int level) = 0;
    virtual void Use(Robot &owner, Game& game) = 0;
    virtual AbilityType GetAbilityType() = 0;
};



#endif