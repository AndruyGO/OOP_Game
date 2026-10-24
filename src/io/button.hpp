#ifndef BUTTON_H
#define BUTTON_H

#include <functional>
#include "../core/commands.hpp"
#include "../core/position.hpp"

class Button {
public:
enum class ButtonTypes {
    kNone,
    kRankUp,
    kAbilityButtons
};
    Button(int width, int height, Position position, ButtonTypes type,
        std::function<Command()> on_click = nullptr, std::function<void()> render_button = nullptr,
        std::function<void()> update_fade = nullptr);
    Button();


    int Width() const;
    int Height() const;
    Position Pos() const;

    bool Contains(int x, int y) const;
    bool Contains(Position pos) const;
    void SetOnClick(std::function<Command()> callback);
    Command OnClick();
    void SetRenderMethod(std::function<void()> callback);
    void SetFadeUpdate(std::function<void()> callback);
    //void UpdateFade();
    void RenderButton();
    bool Fade();
    const ButtonTypes Type() const;


    
    bool is_fade;
    private:
    int width_;
    int height_;
    Position position_;
    ButtonTypes type_;
    std::function<Command()> on_click_;
    std::function<void()> render_button_;
    std::function<void()> update_fade_;
};

#endif