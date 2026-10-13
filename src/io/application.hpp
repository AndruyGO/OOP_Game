#ifndef APPLICATION_H
#define APPLICATION_H


#include <SFML/Graphics.hpp>
#include "../core/game.hpp"
#include "gui.hpp"

class Application {
public:
    Application(Game game);
    ~Application() = default;
    
    void Run();

private:
    sf::RenderWindow window_;
    Game game_;
    GUI gui_;
};



#endif
