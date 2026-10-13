#include "heal.hpp"
#include "../core/game.hpp"
#include <iostream>
Heal::Heal(int level) 
  : level_(level),
    energy_cost_(level*50) {

    if(level < 1) {
        level_ = 1;
        energy_cost_ = 50;
    }
    heal_amount_ = level_*50;

}

int Heal::EnergyCost() const {
    return energy_cost_;
}

int Heal::Level() const { return level_; }

void Heal::Upgrade() {
    level_++;
    energy_cost_ = (level_)*50;
    heal_amount_ = int(energy_cost_*1.5f);
}

int Heal::HealAmount() const { return heal_amount_; }

Ability::AbilityType Heal::GetAbilityType() const { return Ability::AbilityType::kHeal; }

void Heal::Use(Robot &owner, Game& game, Position pos) {
    if (owner.NowEnergy() < EnergyCost()) return;

    owner.SetNowEnergy(owner.NowEnergy() - EnergyCost());

    owner.SetNowHealth(owner.NowHealth()+heal_amount_);
}

int Heal::Radius() const { return 0; }