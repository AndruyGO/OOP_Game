#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "input_contaioner.hpp"

class InputSystem {
public:
    InputSystem();
    ~InputSystem() = default;
    
    void HandleEvent(const sf::Event& event);
    InputContainer ReadCommand(float tile_size);
    bool HasCommand() const;

private:
    InputContainer pending_;
    Position mouse_position_;
};


#endif
