#include "input_system.hpp"
#include <iostream>
InputSystem::InputSystem() : pending_() {}

void InputSystem::HandleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        mouse_position_ = {event.mouseMove.x, event.mouseMove.y};
    }

    if (event.type == sf::Event::MouseButtonPressed) {
         if (event.mouseButton.button == sf::Mouse::Left) {
            mouse_position_ = {event.mouseButton.x, event.mouseButton.y};
        if (event.mouseButton.button == sf::Mouse::Left) {
            pending_.command = Command::kSelectTarget;
            pending_.real_mouse_position = mouse_position_;
        }
        } else if (event.mouseButton.button == sf::Mouse::Right) {
            pending_.command = Command::kCancel;
        }
        
        return;
    }

    if (event.type != sf::Event::KeyPressed)
        return;
    switch (event.key.code) {
        case sf::Keyboard::W:      pending_.command = Command::kMoveUp;    break;
        case sf::Keyboard::S:      pending_.command = Command::kMoveDown;  break;
        case sf::Keyboard::A:      pending_.command = Command::kMoveLeft;  break;
        case sf::Keyboard::D:      pending_.command = Command::kMoveRight; break;
        case sf::Keyboard::Q:      pending_.command = Command::kWait;      break;
        case sf::Keyboard::Z:      pending_.command = Command::kUseAreaStrike;      break;
        case sf::Keyboard::X:      pending_.command = Command::kUseHeal;   break;
        case sf::Keyboard::C:      pending_.command = Command::kUseFarHit; break;
        case sf::Keyboard::V:      pending_.command = Command::kUseTeleport; break;
        case sf::Keyboard::Escape: pending_.command = Command::kQuit;      break;
        default: break;
    }
}

InputContainer InputSystem::ReadCommand(float tile_size) {
    InputContainer input = pending_;
    input.field_mouse_position.SetPosition(mouse_position_.X()/tile_size,
                                           mouse_position_.Y()/tile_size);
    input.real_mouse_position = mouse_position_;
    
    pending_ = InputContainer();

    return input;
}

bool InputSystem::HasCommand() const {
    return pending_.command != Command::kNone;
}