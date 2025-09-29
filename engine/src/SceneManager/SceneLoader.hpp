/**
 * @file SceneLoader.hpp
 * @brief Scene loading and management system for game engine
 * @author Juan Torres
 * @date 2024
 * @ingroup SceneManager
 */

#ifndef SCENELOADER_HPP
#define SCENELOADER_HPP

#include <SDL2/SDL.h>
#include <tinyxml2/tinyxml2.h>
#include <memory>
#include <sol/sol.hpp>
#include <string>
#include <map>
#include "../AnimationManager/AnimationManager.hpp"
#include "../AssetManager/AssetManager.hpp"
#include "../ControllerManager/ControllerManager.hpp"
#include "../ECS/ECS.hpp"

 /**
  * @class SceneLoader
  * @brief Manages loading and initialization of game scenes from Lua scripts
  *
  * The SceneLoader class is responsible for parsing scene files written in Lua
  * and creating the corresponding game objects, assets, and configurations.
  * It handles loading of various assets including videos, sprites, music,
  * sound effects, and fonts, as well as setting up input controls and entities.
  */
class SceneLoader {
private:
    /** @brief Map storing replicable entities with their unique identifiers */
    std::map<std::string, Entity> entityMap;

    /**
     * @brief Loads video assets from Lua configuration
     * @param renderer SDL renderer instance
     * @param videos Lua table containing video configurations
     * @param assetManager Asset management system instance
     */
    void LoadVideos(SDL_Renderer* renderer, const sol::table& videos
        , std::unique_ptr<AssetManager>& assetManager);

    /**
     * @brief Loads 3d Objects assets from Lua configuration
     * @param renderer SDL renderer instance
     * @param objects Lua table containing 3d Objects configurations
     * @param assetManager Asset management system instance
     */
    void LoadObjects(const sol::table& objects
        , std::unique_ptr<AssetManager>& assetManager);

    /**
     * @brief Loads sprite assets from Lua configuration
     * @param renderer SDL renderer instance
     * @param sprites Lua table containing sprite configurations
     * @param assetManager Asset management system instance
     */
    void LoadSprites(SDL_Renderer* renderer, const sol::table& sprites
        , std::unique_ptr<AssetManager>& assetManager);

    /**
     * @brief Loads animations from a Lua table and adds them to the animation manager.
     *
     * @param animations A Lua table containing animation definitions.
     *                   Each entry should include necessary data such as texture ID,
     *                   frame dimensions, number of frames, frame speed, and loop status.
     * @param animationManager A unique pointer to the animation manager where the animations will be added.
     */
    void LoadAnimations(const sol::table& animations
        , std::unique_ptr<AnimationManager>& animationManager);

    /**
     * @brief Loads music assets from Lua configuration
     * @param music Lua table containing music configurations
     * @param assetManager Asset management system instance
     */
    void LoadMusic(const sol::table& music
        , std::unique_ptr<AssetManager>& assetManager);

    /**
     * @brief Loads sound effect assets from Lua configuration
     * @param sounds Lua table containing sound effect configurations
     * @param assetManager Asset management system instance
     */
    void LoadSoundEffects(const sol::table& sounds
        , std::unique_ptr<AssetManager>& assetManager);

    /**
     * @brief Loads font assets from Lua configuration
     * @param fonts Lua table containing font configurations
     * @param assetManager Asset management system instance
     */
    void LoadFonts(const sol::table& fonts,
        std::unique_ptr<AssetManager>& assetManager);

    /**
     * @brief Loads keyboard input mappings from Lua configuration
     * @param keys Lua table containing key mappings
     * @param controllerManager Input controller management system instance
     */
    void LoadKeys(const sol::table& keys
        , std::unique_ptr<ControllerManager>& controllerManager);

    /**
     * @brief Loads mouse button mappings from Lua configuration
     * @param buttons Lua table containing button mappings
     * @param controllerManager Input controller management system instance
     */
    void LoadButtons(const sol::table& buttons
        , std::unique_ptr<ControllerManager>& controllerManager);

    /**
     * @brief Loads and creates entities from Lua configuration
     * @param lua Lua state instance
     * @param entities Lua table containing entity configurations
     * @param registry Entity registry instance
     */
    void LoadEntities(sol::state& lua, const sol::table& entities
        , std::unique_ptr<Registry>& registry);

    /**
     * @brief Loads the map configuration with a Lua table and creates corresponding entities.
     * @param map Lua table with the map configuration data.
     * @param registry Entity registry instance to manage the created entities.
     */
    void LoadMap(const sol::table map, std::unique_ptr<Registry>& registry);

    /**
     * @brief Loads a specific layer of the map from an XML element and creates the corresponding entities.
     * @param registry Entity registry instance to manage the created entities.
     * @param layer Pointer to the XML element representing the map layer.
     * @param tWidth Width of each tile in pixels.
     * @param tHeight Height of each tile in pixels.
     * @param mWidth Width of the map in tiles.
     * @param tileSet Name of the tileset used by the layer.
     * @param columns Number of columns in the tileset.
     */
    void LoadLayer(std::unique_ptr<Registry>& registry, tinyxml2::XMLElement* layer,
        int tWidth, int tHeight, int mWidth,
        const std::string& tileSet, int columns);

    /**
     * @brief Loads colliders from an object group in an XML map file.
     *
     * Parses the specified XML element representing an object group and creates
     * entities with collider components in the provided registry.
     *
     * @param registry Entity registry instance to manage the created entities.
     * @param objectGroup Pointer to the XML element representing the object group.
     */
    void LoadColliders(std::unique_ptr<Registry>& registry,
        tinyxml2::XMLElement* objectGroup);

public:
    /**
     * @brief Construct a new Scene Loader object
     */
    SceneLoader();

    /**
     * @brief Destroy the Scene Loader object
     */
    ~SceneLoader();

    /**
     * @brief Loads and initializes a complete scene from a Lua script
     * @param scenePath Path to the Lua scene file
     * @param lua Lua state instance
     * @param assetManager Asset management system instance
     * @param controllerManager Input controller management system instance
     * @param registry Entity registry instance
     * @param renderer SDL renderer instance
     *
     * This method orchestrates the complete scene loading process, including:
     * - Loading all required assets (videos, sprites, music, sound effects, fonts)
     * - Setting up input controls
     * - Creating and configuring entities
     */
    void LoadScene(const std::string& scenePath, sol::state& lua
        , std::unique_ptr<AnimationManager>& animationManager
        , std::unique_ptr<AssetManager>& assetManager
        , std::unique_ptr<ControllerManager>& controllerManager
        , std::unique_ptr<Registry>& registry, SDL_Renderer* renderer);

    /**
     * @brief Creates a copy of an existing entity
     * @param entityId Identifier of the entity to replicate
     * @param registry Entity registry instance
     *
     * Creates a new entity with the same components and properties as the
     * original entity identified by entityId. The new entity is marked as
     * a replica in its PropertyComponent.
     */
    void ReplicateEntity(const std::string& entityId, std::unique_ptr<Registry>& registry,
        float x, float y);

    /**
     * @brief Retrieves an entity by its identifier
     * @param entityId The identifier of the entity to retrieve
     * @return Entity The requested entity
     * @throw std::runtime_error if the entity is not found
     */
    Entity GetDynamicData(const std::string& entityId);
};

#endif // SCENELOADER_HPP