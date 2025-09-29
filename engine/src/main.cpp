/**
 * @file main.cpp
 * @brief Entry point for the Illusionary Bird game
 * @author Juan Torres
 * @date 2024
 * @ingroup MainFlow
 * This file contains the main function that initializes and runs
 * the Illusionary Bird game. It manages the game's lifecycle through
 * initialization, execution, and cleanup phases using the Game singleton.
 */

#include <iostream>
#include "Game/Game.hpp"

 /**
  * @brief Program entry point
  *
  * Initializes the game engine, runs the main game loop, and performs cleanup
  * before program termination. Uses the Game singleton pattern to manage
  * the game instance.
  *
  * @return int Returns 0 on successful execution
  */
int main(int argc, char* argv[]) {
    // Suppress unused parameter warnings
    (void)argc;
    (void)argv;

    std::cout << "Videogame" << std::endl;

    // Get the singleton instance of the game
    Game& game = Game::GetInstance();

    // Initialize game systems
    game.init();

    // Run the main game loop
    game.run();

    // Cleanup and shutdown
    game.destroy();

    return 0;
}