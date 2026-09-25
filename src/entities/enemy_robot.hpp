#ifndef ENEMY_ROBOT_H
#define ENEMY_ROBOT_H
#include "robot.hpp"

class EnemyRobot : public Robot {
public:
    EnemyRobot(int id, int max_health, int max_energy,
        int damage, bool is_friendly, int speed);
    EnemyRobot();
    ~EnemyRobot() = default;

    EnemyRobot(const EnemyRobot&) = delete;
    EnemyRobot& operator=(const EnemyRobot&) = delete;
    EnemyRobot(EnemyRobot&&) = default;
    EnemyRobot& operator=(EnemyRobot&&) = default;

};


#endif