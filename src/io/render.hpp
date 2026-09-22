
#ifndef RENDER_H
#define RENDER_H

#include <SFML/Graphics.hpp>
#include "../core/game.hpp"

#define SHOW_NONVISITED_CELLS 0

class Render {
public:
    explicit Render(sf::RenderWindow& window);
    ~Render() = default;
    
    void Draw(const Game& game);
    
    
    private:
    float tile_size_;

    void DrawField(const GameField& field, const PlayerRobot& player);
    void DrawPlayer(const PlayerRobot& player);
    void DrawEnemies(const std::list<EnemyRobot>& robots, const GameField& field, const PlayerRobot& player);
    void DrawBuildings(const std::list<RobotFactory>& factories, const GameField& field, const PlayerRobot& player);

    void DrawCenteredText(const std::string& str, sf::Color color);



private:
    sf::RenderWindow& window_;
    sf::Font font_;
};

#endif