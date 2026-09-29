
#ifndef HEAL_H
#define HEAL_H

#include "ability.hpp"
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
    void Upgrade(int level) override;

    int HealAmount() const;
    void Use(Robot &owner, Game& game, Position pos = Position(0, 0)) override;
    Ability::AbilityType GetAbilityType() override;
    int Radius() const override;
};


#endif