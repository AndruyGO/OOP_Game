#include "player_robot.hpp"
#include "enemy_robot.hpp"

PlayerRobot::PlayerRobot(int id, int max_health, int max_energy, int damage,
                        bool is_friendly, int speed, int rank, int visibility_radius)
  : Robot(id, max_health, max_energy, damage, is_friendly, speed, visibility_radius),
    rank_(rank) {
        is_friendly_ = 1;

        now_xp_ = 0;
        xp_required = CalculateXPForRank(rank_);
        max_health_ = 50 + 50*rank_;
        max_energy_ += 50 + 50*rank_;
        damage_ = 50*rank_;
    }

PlayerRobot::PlayerRobot()
  : Robot(0, 100, 100, 50, 1, 1, 3),
    rank_(1) {
        xp_required = CalculateXPForRank(rank_);
        max_health_ = 50 + 50*rank_;
        max_energy_ += 50 + 50*rank_;
        damage_ = 50*rank_;
    }

int PlayerRobot::Rank() const { return rank_; }

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

        now_energy_ = max_energy_;
        now_health_ = max_health_;
    }
}

bool PlayerRobot::IsEnoughXP() const {
    return now_xp_ >= xp_required;
}