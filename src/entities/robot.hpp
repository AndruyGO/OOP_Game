
#ifndef ROBOT_H
#define ROBOT_H
#include "../core/position.hpp"

class Robot
{
protected:
    int id_;
    int max_health_;
    int max_energy_;
    int damage_;
    bool is_friendly_;
    int speed_;
    int moves_remain_;
    int now_health_;
    int now_energy_;
    Position now_position_;

public:
    Robot(int of, int max_health, int max_energy,
        int damage, bool is_friendly, int speed);

    virtual ~Robot() = default;

    int MaxHealth() const;
    int MaxEnergy() const;
    int NowHealth() const;
    int NowEnergy() const;
    int Damage() const;
    bool IsFriendly() const;
    Position NowPosition() const;
    int Id() const;
    int Speed() const;
    int MovesRemain() const;

    void SetMaxHealth(int value);
    void SetMaxEnergy(int value);
    void SetNowHealth(int value);
    void SetNowEnergy(int value);
    void SetDamage(int value);
    void SetPosition(Position position);
    void SetMovesRemain(int value);
    void SetSpeed(int value);
    
    void ReduceMovesRemain(int value);
    
    void Interact(class Robot &another_robot);
    void RecoverMoves();
    
    void Hit(int value);
    void Heal(int value);
    void AddNowEnergy(int value);

    bool operator < (const Robot &another_robot) const;
    bool operator == (const Robot &another_robot) const;

    static constexpr int kHealAmount = 25;
};


#endif