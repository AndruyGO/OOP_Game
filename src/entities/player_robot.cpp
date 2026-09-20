#include "player_robot.hpp"
#include "enemy_robot.hpp"

PlayerRobot::PlayerRobot(int id, int max_health, int max_energy, int damage,
                        bool is_friendly, int speed, int rank, int visibility_radius)
  : Robot(id, max_health, max_energy, damage, is_friendly, speed),
    rank_(rank),
    visibility_radius_(visibility_radius){
        is_friendly_ = 1;

        now_xp_ = 0;
        xp_required = CalculateXPForRank(rank_);
        max_health_ = 50 + 50*rank_;
        max_energy_ += 50 + 50*rank_;
        damage_ = 50*rank_;
    }

PlayerRobot::PlayerRobot()
  : Robot(0, 100, 100, 50, 1, 1),
    rank_(1),
    visibility_radius_(3) {
        xp_required = CalculateXPForRank(rank_);
        max_health_ = 50 + 50*rank_;
        max_energy_ += 50 + 50*rank_;
        damage_ = 50*rank_;
    }

int PlayerRobot::Rank() const { return rank_; }
int PlayerRobot::VisibilityRadius() const { return visibility_radius_; }
void PlayerRobot::SetVisibilityRadius(int value) {
    if (value < 1){
        visibility_radius_ = 1;
    }else{
        visibility_radius_ = value;
    }
}
void PlayerRobot::SetNowXP(int value) {
    if (value < 0)
        value = 0;
    now_xp_ = value;
}

void PlayerRobot::AddNowXP(int value) {
    SetNowXP(now_xp_+value);
}

int PlayerRobot::CalculateXPForRank(int rank){
    return rank*15;
}

void PlayerRobot::RankUp () {
    while(now_xp_ >= xp_required){
        rank_++;
        now_xp_ -= xp_required;
        
        xp_required = CalculateXPForRank(rank_);
        max_health_ += 50;
        max_energy_ += 50;
        damage_ += 50;
    }
}

