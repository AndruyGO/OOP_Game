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

    UpdateButtonsFade();

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
        Command com;
        int t;
    };

    const std::vector<RankUpOption> options = {
        {{50+(W+F*2+20)*0, 150},  Ability::AbilityType::kAreaStrike, Command::kUseAreaStrike, 0},
        {{50+(W+F*2+20)*1, 150},  Ability::AbilityType::kFarHit, Command::kUseFarHit, 1},
        {{50+(W+F*2+20)*2, 150},  Ability::AbilityType::kHeal, Command::kUseHeal, 2},
        {{50+(W+F*2+20)*3, 150}, Ability::AbilityType::kTeleport, Command::kUseTeleport, 3}
    };

    for (const auto& opt : options) {
        buttons_.push_back(Button(W, H, opt.pos,
                                  Button::ButtonTypes::kRankUp));

        Button& button = buttons_.back();

        button.SetRenderMethod([this, opt, &button]() {
            render_.DrawAbilityCard(opt.pos, W, H, F, opt.type, game_, button.Fade());
        });

        button.SetOnClick([this, opt]() {
            game_.AddAbility(game_.Player(), opt.type);

            int start_x = window_.getSize().y + 30;
            int start_y = window_.getSize().y - 250;
            int spacing = 32*3 + 20;

            int col = opt.t % 2;
            int row = opt.t / 2;
            Position new_pos(start_x + col * spacing, 
                            start_y + row * spacing);
            
            
            buttons_.push_back(Button(
                32*3, 32*3, new_pos, Button::ButtonTypes::kAbilityButtons,
                
                [this, opt]() {
                    return opt.com;
                }
            ));
            Button& ability_button = buttons_.back();
            ability_button.SetRenderMethod(
                [this, opt, &ability_button]() { 
                    render_.DrawAbilityButton(
                        ability_button.Pos(),
                        3, opt.type, 
                        ability_button.Fade()
                    );
                }
            );
            ability_button.SetFadeUpdate([this, &ability_button]{
                if(game_.GameStatus() == Game::GameStatuses::kGameGoing &&
                   game_.InputStatus() == Game::InputStatuses::kStandard &&
                   ability_button.Contains(input_system_.MousePosition()) )
                    {
                    ability_button.is_fade = 1;
                }else {
                    ability_button.is_fade = 0;
                }
            }); 

            RemoveRankUpButtons();
        });

        button.SetFadeUpdate([this, &button]{
            if(button.Contains(input_system_.MousePosition())) {
                button.is_fade = 1;
            }else {
                button.is_fade = 0;
            }
        }); 
    }
}
void GUI::RemoveRankUpButtons() {
    buttons_.remove_if([](const Button& button) {
        return button.Type() == Button::ButtonTypes::kRankUp;
    });
    
    game_.SetInputStatus(Game::InputStatuses::kStandard);
}

InputContainer GUI::GetCommand() {
    InputContainer cmd = input_system_.ReadCommand(render_.TileSize());

    if(cmd.command == Command::kSelectTarget) {
        for(auto& button : buttons_) {
            if(game_.InputStatus() == Game::InputStatuses::kStandard 
            ||(game_.InputStatus() == Game::InputStatuses::kWaitingForRankUp && button.Type() == Button::ButtonTypes::kRankUp)) {
                if(button.Contains(cmd.real_mouse_position)) {
                    return InputContainer(button.OnClick(), cmd.real_mouse_position, cmd.real_mouse_position);
                }
            }
        }
    }

    return cmd;
}

void GUI::UpdateButtonsFade() {
    Position mouse_pos = input_system_.MousePosition();
    for(auto& button : buttons_) {
        button.Fade();
        /*
        if( 
        ||(game_.InputStatus() == Game::InputStatuses::kWaitingForRankUp && button.Type() == Button::ButtonTypes::kRankUp)) {
            if(button.Contains(mouse_pos)) {
                button.is_fade = 1;
                //break;
            } else {
                button.is_fade = 0;
            }
        }
        */
    }
}


