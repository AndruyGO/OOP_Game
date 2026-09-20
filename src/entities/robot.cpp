#include "robot.hpp"
#include "enemy_robot.hpp"



Robot::Robot(int id, int max_health, int max_energy, int damage,
    bool is_friendly, int speed)
    : id_(id),
      max_health_(max_health),
      max_energy_(max_energy),
      damage_(damage), 
      is_friendly_(is_friendly),
      speed_(speed),
      moves_remain_(speed),
      now_health_(max_health),
      now_energy_(max_energy) {}


    bool Robot::operator < (const Robot &another_robot) const {
        return id_ < another_robot.Id();    
    }
    bool Robot::operator == (const Robot &another_robot) const {
        return id_ == another_robot.Id();  
    }

int Robot::MaxHealth() const { return max_health_; }
int Robot::MaxEnergy() const { return max_energy_; }
int Robot::NowHealth() const { return now_health_; }
int Robot::NowEnergy() const { return now_energy_; }
int Robot::Damage() const { return damage_; }
bool Robot::IsFriendly() const { return is_friendly_; }
Position Robot::NowPosition() const { return now_position_; }
int Robot::Id() const { return id_; }

void Robot::RecoverMoves(){
    moves_remain_ = speed_;
}

int Robot::Speed() const{
    return speed_;
}
int Robot::MovesRemain() const {
    return moves_remain_;
}
void Robot::SetMovesRemain(int value) {
    if(value < 0) {
        moves_remain_ = 0;
    } else {
        moves_remain_ = value;
    }
}
void Robot::SetSpeed(int value) {
    if(value < 0) {
        speed_ = 0;
    } else {
        speed_ = value;
    }
}
void Robot::ReduceMovesRemain(int value) {
    if(moves_remain_ - value < 0) {
        moves_remain_ = 0;
    } else {
        moves_remain_ -= value;
    }
}


void Robot::SetMaxHealth(int value) {
    if (value < 0)
        value = 0;
    max_health_ = value;
    if (now_health_ > max_health_)
        now_health_ = max_health_;
}

void Robot::SetMaxEnergy(int value) {
    if (value < 0)
        value = 0;
    max_energy_ = value;
    if (now_energy_ > max_energy_)
        now_energy_ = max_energy_;
}

void Robot::SetNowHealth(int value) {
    if (value < 0)
        value = 0;
    if (value > max_health_)
        value = max_health_;
    now_health_ = value;
}

void Robot::SetNowEnergy(int value) {
    if (value < 0)
        value = 0;
    if (value > max_energy_)
        value = max_energy_;
    now_energy_ = value;
}

void Robot::SetDamage(int value) {
    if (value < 0)
        value = 0;
    damage_ = value;
}

void Robot::SetPosition(Position position){
    now_position_.SetPosition(position);
}

void Robot::Hit(int value){
    SetNowHealth(NowHealth()-value);
}
void Robot::Heal(int value){
    SetNowHealth(NowHealth()+value);
}

void Robot::Interact(class Robot &another_robot){
    if(another_robot.IsFriendly() == IsFriendly()){
        another_robot.Heal(kHealAmount);
    }else{
        another_robot.Hit(Damage());
    }
}

void Robot::AddNowEnergy(int value){
    SetNowEnergy(now_energy_+value);
}
