#ifndef BURNING_H
#define BURNING_H

#include "status.hpp"

class Burning : public Status {
public:

    Burning();


    StatusType GetStatusType() const override;
    StatusGroup GetStatusGroup() const override;
    void DoAction(Robot &target) override;
    int Count() const override;
    void AddToCount(int value) override;
    bool IsToRemove() const override;

private:
    int count_;
};



#endif