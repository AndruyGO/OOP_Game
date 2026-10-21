#ifndef STATUS_H
#define STATUS_H

class Robot;

class Status {
public:
    enum class StatusType {
        kNone,
        kItem,
        kSlowness,
        kOverload,
        kBurning,
        kShield
    };

    enum class StatusGroup {
        kNone,
        kBeforeMove,
        kAfterMove,
        kInMove
    };

    virtual ~Status() = default;

    virtual StatusType GetStatusType() const = 0;
    virtual StatusGroup GetStatusGroup() const = 0;
    virtual void DoAction(Robot &target) = 0;
    virtual int Count() const = 0;
    virtual void AddToCount(int value) = 0;
    virtual bool IsToRemove() const = 0;

};



#endif