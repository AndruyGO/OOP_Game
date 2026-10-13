#ifndef BUTTON_H
#define BUTTON_H

#include <functional>
#include "../core/position.hpp"

class Button {
public:
enum class ButtonTypes {
    kNone,
    kRankUp
};
    Button(int width, int height, Position position, ButtonTypes type,
        std::function<void()> on_click = nullptr, std::function<void()> render_button = nullptr);
    Button();


    int Width() const;
    int Height() const;
    Position Pos() const;

    bool Contains(int x, int y) const;
    bool Contains(Position pos) const;
    void SetOnClick(std::function<void()> callback);
    void OnClick();
    void SetRenderMethod(std::function<void()> callback);
    void RenderButton();
    const ButtonTypes Type() const;


    bool is_fade;

private:
    int width_;
    int height_;
    Position position_;
    ButtonTypes type_;
    std::function<void()> on_click_;
    std::function<void()> render_button_;
};

#endif