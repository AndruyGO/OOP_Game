#ifndef SHIELD_H
#define SHIELD_H

#include "status.hpp"

class Shield : public Status {
public:

    Shield(int count = 100);


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