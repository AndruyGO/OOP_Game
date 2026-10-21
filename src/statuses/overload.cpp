#include "overload.hpp"
#include "../entities/robot.hpp"

Overload::Overload() : count_(1) {}

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
}
bool Overload::IsToRemove() const {
    return Count() == 0;
}

Status::StatusGroup Overload::GetStatusGroup() const { return Status::StatusGroup::kAfterMove; }