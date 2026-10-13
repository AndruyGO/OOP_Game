
#ifndef AREA_STRIKE_H
#define AREA_STRIKE_H

#include "ability.hpp"
class Robot;

class AreaStrike : public Ability {
private:
    int level_;
    int radius_;
    int energy_cost_;
    int damage_;

public:

    AreaStrike(int level = 1);

    ~AreaStrike() = default;

    int EnergyCost() const override;
    void Upgrade() override;

    int Damage() const;
    void Use(Robot &owner, Game& game, Position pos = Position(0, 0)) override;
    Ability::AbilityType GetAbilityType() const override;
    int Radius() const override;
    int Level() const override;
};


#endif