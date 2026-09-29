#include "input_contaioner.hpp"

InputContainer::InputContainer(Command command, Position real_mouse_position, Position field_mouse_position)
  : command(command),
    real_mouse_position(real_mouse_position),
    field_mouse_position(field_mouse_position) {}

InputContainer::InputContainer()
  : command(Command::kNone),
    real_mouse_position(0, 0),
    field_mouse_position(0, 0) {}
InputContainer::InputContainer(const InputContainer& other)
    : command(other.command),
      real_mouse_position(other.real_mouse_position),
      field_mouse_position(other.field_mouse_position) {}


InputContainer& InputContainer::operator = (const InputContainer &other) {
    if (this == &other) return *this;
    command = other.command;
    real_mouse_position = other.real_mouse_position;
    field_mouse_position = other.field_mouse_position;
    return *this;
}