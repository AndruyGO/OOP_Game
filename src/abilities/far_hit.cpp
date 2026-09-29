#include "far_hit.hpp"
#include "../core/game.hpp"

FarHit::FarHit(int level) 
  : level_(level),
    radius_(level*3),
    energy_cost_(level*50) {

    if(level < 1) {
        level_ = 1;
        radius_ = 3;
        energy_cost_ = 50;
    }

}

int FarHit::EnergyCost() const {
    return energy_cost_;
}

void FarHit::Upgrade(int level) {

}

Ability::AbilityType FarHit::GetAbilityType() { return Ability::AbilityType::kFarHit; }

void FarHit::Use(Robot &owner, Game& game, Position pos) {
    if (owner.NowEnergy() < EnergyCost()) return;
    if (owner.NowPosition().DistanceTo(pos) > radius_) return;
    Robot *target_robot = game.RobotOnPosition(pos);
    RobotFactory *target_factory = game.FactoryOnPosition(pos);
    
    if(target_robot != nullptr || target_factory != nullptr) { 
        owner.SetNowEnergy(owner.NowEnergy() - EnergyCost());
        if(target_robot != nullptr) game.InteractWithRobot(owner, *target_robot);
        if(target_factory != nullptr) game.InteractWithBuilding(owner, *target_factory);
    }
}

int FarHit::Radius() const {
    return radius_;
}