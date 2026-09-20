#ifndef APPLICATION_H
#define APPLICATION_H


#include <SFML/Graphics.hpp>
#include "../core/game.hpp"
#include "render.hpp"
#include "input_system.hpp"

class Application {
public:
    Application(Game game);
    ~Application() = default;
    
    void Run();

private:
    sf::RenderWindow window_;
    Game game_;
    Render render_;
    InputSystem input_;
};



#endif
