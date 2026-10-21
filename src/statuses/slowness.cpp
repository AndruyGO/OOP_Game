#include "slowness.hpp"
#include "../entities/robot.hpp"

Slowness::Slowness() : count_(1) {}

Status::StatusType Slowness::GetStatusType() const {
    return Status::StatusType::kSlowness;
}

void Slowness::DoAction(Robot &target) {
    target.SetMovesRemain(0);
    AddToCount(-1);    
}

int Slowness::Count() const {
    return count_;
}

void Slowness::AddToCount(int value) {
    count_ += value;
    if(count_ < 0) count_ = 0;
}
bool Slowness::IsToRemove() const {
    return Count() == 0;
}


Status::StatusGroup Slowness::GetStatusGroup() const { return Status::StatusGroup::kBeforeMove; }