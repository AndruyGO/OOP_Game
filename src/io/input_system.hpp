#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../core/commands.hpp"

class InputSystem {
public:
    InputSystem();
    ~InputSystem() = default;
    
    void HandleEvent(const sf::Event& event);
    Command ReadCommand();
    bool HasCommand() const;

private:
    Command pending_;
};


#endif
