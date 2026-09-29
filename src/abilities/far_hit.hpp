
#ifndef FAR_HIT_H
#define FAR_HIT_H

#include "ability.hpp"
class Robot;

class FarHit : public Ability {
private:
    int level_;
    int energy_cost_;
    int radius_;

public:

    FarHit(int level = 1);

    ~FarHit() = default;

    int EnergyCost() const override;
    void Upgrade(int level) override;

    void Use(Robot &owner, Game& game, Position pos = Position(0, 0)) override;
    Ability::AbilityType GetAbilityType() override;
    int Radius() const override;
};


#endif