#include "gui.hpp"
#include <algorithm>
#include <iostream>

GUI::GUI(sf::RenderWindow &window, Game& game) :
            window_(window),
            game_(game),
            render_(window) {}

InputSystem& GUI::Input() { return input_system_; }

Render& GUI::Renderer() { return render_; }

void GUI::Draw() { 
    render_.Draw(game_);
    

    //std::cout << buttons_.size() << std::endl;
    //if(game.InputStatus() == Game::InputStatuses::kWaitingForRankUp) std::cout << "lol" << std::endl;
    if(game_.InputStatus() == Game::InputStatuses::kWaitingForRankUp
       && std::find_if(buttons_.begin(), buttons_.end(),
       [](const Button& button) {return button.Type() == Button::ButtonTypes::kRankUp; } ) == buttons_.end()) {

        AddRankUpButtons();
    }

    UpdateButtonsFade(game_.InputStatus());

    for(auto& button : buttons_) {
        button.RenderButton();
    }

    render_.Flush();
}

void GUI::AddRankUpButtons() {
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

        button.SetRenderMethod([this, opt, &button]() {
            render_.DrawAbilityCard(opt.pos, W, H, F, opt.type, game_, button.is_fade);
        });

        button.SetOnClick([this, opt]() {
            game_.AddAbility(game_.Player(), opt.type);
            RemoveRankUpButtons();
        });
    }
}

void GUI::RemoveRankUpButtons() {
   buttons_.erase(
        std::remove_if(buttons_.begin(), buttons_.end(),
            [](const Button& button) {
                return button.Type() == Button::ButtonTypes::kRankUp;
            }),
        buttons_.end()
    );
    game_.SetInputStatus(Game::InputStatuses::kStandard);
}

InputContainer GUI::GetCommand() {
    InputContainer cmd = input_system_.ReadCommand(render_.TileSize());

    if(cmd.command == Command::kSelectTarget) {
        for(auto& button : buttons_) {
            if(game_.InputStatus() == Game::InputStatuses::kStandard 
            ||(game_.InputStatus() == Game::InputStatuses::kWaitingForRankUp && button.Type() == Button::ButtonTypes::kRankUp)) {
                if(button.Contains(cmd.real_mouse_position)) {
                    button.OnClick();
                    return InputContainer(Command::kNone, cmd.real_mouse_position, cmd.real_mouse_position);
                }
            }
        }
    }

    return cmd;
}

void GUI::UpdateButtonsFade(Game::InputStatuses input_status_) {
    Position mouse_pos = input_system_.MousePosition();
    for(auto& button : buttons_) {
        if(input_status_ == Game::InputStatuses::kStandard 
        ||(input_status_ == Game::InputStatuses::kWaitingForRankUp && button.Type() == Button::ButtonTypes::kRankUp)) {
            if(button.Contains(mouse_pos)) {
                button.is_fade = 1;
                break;
            } else {
                button.is_fade = 0;
            }
        }
    }
}


