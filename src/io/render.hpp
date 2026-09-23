
#ifndef RENDER_H
#define RENDER_H

#include <SFML/Graphics.hpp>
#include "../core/game.hpp"
#include <string>

#define SHOW_NONVISITED_CELLS 0

class Render {
public:
    static constexpr int kTileSize = 16;

    explicit Render(sf::RenderWindow& window);
    ~Render() = default;
    
    void Draw(const Game& game);
    
    
    private:
    float tile_size_;

    void DrawField(const GameField& field, const PlayerRobot& player);
    void DrawPlayer(const PlayerRobot& player);
    void DrawEnemies(const std::list<EnemyRobot>& robots, const GameField& field, const PlayerRobot& player);
    void DrawBuildings(const std::list<RobotFactory>& factories, const GameField& field, const PlayerRobot& player);
    void DrawBar(Position pos, double now, double max, sf::Color color, sf::Color text_color);
    void DrawUI(const Game& game);
    void DrawText(Position pos, const std::string &str, int size, sf::Color color);

    void DrawCenteredText(const std::string& str, sf::Color color);



private:
    sf::Texture tiles_texture_;
    sf::RenderWindow& window_;
    sf::Font font_;
};

#endif