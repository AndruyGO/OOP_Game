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

void AreaStrike::Use(Robot &owner, Game& game, Position pos) {
    if (owner.NowEnergy() < EnergyCost()) return;

    owner.SetNowEnergy(owner.NowEnergy() - EnergyCost());

    const Position &owner_pos = owner.NowPosition();
    for(int y = owner_pos.Y() - radius_; y <= owner_pos.Y()+radius_; y++) {
    for(int x = owner_pos.X() - radius_; x <= owner_pos.X()+radius_; x++) {
        if (owner_pos.DistanceTo(x, y) <= radius_) {
            Robot *target_robot = game.RobotOnPosition({x, y});
            RobotFactory *target_factory = game.FactoryOnPosition({x, y});

            if(target_robot != nullptr) game.InteractWithRobot(owner, *target_robot);
            if(target_factory != nullptr) game.InteractWithBuilding(owner, *target_factory);
        }
    }
    }
}

int AreaStrike::Radius() const {
    return radius_;
}