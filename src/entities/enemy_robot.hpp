#ifndef ENEMY_ROBOT_H
#define ENEMY_ROBOT_H
#include "robot.hpp"

class EnemyRobot : public Robot {
public:
    EnemyRobot(int id, int max_health, int max_energy,
        int damage, bool is_friendly, int speed);
    EnemyRobot();
    ~EnemyRobot() = default;

};


#endif