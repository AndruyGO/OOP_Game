
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
    
    void DrawField(const Game& game);
    void DrawPlayer(const Game& game);
    void DrawEnemies(const Game& game);
    void DrawBuildings(const Game& game);
    void DrawBar(Position pos, double now, double max, sf::Color color, sf::Color text_color);
    void DrawUI(const Game& game);
    void DrawText(Position pos, const std::string &str, int size, sf::Color color);
    
    void DrawCenteredText(const std::string& str, sf::Color color);
    void DrawAbilityRadius(const Game &game);
    
    public:
    float TileSize() const;
    
    
    
    private:
    sf::Texture tiles_texture_;
    sf::Texture robot_factory_;
    sf::RenderWindow& window_;
    sf::Font font_;
};

#endif