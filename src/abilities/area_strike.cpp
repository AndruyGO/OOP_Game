#include "area_strike.hpp"
#include "../core/game.hpp"

AreaStrike::AreaStrike(int level) 
  : level_(level),
    radius_(level),
    energy_cost_(level*50) {

    if(level < 1) {
        level_ = 1;
        radius_ = 1;
        energy_cost_ = 50;
    }

}

int AreaStrike::EnergyCost() const {
    return energy_cost_;
}

void AreaStrike::Upgrade(int level) {

}

int AreaStrike::Damage() const { return damage_; }

Ability::AbilityType AreaStrike::GetAbilityType() { return Ability::AbilityType::kAreaStrike; }

void AreaStrike::Use(Robot &owner, Game& game) {
    if (owner.NowEnergy() < EnergyCost()) return;

    owner.SetNowEnergy(owner.NowEnergy() - EnergyCost());

    const Position &pos = owner.NowPosition();
    for(int y = pos.Y() - radius_; y <= pos.Y()+radius_; y++) {
    for(int x = pos.X() - radius_; x <= pos.X()+radius_; x++) {
        if (pos.DistanceTo(x, y) <= radius_ && game.RobotOnPosition({x, y}) != nullptr){
            game.InteractWithRobot(owner, *game.RobotOnPosition({x, y}));
        }
    }
    }
}