/**
 * @file Game.hpp
 * @brief Game Flow Controller
 * @author Juan Torres
 * @date 2024
 * @defgroup MainFlow Main Game Flow
 * @{
 * @brief Manages the main flow of the game
 */

#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <sol/sol.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../AnimationManager/AnimationManager.hpp"
#include "../AssetManager/AssetManager.hpp"
#include "../ControllerManager/ControllerManager.hpp"
#include "../EventManager/EventManager.hpp"
#include "../ECS/ECS.hpp"
#include "../SceneManager/SceneManager.hpp"

/**
 * @brief Target frames per second for the game
 */
const int FPS = 60;

/**
 * @brief Milliseconds per frame based on target FPS
 */
const int MSPerFrame = 1000 / FPS;

/**
 * @class Game
 * @brief Main game engine class implementing the Singleton pattern
 *
 * The Game class serves as the core engine, managing the game loop, window creation,
 * rendering, and various manager classes for assets, events, and scene handling.
 */
class Game {
private:
    /** @brief SDL window pointer */
    SDL_Window* window = nullptr;

    /** @brief Camera rectangle defining the viewport */
    SDL_Rect camera = { 0,0,0,0 };

    /** @brief Time tracking for frame rate control */
    int msPreviousFrame = 0;

    /** @brief Flag indicating if the game is currently running */
    bool isRunning = false;

    /** @brief Flag indicating if the game is paused */
    bool isPaused = false;

    /** @brief Flag indicating if the game is being debugged */
    bool isDebugMode = false;

public:
    /** @brief SDL renderer pointer used for drawing */
    SDL_Renderer* renderer = nullptr;

    /** @brief Manager for handling game animations*/
    std::unique_ptr<AnimationManager> animationManager;

    /** @brief Manager for handling game assets (textures, fonts, etc.) */
    std::unique_ptr<AssetManager> assetManager;

    /** @brief Manager for handling input controllers */
    std::unique_ptr<ControllerManager> controllerManager;

    /** @brief Manager for handling game events */
    std::unique_ptr<EventManager> eventManager;

    /** @brief Registry for the Entity Component System */
    std::unique_ptr<Registry> registry;

    /** @brief Manager for handling different game scenes */
    std::unique_ptr<SceneManager> sceneManager;

    /** @brief Lua state for scripting support */
    sol::state lua;

    /** @brief Width of the game window */
    int windowWidth = 0;

    /** @brief Height of the game window */
    int windowHeight = 0;

    /** @brief Width of the game map/level */
    int mapWidth = 0;

    /** @brief Height of the game map/level */
    int mapHeight = 0;

private:
    /**
     * @brief Process user input and events
     */
    void processInput();

    /**
     * @brief Update game state
     */
    void update();

    /**
     * @brief Render the current frame
     */
    void render();

    /**
     * @brief Set up initial game state
     */
    void setup();

    /**
     * @brief Execute the current scene
     */
    void RunScene();

    /**
     * @brief Private constructor for singleton pattern
     */
    Game();

    /**
     * @brief Destructor
     */
    ~Game();

public:
    /**
     * @brief Get the singleton instance of the Game class
     * @return Reference to the Game instance
     */
    static Game& GetInstance();

    /**
     * @brief Initialize the game engine
     *
     * Sets up SDL, creates window and renderer, initializes all managers
     */
    void init();

    /**
     * @brief Start the game loop
     *
     * Handles the main game loop including input processing, updates, and rendering
     */
    void run();

    /**
     * @brief Clean up and shut down the game
     *
     * Releases all resources, destroys window and renderer, and quits SDL
     */
    void destroy();
};

#endif // GAME_HPP

/** @} */ // end of group