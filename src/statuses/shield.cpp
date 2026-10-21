#include "shield.hpp"
#include "../entities/robot.hpp"

#define STRENGTH_PER_COUNT 100
Shield::Shield() : count_(1), strength_(STRENGTH_PER_COUNT) {}

Status::StatusType Shield::GetStatusType() const {
    return Status::StatusType::kShield;
}

void Shield::DoAction(Robot &target) {
    int delta = target.MaxHealth() - target.NowHealth();
    if(delta <= strength_) {
        strength_ -= delta;
        target.Heal(delta);
    } else { // delta > strength
        target.Heal(strength_);
        strength_ = 0;
    }

    count_ = ((strength_ + STRENGTH_PER_COUNT - 1) / STRENGTH_PER_COUNT);
}

int Shield::Count() const {
    return strength_;
}

void Shield::AddToCount(int value) {
    int delta = value * STRENGTH_PER_COUNT;

    strength_ += value;
    if(strength_ < 0) strength_ = 0;

    count_ = ((strength_ + STRENGTH_PER_COUNT - 1) / STRENGTH_PER_COUNT);

}
bool Shield::IsToRemove() const {
    return Count() == 0;
}


Status::StatusGroup Shield::GetStatusGroup() const { return Status::StatusGroup::kInMove; }