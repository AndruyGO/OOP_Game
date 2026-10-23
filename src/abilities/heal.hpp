
#ifndef HEAL_H
#define HEAL_H

#include "ability.hpp"
#include "../statuses/shield.hpp"
class Robot;

class Heal : public Ability {
private:
    int level_;
    int energy_cost_;
    int heal_amount_;

public:

    Heal(int level = 1);

    ~Heal() = default;

    int EnergyCost() const override;
    void Upgrade() override;

    int HealAmount() const;
    void Use(Robot &owner, Game& game, Position pos = Position(0, 0)) override;
    Ability::AbilityType GetAbilityType() const override;
    int Radius() const override;
    int Level() const override;
};


#endif