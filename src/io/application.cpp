#include "application.hpp"

Application::Application(Game game)
    : window_(sf::VideoMode(1100, 800), "Game",
              sf::Style::Titlebar | sf::Style::Close),
      game_(game),
      render_(window_),
      input_() {}

void Application::Run() {
    while (window_.isOpen()) { /// or if the end
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window_.close();
            input_.HandleEvent(event);
        }

        Command cmd = input_.ReadCommand();
        
        if (cmd == Command::kQuit) {
            window_.close();
            break;
        }
        
        if(game_.GameStatus() == Game::GameStatuses::kGameGoing) game_.Move(cmd);
            
        render_.Draw(game_);
    }
}