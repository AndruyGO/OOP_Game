#include "render.hpp"

Render::Render(sf::RenderWindow& window) : window_(window) {}

void Render::Draw(const Game& game) {

    tile_size_ = std::max(window_.getSize().x, window_.getSize().y)
                / std::max(game.Field().Height(), game.Field().Width());
            
    if (!font_.loadFromFile("../assets/font.ttf"))
        throw std::runtime_error("Font not loaded");

    window_.clear(sf::Color::Black);
    DrawField(game.Field());
    DrawPlayer(game.Player());
    DrawEnemies(game.Robots(), game.Field());
    if(game.GameStatus() == Game::GameStatuses::kGameOver){
        DrawCenteredText("Game Over", sf::Color::Red);
    } else if(game.GameStatus() == Game::GameStatuses::kGamePassed) {
        DrawCenteredText("Player Won", sf::Color::Green);
    }
    window_.display();
}

void Render::DrawField(const GameField& field) {
    for (int y = 0; y < field.Height(); ++y) {
        for (int x = 0; x < field.Width(); ++x) {
            sf::RectangleShape tile({tile_size_ - 1.f, tile_size_ - 1.f});
            tile.setPosition(x * tile_size_, y * tile_size_);
            if(field.GetCell(x, y).IsVisited() || SHOW_NONVISITED_CELLS) {
                switch(field.GetCell(x, y).Type()) {
                    case 0: tile.setFillColor(sf::Color(30, 30, 30)); break;
                    case 1: tile.setFillColor(sf::Color(80, 80, 80)); break;
                    case 2: tile.setFillColor(sf::Color(150, 90, 60)); break;
                    default: tile.setFillColor(sf::Color(0, 0, 0)); break;
                }
                window_.draw(tile);
            }
        }
    }
}

void Render::DrawPlayer(const PlayerRobot& player) {
    sf::CircleShape shape(tile_size_ / 2.f - 1.f);
    shape.setPosition(player.NowPosition().X() * tile_size_,
                      player.NowPosition().Y() * tile_size_);
    shape.setFillColor(sf::Color::Green);
    window_.draw(shape);
}

void Render::DrawEnemies(const std::list<EnemyRobot>& robots, const GameField& field) {
    for (const auto& r : robots) {
        if(field.GetCell(r.NowPosition()).IsVisited() || SHOW_NONVISITED_CELLS) {
            sf::CircleShape shape(tile_size_ / 2.f - 1.f);
            shape.setPosition(r.NowPosition().X() * tile_size_,
                            r.NowPosition().Y() * tile_size_);
            shape.setFillColor(sf::Color::Red);
            window_.draw(shape);
        }
    }
}


void Render::DrawCenteredText(const std::string& str, sf::Color color) {
    sf::Text text(str, font_, 72);
    sf::FloatRect b = text.getLocalBounds();
    text.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
    text.setPosition(window_.getSize().x / 2.f, window_.getSize().y / 2.f);
    text.setFillColor(color);
    window_.draw(text);
}