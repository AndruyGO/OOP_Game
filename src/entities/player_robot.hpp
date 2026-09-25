
#ifndef PLAYER_ROBOT_H
#define PLAYER_ROBOT_H
#include "robot.hpp"

class PlayerRobot : public Robot {

private:
    int rank_, now_xp_, xp_required, visibility_radius_;

public:
    PlayerRobot(int id, int max_health, int max_energy,
        int damage, bool is_friendly, int speed, int rank, int visibility_radius);
    PlayerRobot();
    ~PlayerRobot() = default;

    PlayerRobot(const PlayerRobot&) = delete;
    PlayerRobot& operator=(const PlayerRobot&) = delete;
    PlayerRobot(PlayerRobot&&) = default;
    PlayerRobot& operator=(PlayerRobot&&) = default;

    int Rank() const;
    int NowXp() const;
    int XpRequired() const;
    int VisibilityRadius() const;

    void SetNowXP(int value);
    void AddNowXP(int value);
    void SetVisibilityRadius(int value);
    
    int CalculateXPForRank(int rank);
    void RankUp();

};


#endif