/**
 * @file SceneManager.hpp
 * @brief High-level scene management system for game engine
 * @author Juan Torres
 * @date 2024
 * @defgroup SceneManager Scene Management
 * @{
 * @brief Manages the loading process of Scenes into the game
 */

#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <memory>
#include <string>
#include <map>
#include <sol/sol.hpp>
#include "SceneLoader.hpp"
#include "../ECS/ECS.hpp"

 /**
  * @class SceneManager
  * @brief Manages scene lifecycle and transitions in the game engine
  *
  * The SceneManager class provides high-level scene management functionality,
  * including scene loading, transitions, and state management. It works with
  * Lua scripts to define and load scenes, and maintains the current scene state.
  */
class SceneManager {
private:
    /** @brief Map storing scene names and their corresponding file paths */
    std::map<std::string, std::string> scenes;

    /** @brief Pointer to the scene loader instance */
    std::unique_ptr<SceneLoader> sceneLoader;

    /** @brief Name of the next scene to be loaded */
    std::string nextScene;

    /** @brief Flag indicating if a scene is currently running */
    bool isSceneRunning = false;

public:
    /**
     * @brief Construct a new Scene Manager object
     *
     * Initializes the SceneManager and creates a new SceneLoader instance.
     */
    SceneManager();

    /**
     * @brief Destroy the Scene Manager object
     *
     * Cleans up the SceneLoader and releases resources.
     */
    ~SceneManager();

    /**
     * @brief Loads scene definitions from a Lua script
     * @param path Path to the Lua script containing scene definitions
     * @param lua Lua state instance
     *
     * Parses the Lua script and populates the scenes map with scene names
     * and their corresponding file paths. Sets the first scene as the next scene.
     */
    void LoadSceneFromScript(const std::string& path, sol::state& lua);

    /**
     * @brief Loads the next scene using the SceneLoader
     *
     * Gets the game instance and loads the scene specified by nextScene
     * using the SceneLoader.
     */
    void LoadScene();

    /**
     * @brief Gets the name of the next scene
     * @return std::string Name of the next scene to be loaded
     */
    std::string GetNextScene() const;

    /**
     * @brief Sets the next scene to be loaded
     * @param nextScene Name of the scene to set as next
     */
    void SetNextScene(const std::string& nextScene);

    /**
     * @brief Checks if a scene is currently running
     * @return true if a scene is running
     * @return false if no scene is running
     */
    bool IsSceneRunning() const;

    /**
     * @brief Marks the current scene as running
     */
    void StartScene();

    /**
     * @brief Marks the current scene as stopped
     */
    void StopScene();

    /**
     * @brief Adds a replicated entity to the current scene
     * @param toAdd Identifier of the entity to replicate
     * @param registry Entity registry instance
     */
    void AddToScene(const std::string& toAdd, std::unique_ptr<Registry>& registry,
        float x, float y);

    /**
     * @brief Retrieves an entity from the current scene
     * @param toSeek Identifier of the entity to retrieve
     * @return Entity The requested entity
     * @throw std::runtime_error if the entity is not found
     */
    Entity GetFromScene(const std::string& toSeek);
};

#endif // SCENEMANAGER_HPP

/** @} */ // end of group