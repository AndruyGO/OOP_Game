#include "burning.hpp"
#include "../entities/robot.hpp"

Burning::Burning(int count) : count_(count) {}

Status::StatusType Burning::GetStatusType() const {
    return Status::StatusType::kBurning;
}

void Burning::DoAction(Robot &target) {
    target.Hit(30);
    count_--;
}

int Burning::Count() const {
    return count_;
}

void Burning::AddToCount(int value) {
    count_ += value*3;
    if(count_ < 0) count_ = 0;
}
bool Burning::IsToRemove() const {
    return Count() == 0;
}

Status::StatusGroup Burning::GetStatusGroup() const { return Status::StatusGroup::kBeforeMove; }