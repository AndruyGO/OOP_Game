#include "robot.hpp"
#include "enemy_robot.hpp"

EnemyRobot::EnemyRobot(int id, int max_health, int max_energy,
    int damage, bool is_friendly, int speed)
    : Robot(id, max_health, max_energy, damage, is_friendly, speed) {}
    
EnemyRobot::EnemyRobot()
    : Robot(0, 100, 100, 50, 0, 1) {}
