#include "teleport.hpp"
#include "../core/game.hpp"

Teleport::Teleport(int level) 
  : level_(level),
    radius_(level*3),
    energy_cost_(level*100) {

    if(level < 1) {
        level_ = 1;
        radius_ = 3;
        energy_cost_ = 100;
    }

}

int Teleport::EnergyCost() const {
    return energy_cost_;
}
    
void Teleport::Upgrade(int level) {
   
}

Ability::AbilityType Teleport::GetAbilityType() { return Ability::AbilityType::kTeleport; }

void Teleport::Use(Robot &owner, Game& game, Position pos) {
    if (owner.NowEnergy() < EnergyCost()) return;
    if (owner.NowPosition().DistanceTo(pos) > radius_) return;
    if(!game.Field().IsInside(pos)) return;
    if(!game.Field().GetCell(pos).Passable()) return;

    Robot *target_robot = game.RobotOnPosition(pos);
    RobotFactory *target_factory = game.FactoryOnPosition(pos);
    
    if(target_robot == nullptr && target_factory == nullptr) { 
        owner.SetNowEnergy(owner.NowEnergy() - EnergyCost());
        owner.SetPosition(pos);
    }
}

int Teleport::Radius() const { return radius_; }