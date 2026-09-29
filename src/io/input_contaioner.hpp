#ifndef INPUT_CONTAINER_H
#define INPUT_CONTAINER_H

#include "../core/commands.hpp"
#include "../core/position.hpp"

class InputContainer {
public:
    Command command;
    Position real_mouse_position;
    Position field_mouse_position;

    InputContainer(Command command, Position mouse_position, Position field_mouse_position);
    InputContainer();
    ~InputContainer() = default;
    InputContainer(const InputContainer& other);
    InputContainer& operator = (const InputContainer &other);
};

#endif