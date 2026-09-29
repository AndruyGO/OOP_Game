
#ifndef TELEPORT_H
#define TELEPORT_H

#include "ability.hpp"
class Robot;

class Teleport : public Ability {
private:
    int level_;
    int energy_cost_;
    int radius_;

public:

    Teleport(int level = 1);

    ~Teleport() = default;

    int EnergyCost() const override;
    void Upgrade(int level) override;

    void Use(Robot &owner, Game& game, Position pos = Position(0, 0)) override;
    Ability::AbilityType GetAbilityType() override;
    int Radius() const override;
};


#endif