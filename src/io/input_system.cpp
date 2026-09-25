#include "input_system.hpp"

InputSystem::InputSystem() : pending_(Command::kNone) {}

void InputSystem::HandleEvent(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed)
        return;
    switch (event.key.code) {
        case sf::Keyboard::W:      pending_ = Command::kMoveUp;    break;
        case sf::Keyboard::S:      pending_ = Command::kMoveDown;  break;
        case sf::Keyboard::A:      pending_ = Command::kMoveLeft;  break;
        case sf::Keyboard::D:      pending_ = Command::kMoveRight; break;
        case sf::Keyboard::Q:      pending_ = Command::kWait;      break;
        case sf::Keyboard::Z:      pending_ = Command::kUseAreaStrike;      break;
        case sf::Keyboard::Escape: pending_ = Command::kQuit;      break;
        default: break;
    }
}

Command InputSystem::ReadCommand() {
    Command command = pending_;
    pending_ = Command::kNone;
    return command;
}

bool InputSystem::HasCommand() const {
    return pending_ != Command::kNone;
}