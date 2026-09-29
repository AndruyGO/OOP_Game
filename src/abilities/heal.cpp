#include "heal.hpp"
#include "../core/game.hpp"

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

void Heal::Upgrade(int level) {
    energy_cost_ = level*50;
    heal_amount_ = int(energy_cost_*1.5f);
}

int Heal::HealAmount() const { return heal_amount_; }

Ability::AbilityType Heal::GetAbilityType() { return Ability::AbilityType::kHeal; }

void Heal::Use(Robot &owner, Game& game, Position pos) {
    if (owner.NowEnergy() < EnergyCost()) return;

    owner.SetNowEnergy(owner.NowEnergy() - EnergyCost());

    owner.SetNowHealth(owner.NowHealth()+heal_amount_);
}

int Heal::Radius() const { return 0; }