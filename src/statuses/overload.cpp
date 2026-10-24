#include "overload.hpp"
#include "../entities/robot.hpp"

Overload::Overload(int count) : count_(count) {}

Status::StatusType Overload::GetStatusType() const {
    return Status::StatusType::kOverload;
}

void Overload::DoAction(Robot &target) {
    AddToCount(-1);
}

int Overload::Count() const {
    return count_;
}

void Overload::AddToCount(int value) {
    count_ += value;
    if(count_ < 0) count_ = 0;
    if(count_ > 1) count_ = 1;
}
bool Overload::IsToRemove() const {
    return Count() == 0;
}

Status::StatusGroup Overload::GetStatusGroup() const { return Status::StatusGroup::kBeforeMove; }