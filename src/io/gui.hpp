#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "input_system.hpp"
#include "render.hpp"
#include "../core/game.hpp"
#include "button.hpp"

class GUI {
private:
    sf::RenderWindow &window_;
    Game& game_;
    Render render_;
    InputSystem input_system_;
    std::list<Button> buttons_;
    

    void UpdateButtonsFade();
    void AddRankUpButtons();
    void RemoveRankUpButtons();
public:
    
    GUI(sf::RenderWindow &window, Game& game);

    InputSystem& Input();
    Render& Renderer();

    void Draw();
    InputContainer GetCommand();



};


#endif