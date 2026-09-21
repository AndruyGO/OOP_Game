#include "robot_factory.hpp"


RobotFactory::RobotFactory(int id, Position top_left_position, bool is_invulnerable,
        int health, int coldown)
  : Building(id, top_left_position, 2, is_invulnerable, health, 1, true),
    coldown_(coldown), 
    moves_untill_next_move_(coldown),
    robots_inside_(0) {}
    
    

    

    void RobotFactory::Move() {
        ReduceMovesUntillNextMove(1);
        if(IsReady()) {
            robots_inside_++;
            ResetMovesUntillNextMove();
        }
    }
    
    void RobotFactory::AddRobotsInside(int value) {
        if(value < 0) return;
        robots_inside_ += value;
    }
    
    void RobotFactory::ResetMovesUntillNextMove() {
        moves_untill_next_move_ = coldown_;
    }
    void RobotFactory::SetMovesUntillNextMove(int value) {
        if(value < 0){
            moves_untill_next_move_ = 0;
        }else moves_untill_next_move_ = value;
    }
    int RobotFactory::MovesUntillNextMove() const {
        return moves_untill_next_move_; 
    }
    
    int RobotFactory::Coldown() const {
        return coldown_;
    }
    void RobotFactory::ReduceMovesUntillNextMove(int value) {
        if(moves_untill_next_move_ - value <= 0) { 
            moves_untill_next_move_ = 0;
            return;
        }
        moves_untill_next_move_ -= value;
    }
    
    
    bool RobotFactory::IsReady() const {
        return moves_untill_next_move_ == 0;
    }
    
    int RobotFactory::RobotsInside() const {
        return robots_inside_;
    }
    void RobotFactory::ReduceRobotsInside(int value) {
        if(robots_inside_ - value <= 0){
            robots_inside_ = 0;
            return;
        }
        robots_inside_ -= value;
    }