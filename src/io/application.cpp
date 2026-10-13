#include "application.hpp"

#include <iostream>
#include <utility>

Application::Application(Game game)
    :
      window_(sf::VideoMode(1100, 800), "Game",
              sf::Style::Titlebar | sf::Style::Close),
      game_(std::move(game)),
      gui_(window_, game_)
              {
                game_.SetInputStatus(Game::InputStatuses::kWaitingForRankUp);
              }

void Application::Run() {
    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window_.close();
            gui_.Input().HandleEvent(event);
        }

        InputContainer cmd = gui_.GetCommand();
        
        if (cmd.command == Command::kQuit) {
            window_.close();
            break;
        }
        
        if(game_.GameStatus() == Game::GameStatuses::kGameGoing) game_.Move(cmd);
        gui_.Draw();
    }
}