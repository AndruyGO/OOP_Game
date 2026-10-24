#include "button.hpp"

Button::Button(int width, int height, Position position,  ButtonTypes type,
    std::function<Command()> on_click, std::function<void()> render_button, std::function<void()> update_fade)
    : is_fade(0), width_(width), height_(height), position_(position), type_(type)
    {
        SetOnClick(on_click);
        SetRenderMethod(render_button);
    }
Button::Button() {}

int Button::Width() const { return width_; }
int Button::Height() const { return height_; }
Position Button::Pos() const { return position_; }

bool Button::Contains(int x, int y) const {
    return x >= position_.X() && x <= position_.X() + width_ &&
           y >= position_.Y() && y <= position_.Y() + height_;
}
bool Button::Contains(Position pos) const {
    return Contains(pos.X(), pos.Y());
}

void Button::SetOnClick(std::function<Command()> callback) {
    on_click_ = std::move(callback);
}

Command Button::OnClick() {
    if (on_click_) return on_click_();
    return Command::kNone;
}

void Button::SetRenderMethod(std::function<void()> callback) {
    render_button_ = std::move(callback);
}
void Button::RenderButton() {
    if (render_button_) render_button_();
}

const Button::ButtonTypes Button::Type() const { return type_; }


void Button::SetFadeUpdate(std::function<void()> callback) {
    update_fade_ = std::move(callback);
}

bool Button::Fade() {
    update_fade_();

    return is_fade;
}
