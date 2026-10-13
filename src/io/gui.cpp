#include "gui.hpp"
#include <algorithm>
#include <iostream>

GUI::GUI(sf::RenderWindow &window) : window_(window),
              render_(window) {}

InputSystem& GUI::Input() { return input_system_; }

Render& GUI::Renderer() { return render_; }

void GUI::Draw(Game& game) { 
    render_.Draw(game);
    

    //std::cout << buttons_.size() << std::endl;
    //if(game.InputStatus() == Game::InputStatuses::kWaitingForRankUp) std::cout << "lol" << std::endl;
    if(game.InputStatus() == Game::InputStatuses::kWaitingForRankUp
       && std::find_if(buttons_.begin(), buttons_.end(),
       [](const Button& button) {return button.Type() == Button::ButtonTypes::kRankUp; } ) == buttons_.end()) {
        std::cout << "ok" << std::endl;
        AddRankUpButtons(game);
    }

    UpdateButtonsFade();

    for(auto& button : buttons_) {
        button.RenderButton();
    }

    render_.Flush();
}

void GUI::AddRankUpButtons(Game& game) {
    static constexpr int W = 200, H = 400, F = 10;
    struct RankUpOption {
        Position pos;
        Ability::AbilityType type;
    };

    const std::vector<RankUpOption> options = {
        {{50+(W+F*2+20)*0, 150},  Ability::AbilityType::kAreaStrike},
        {{50+(W+F*2+20)*1, 150},  Ability::AbilityType::kFarHit},
        {{50+(W+F*2+20)*2, 150},  Ability::AbilityType::kHeal},
        {{50+(W+F*2+20)*3, 150}, Ability::AbilityType::kTeleport},
    };

    for (const auto& opt : options) {
        buttons_.push_back(Button(W, H, opt.pos,
                                  Button::ButtonTypes::kRankUp));

        Button& button = buttons_.back();

        button.SetRenderMethod([this, &game, opt, &button]() {
            render_.DrawAbilityCard(opt.pos, W, H, F, opt.type, game, button.is_fade);
        });

        button.SetOnClick([this, &game, opt]() {
            game.AddAbility(game.Player(), opt.type);
            RemoveRankUpButtons(game);
        });
    }
}

void GUI::RemoveRankUpButtons(Game &game) {
   buttons_.erase(
        std::remove_if(buttons_.begin(), buttons_.end(),
            [](const Button& button) {
                return button.Type() == Button::ButtonTypes::kRankUp;
            }),
        buttons_.end()
    );
    game.SetInputStatus(Game::InputStatuses::kStandard);
}

InputContainer GUI::GetCommand() {
    InputContainer cmd = input_system_.ReadCommand(render_.TileSize());

    if(cmd.command == Command::kSelectTarget) {
        for(auto& button : buttons_) {
            if(button.Contains(cmd.real_mouse_position)) {
                button.OnClick();
                return InputContainer(Command::kNone, cmd.real_mouse_position, cmd.real_mouse_position);
            }
        }
    }

    return cmd;
}

void GUI::UpdateButtonsFade() {
    Position mouse_pos = input_system_.MousePosition();
    for(auto& button : buttons_) {
        if(button.Contains(mouse_pos)) {
            button.is_fade = 1;
            break;
        } else {
            button.is_fade = 0;
        }
    }
}


