
#ifndef AREA_STRIKE_H
#define AREA_STRIKE_H

#include "ability.hpp"
class Robot;

class AreaStrike : public Ability {
private:
    int level_;
    int energy_cost_;
    int damage_;
    int radius_;

public:

    AreaStrike(int level = 1);

    ~AreaStrike() = default;

    int EnergyCost() const override;
    void Upgrade(int level) override;

    int Damage() const;
    void Use(Robot &owner, Game& game) override;
    Ability::AbilityType GetAbilityType() override;
};


#endif