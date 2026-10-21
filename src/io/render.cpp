#include "render.hpp"
#include <iostream>
Render::Render(sf::RenderWindow& window) : window_(window) {

    if (!font_.loadFromFile("../assets/font.ttf"))
        throw std::runtime_error("Font not loaded");
    if (!tiles_texture_.loadFromFile("../assets/tiles.png"))
        throw std::runtime_error("Tiles not loaded");
    if(!robot_factory_.loadFromFile("../assets/robot_factory.png"))
        throw std::runtime_error("Tiles not loaded");
    if(!abilities_texture_.loadFromFile("../assets/abilities_texture.png"))
        throw std::runtime_error("Icons not loaded");
    if(!statuses_texture_.loadFromFile("../assets/statuses.png"))
        throw std::runtime_error("Statuses not loaded");
}

void Render::Draw(const Game& game) {
    tile_size_ = std::min(window_.getSize().x, window_.getSize().y)
                / std::min(game.Field().Height(), game.Field().Width());
            
    

    window_.clear(sf::Color::Black);
    DrawField(game);
    DrawPlayer(game);
    DrawEnemies(game);
    DrawBuildings(game);

    if(game.GameStatus() == Game::GameStatuses::kGameOver){
        DrawCenteredText("Game Over", sf::Color::Red);
    } else if(game.GameStatus() == Game::GameStatuses::kGamePassed) {
        DrawCenteredText("Player Won", sf::Color::Green);
    }
    DrawUI(game);
    DrawPlayerStatuses(game);

    if(game.InputStatus() == Game::InputStatuses::kWaitingForMouseToAbility) {
        DrawAbilityRadius(game);
    }
    //DrawAbilityCard({100,100}, Ability::AbilityType::kHeal, game);
}

void Render::DrawAbilityRadius(const Game &game) {
    const GameField& field = game.Field();
    const PlayerRobot& player = game.Player();
    const int radius = (*game.GetAbility(player, game.LastAbility())).Radius();
    for (int y = 0; y < field.Height(); ++y) {
        for (int x = 0; x < field.Width(); ++x) { 
            if(player.NowPosition().DistanceTo(x, y) <= radius) {
                sf::RectangleShape fadetile({tile_size_ , tile_size_});
                fadetile.setPosition(x * tile_size_, y * tile_size_);
                fadetile.setFillColor(sf::Color(128, 128, 128, 128));
                window_.draw(fadetile);
            } else {
                sf::RectangleShape fadetile({tile_size_ , tile_size_});
                fadetile.setPosition(x * tile_size_, y * tile_size_);
                fadetile.setFillColor(sf::Color(0, 0, 0, 128));
                window_.draw(fadetile);
            }
        }
    }
}


float Render::TileSize() const { return tile_size_; }

void Render::DrawField(const Game &game) {
    const GameField& field = game.Field();
    const PlayerRobot& player = game.Player();
    for (int y = 0; y < field.Height(); ++y) {
    for (int x = 0; x < field.Width(); ++x) {
        sf::RectangleShape tile({tile_size_ - 1.f, tile_size_ - 1.f});
        tile.setPosition(x * tile_size_, y * tile_size_);
        if(field.GetCell(x, y).IsVisited() || SHOW_NONVISITED_CELLS) {
            
            sf::Sprite tile(tiles_texture_);
            tile.setTextureRect(sf::IntRect(
                field.GetCell(x, y).Type() * kTileSize,   // x в атласе
                0,                  // y в атласе
                kTileSize,          // ширина
                kTileSize           // высота
            ));

            /*
            switch(field.GetCell(x, y).Type()) {
                case 0: tile.setFillColor(sf::Color(30, 30, 30)); break;
                case 1: tile.setFillColor(sf::Color(80, 80, 80)); break;
                case 2: tile.setFillColor(sf::Color(150, 90, 60)); break;
                default: tile.setFillColor(sf::Color(0, 0, 0)); break;
            }
            */
            tile.setPosition(x * tile_size_, y * tile_size_);
            float scale = tile_size_ / kTileSize;
            tile.setScale(scale, scale);
            
            window_.draw(tile);
            
            if(player.NowPosition().DistanceTo(x, y) > player.VisibilityRadius()) {
                sf::RectangleShape fadetile({tile_size_ , tile_size_});
                fadetile.setPosition(x * tile_size_, y * tile_size_);
                fadetile.setFillColor(sf::Color(0, 0, 0, 128));
                window_.draw(fadetile);
            } else {

                // statuses
                int offset = 0;
                int t = 0;
                for (auto status : field.GetCell(x, y).GetStatuses()) {

                    switch (status->GetStatusType()) {
                        case Status::StatusType::kBurning: t = 0; break;
                        case Status::StatusType::kOverload: t = 1; break;
                        case Status::StatusType::kShield:   t = 2; break;
                        case Status::StatusType::kSlowness: t = 3; break;
                    }
                    
                    sf::Sprite status_tile(statuses_texture_);
                    
                    status_tile.setTextureRect(sf::IntRect(
                        t*16,   // x в атласе
                        0,      // y в атласе
                        16,     // ширина
                        16      // высота
                    ));
                    
                    status_tile.setPosition(x * tile_size_ + offset, y * tile_size_);
                    float scale = ( tile_size_ / kTileSize ) / 4;
                    status_tile.setScale(scale, scale);
            
                    window_.draw(status_tile);
                    
                    offset+=tile_size_/4;
                }
            }
            

        }
    }
    }
}

void Render::DrawPlayer(const Game &game) {
    const PlayerRobot& player = game.Player();
    sf::CircleShape shape(tile_size_ / 2.f - 1.f);
    shape.setPosition(player.NowPosition().X() * tile_size_,
                      player.NowPosition().Y() * tile_size_);
    shape.setFillColor(sf::Color::Green);
    window_.draw(shape);
}

void Render::DrawEnemies(const Game &game) {
    const std::list<EnemyRobot>& robots = game.Robots(); const GameField& field = game.Field();
    const PlayerRobot& player = game.Player();
    for (const auto& r : robots) {
        if(field.GetCell(r.NowPosition()).IsVisited() || SHOW_NONVISITED_CELLS) {
            sf::CircleShape shape(tile_size_ / 2.f - 1.f);
            shape.setPosition(r.NowPosition().X() * tile_size_,
                            r.NowPosition().Y() * tile_size_);
            switch(r.IsFriendly()){
                case false: shape.setFillColor(sf::Color::Red); break;
                case true: shape.setFillColor(sf::Color::Blue); break;
            }
            
            if(player.NowPosition().DistanceTo(r.NowPosition().X(), r.NowPosition().Y()) <= player.VisibilityRadius()) window_.draw(shape);
        }
    }
}

void Render::DrawBuildings(const Game &game) {
    const std::list<RobotFactory>& factories = game.Factories();
    const GameField& field = game.Field();
    const PlayerRobot& player = game.Player();

    for (const auto& b : factories) {
        Position pos = b.TopLeftPosition();
        for (int y = pos.Y(); y < pos.Y() + b.Size(); ++y) {
        for (int x = pos.X(); x < pos.X() + b.Size(); ++x) {
            sf::RectangleShape tile({tile_size_, tile_size_});
            tile.setPosition(x * tile_size_, y * tile_size_);
            if(field.GetCell(x, y).IsVisited() || SHOW_NONVISITED_CELLS) {
                
                sf::Sprite tile(robot_factory_);
                tile.setTextureRect(sf::IntRect(
                    (x - pos.X())*kTileSize,   // x в атласе
                    (y - pos.Y())*kTileSize,   // y в атласе
                    kTileSize,          // ширина
                    kTileSize           // высота
                ));

                tile.setPosition(x * tile_size_, y * tile_size_);
                float scale = tile_size_ / kTileSize;
                tile.setScale(scale, scale);
                
                window_.draw(tile);



                if(player.NowPosition().DistanceTo(x, y) > player.VisibilityRadius()) {
                    sf::RectangleShape fadetile({tile_size_, tile_size_});
                    fadetile.setPosition(x * tile_size_, y * tile_size_);
                    fadetile.setFillColor(sf::Color(0, 0, 0, 128));
                    window_.draw(fadetile);
                }
            }
        }
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


void Render::DrawText(Position pos, const std::string &str, int size, sf::Color color) {
    sf::Text text(str, font_, size);
    sf::FloatRect b = text.getLocalBounds();
    text.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
    text.setPosition(pos.X(), pos.Y());
    text.setFillColor(color);

    window_.draw(text);
}

void Render::DrawBar(Position pos, double now, double max, sf::Color bar_color, sf::Color text_color){

    sf::RectangleShape fill_bar({200+20, 50+20});
    fill_bar.setPosition(pos.X()-10, pos.Y()-10);
    fill_bar.setFillColor(sf::Color(150, 150, 150));

    sf::RectangleShape bar({((now/max)*200), 50});
    bar.setPosition(pos.X(), pos.Y());
    bar.setFillColor(bar_color);

    
    window_.draw(fill_bar);
    window_.draw(bar);
    DrawText({pos.X()+100, pos.Y()+25}, std::to_string((int)now)+" / "+std::to_string((int)max), 20, text_color);
}

void Render::DrawUI(const Game& game) {
    sf::RectangleShape cross_line({6, window_.getSize().y});
    cross_line.setPosition(window_.getSize().y, 0);
    cross_line.setFillColor(sf::Color(128, 128, 128));
    
    window_.draw(cross_line);

    sf::RectangleShape bg({window_.getSize().x - window_.getSize().y, window_.getSize().y});
    bg.setPosition(window_.getSize().y + 6, 0);
    bg.setFillColor(sf::Color(68, 68, 68));
    
    window_.draw(bg);

    sf::RectangleShape shadow_line({window_.getSize().x - window_.getSize().y, 6});
    shadow_line.setPosition(window_.getSize().y+6, window_.getSize().y - 6);
    shadow_line.setFillColor(sf::Color(58, 58, 58));
    window_.draw(shadow_line);

    sf::RectangleShape shadow_line2({6, window_.getSize().y});
    shadow_line2.setPosition(window_.getSize().x-6, 0);
    shadow_line2.setFillColor(sf::Color(58, 58, 58));
    window_.draw(shadow_line2);


    
    const PlayerRobot &player = game.Player();
    DrawBar({window_.getSize().y + 30, 100}, player.NowHealth(), player.MaxHealth(), sf::Color::Red, sf::Color::White);
    DrawBar({window_.getSize().y + 30, 200}, player.NowEnergy(), player.MaxEnergy(), sf::Color::Green, sf::Color(50, 50, 50));
    DrawText({window_.getSize().y + 70, 325}, "Rank : "+std::to_string((int)player.Rank()), 20, sf::Color::White);

}


void Render::DrawAbilityCard(Position pos, int W, int H, int F, Ability::AbilityType ability, const Game &game, bool is_fade, sf::Color bg_color) {
    sf::RectangleShape card_bg_frame({W+2*F, H+2*F});
    card_bg_frame.setPosition(pos.X()-F, pos.Y()-F);
    card_bg_frame.setFillColor(sf::Color(bg_color.r-10, bg_color.g-10, bg_color.b-10));

    sf::RectangleShape card_bg({W, H});
    card_bg.setPosition(pos.X(), pos.Y());
    card_bg.setFillColor(bg_color);

    const Ability *obj_ability = game.GetAbility(game.Player(), ability);
    std::string title;
    int t;
    sf::Sprite tile(abilities_texture_);
    

    switch(ability) {
        case Ability::AbilityType::kAreaStrike:
            title = "Area Strike";
            t = 0;
            break;
        case Ability::AbilityType::kFarHit:
            title = "Far Hit";
            t = 1;
            break;
        case Ability::AbilityType::kHeal:
            title = "Heal";
            t = 2;
            break;
        case Ability::AbilityType::kTeleport:
            title = "Teleport";
            t = 3;
            break;
    }
    tile.setTextureRect(sf::IntRect(
        t*32,   // x в атласе
        0,      // y в атласе
        32,     // ширина
        32      // высота
    ));

    tile.setPosition(pos.X()+F/2, pos.Y()+F+40);
    float scale = (W+2*F)/32;
    tile.setScale(scale, scale);
    
    

    std::string description;
    int level;
    if(obj_ability == nullptr) {
        description = "Unlock";
        level = 0;
        
    } else {
        level = obj_ability->Level();
        switch(obj_ability->GetAbilityType()) {
            case Ability::AbilityType::kAreaStrike:
                description = "Increase damage"; break;
            case Ability::AbilityType::kFarHit:
                description = "Increase radius"; break;
            case Ability::AbilityType::kTeleport:
                description = "Reduce energy\ncost"; break;
            case Ability::AbilityType::kHeal:
                description = "Increase Heal\nAmount";
                break;
        }
    }
    
    window_.draw(card_bg_frame);
    window_.draw(card_bg);
    window_.draw(tile);
    DrawText({pos.X()+W/2, pos.Y()+30}, title, 25, sf::Color::White);
    DrawText({pos.X()+W/2, pos.Y()+280}, description, 20, sf::Color::White);
    DrawText({pos.X()+W/2, pos.Y()+340}, "Level " + std::to_string(level+1), 20, sf::Color::White);

    if(is_fade) {
        sf::RectangleShape card_fade({W+2*F, H+2*F});
        card_fade.setPosition(pos.X()-F, pos.Y()-F);
        card_fade.setFillColor(sf::Color(10, 10, 10, 128));
        window_.draw(card_fade);
    }
}

void Render::DrawAbilityButton(Position pos, float scale, Ability::AbilityType ability, bool is_fade) {
    sf::Sprite tile(abilities_texture_);
    
    int t;
    switch(ability) {
        case Ability::AbilityType::kAreaStrike:
            t = 0;
            break;
        case Ability::AbilityType::kFarHit:
            t = 1;
            break;
        case Ability::AbilityType::kHeal:
            t = 2;
            break;
        case Ability::AbilityType::kTeleport:
            t = 3;
            break;
    }
    tile.setTextureRect(sf::IntRect(
        t*32,   // x в атласе
        0,      // y в атласе
        32,     // ширина
        32      // высота
    ));

    tile.setPosition(pos.X(), pos.Y());
    tile.setScale(scale, scale);

    window_.draw(tile);

    if(is_fade) {
        sf::RectangleShape card_fade({32*scale, 32*scale});
        card_fade.setPosition(pos.X(), pos.Y());
        card_fade.setFillColor(sf::Color(10, 10, 10, 128));
        window_.draw(card_fade);
    }

}

void Render::DrawPlayerStatuses(const Game& game) {
    const PlayerRobot &player = game.Player();

    int offset = 0;
    int t = 0;
    for (auto status : player.Statuses()) {

        switch (status->GetStatusType()) {
            case Status::StatusType::kBurning: t = 0; break;
            case Status::StatusType::kOverload: t = 1; break;
            case Status::StatusType::kShield:   t = 2; break;
            case Status::StatusType::kSlowness: t = 3; break;
        }
        
        sf::Sprite tile(statuses_texture_);
        
        tile.setTextureRect(sf::IntRect(
            t*16,   // x в атласе
            0,      // y в атласе
            16,     // ширина
            16      // высота
        ));
        
        tile.setPosition(window_.getSize().y + offset + 20, 20);
        tile.setScale(3, 3);
        window_.draw(tile);
        
        DrawText({window_.getSize().y + 30 + offset, 60}, std::to_string(status->Count()),12, sf::Color::White);
        
        offset+=60;
    }


}


void Render::Flush() { window_.display(); }
