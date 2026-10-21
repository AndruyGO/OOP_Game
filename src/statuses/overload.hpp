#ifndef OVERLOAD_H
#define OVERLOAD_H

#include "status.hpp"

class Overload : public Status {
public:

    Overload();


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