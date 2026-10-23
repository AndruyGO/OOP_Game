#include "shield.hpp"
#include "../entities/robot.hpp"


Shield::Shield(int count) : count_(count) {}

Status::StatusType Shield::GetStatusType() const {
    return Status::StatusType::kShield;
}

void Shield::DoAction(Robot &target) {
    int delta = target.MaxHealth() - target.NowHealth();
    if(delta <= count_) {
        count_ -= delta;
        target.Heal(delta);
    } else {
        target.Heal(count_);
        count_ = 0;
    }
}

int Shield::Count() const {
    return count_;
}

void Shield::AddToCount(int value) {
    count_ += value;
    if(count_ < 0) count_ = 0;

}
bool Shield::IsToRemove() const {
    return Count() == 0;
}


Status::StatusGroup Shield::GetStatusGroup() const { return Status::StatusGroup::kInMove; }