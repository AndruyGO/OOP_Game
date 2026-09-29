
#ifndef ROBOT_FACTORY_H
#define ROBOT_FACTORY_H

#include "building.hpp"

class RobotFactory : public Building {
private:
    int coldown_;
    int moves_untill_next_move_;
    int robots_inside_;

public:
    RobotFactory(int id, Position top_left_position, bool is_invulnerable,
        int health, int coldown, bool is_friendly);
    ~RobotFactory() = default;

    void Move();
    
    void ResetMovesUntillNextMove();
    void SetMovesUntillNextMove(int value);
    int MovesUntillNextMove() const;

    int Coldown() const;
    void ReduceMovesUntillNextMove(int value);
    
    void AddRobotsInside(int value);
    int RobotsInside() const;
    void ReduceRobotsInside(int value);
    bool IsReady() const;

};




#endif