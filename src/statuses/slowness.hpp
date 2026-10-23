#ifndef SLOWNESS_H
#define SLOWNESS_H

#include "status.hpp"

class Slowness : public Status {
public:

    Slowness(int count = 1);


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