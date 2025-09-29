/**
 * @file LuaBinding.hpp
 * @brief Provides C++ to Lua bindings for the game engine functionality
 * @author Juan Torres
 * @date 2024
 *
 * @defgroup LuaBinding Lua Script Bindings
 * @{
 * @brief Binds methods for use in Lua
 *
 * This file contains the C++ functions that are exposed to Lua scripts,
 * allowing Lua code to interact with the game engine's core systems,
 * components, and functionality.
 */

#ifndef LUABINDING_HPP
#define LUABINDING_HPP

#include <SDL2/SDL.h>
#include <string>
#include "../AnimationManager/AnimationManager.hpp"
#include "../Components/AnimationComponent.hpp"
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/ObjectComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/PropertyComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/TextComponent.hpp"
#include "../Game/Game.hpp"

 /**
  * @brief Changes the animation of a given entity.
  *
  * Updates the entity's animation and sprite components to reflect the
  * specified animation ID. Resets animation parameters and applies new
  * frame and texture data.
  *
  * @param entity The entity whose animation is being changed.
  * @param animationId The ID of the new animation to apply.
  */
void ChangeAnimation(Entity entity, const std::string & animationId) {
    auto& animation = entity.GetComponent<AnimationComponent>();
    auto& sprite = entity.GetComponent<SpriteComponent>();

    AnimationData animationData;
    animationData = Game::GetInstance().animationManager->GetAnimation(
        animationId);

    sprite.textureId = animationData.textureId;
    sprite.width = animationData.width;
    sprite.height = animationData.height;
    sprite.srcRect.x = 0;
    sprite.srcRect.y = 0;

    animation.currentFrame = 1;
    animation.frameSpeedRate = animationData.frameSpeedRate;
    animation.numFrames = animationData.numFrames;
    animation.isLoop = animationData.isLoop;
    animation.startTime = SDL_GetTicks();
}

/**
 * @brief Flips the sprite of a given entity horizontally.
 *
 * @param entity The entity whose sprite is being flipped.
 * @param flip A boolean value indicating whether to flip the sprite.
 *             `true` for flipped, `false` for normal orientation.
 */
void FlipSprite(Entity entity, bool flip) {
    auto& sprite = entity.GetComponent<SpriteComponent>();
    sprite.flip = flip;
}

 // Input Control Functions
 /**
  * @brief Checks if a specific game key action is currently activated
  * @param action Name of the action to check
  * @return true if the action is activated, false otherwise
  *
  * @note Can be called from Lua as: is_action_activated("action_name")
  */
bool IsActionActivated(const std::string& action) {
    return Game::GetInstance().controllerManager->IsActionActivated(action);
}

// Audio System Functions
/**
 * @brief Plays a sound effect by its identifier
 * @param soundId Unique identifier of the sound to play
 *
 * @note Can be called from Lua as: play_sfx("sound_id")
 */
void PlaySound(const std::string& soundId) {
    AudioSystem& audioSystem = Game::GetInstance().registry->
        GetSystem<AudioSystem>();
    audioSystem.PlaySoundEffect(soundId, Game::GetInstance().assetManager);
}

// RigidBody Component Functions
/**
 * @brief Gets the velocity of an entity's rigid body
 * @param entity The entity to fetch
 *
 * @note Can be called from Lua as: get_velocity(entity)
 */

std::tuple<int, int> GetVelocity(Entity entity) {
    const auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

    return {
    static_cast<int>(rigidbody.velocity.x),
    static_cast<int>(rigidbody.velocity.y)
    };
}

/**
 * @brief Sets the velocity of an entity's rigid body
 * @param entity The entity to modify
 * @param x Velocity in the X direction
 * @param y Velocity in the Y direction
 *
 * @note Can be called from Lua as: set_velocity(entity, x, y)
 */
void SetVelocity(Entity entity, float x, float y) {
    if (entity.HasComponent<RigidBodyComponent>()) {
        auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
        rigidbody.velocity.x = x;
        rigidbody.velocity.y = y;
    }
}

/**
 * @brief Adds force to an entity's rigid body
 * @param entity The entity to add
 * @param x Velocity in the X direction
 * @param y Velocity in the Y direction
 *
 * @note Can be called from Lua as: add_force(entity, x, y)
 */
void AddForce(Entity entity, float x, float y) {
    auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
    rigidbody.sumForces += glm::vec2(x, y);

}

// Transform Component Functions
/**
 * @brief Gets the current position of an entity
 * @param entity The entity to query
 * @return A pair containing the x and y coordinates
 *
 * @note Can be called from Lua as: x, y = get_Position(entity)
 */
std::pair<float, float> GetPosition(Entity entity) {
    if (entity.HasComponent<TransformComponent>()) {
        auto& transform = entity.GetComponent<TransformComponent>();
        return { transform.position.x, transform.position.y };
    }
    return { 0.0f, 0.0f };
}

/**
 * @brief Sets the position of an entity
 * @param entity The entity to modify
 * @param x New X coordinate
 * @param y New Y coordinate
 *
 * @note Can be called from Lua as: set_Position(entity, x, y)
 */
void SetPosition(Entity entity, float x, float y) {
    if (entity.HasComponent<TransformComponent>()) {
        auto& transform = entity.GetComponent<TransformComponent>();
        transform.position.x = x;
        transform.position.y = y;
    }
}

/**
 * @brief Gets the current rotation of an entity
 * @param entity The entity to query
 * @return A double containing the rotation
 *
 * @note Can be called from Lua as: get_Rotation(entity)
 */
double GetRotation(Entity entity) {
    if (entity.HasComponent<TransformComponent>()) {
        auto& transform = entity.GetComponent<TransformComponent>();
        return transform.rotation;
    }
    return 0.0;
}

/**
 * @brief Sets the rotation of an entity
 * @param entity The entity to modify
 * @param rot New rotation
 *
 * @note Can be called from Lua as: set_Rotation(entity, rot)
 */
void SetRotation(Entity entity, double rot) {
    if (entity.HasComponent<TransformComponent>()) {
        auto& transform = entity.GetComponent<TransformComponent>();
        transform.rotation = rot;
    }
}

/**
 * @brief Gets the current 3D rotation of an entity
 * @param entity The entity to query
 * @return A pair of doubles containing the rotation
 *
 * @note Can be called from Lua as: x, y = get_3DRotation(entity)
 */
std::pair<double, double> Get3DRotation(Entity entity) {
    if (entity.HasComponent<ObjectComponent>()) {
        auto& object = entity.GetComponent<ObjectComponent>();
        return { object.xRot, object.yRot };
    }
    return { 0.0, 0.0 };
}

/**
 * @brief Sets the 3D rotation of an entity
 * @param entity The entity to modify
 * @param rotX New x rotation
 * @param rotY New y rotation
 *
 * @note Can be called from Lua as: set_3DRotation(entity, rotX, rotY)
 */
void Set3DRotation(Entity entity, double rotX, double rotY) {
    if (entity.HasComponent<ObjectComponent>()) {
        auto& object = entity.GetComponent<ObjectComponent>();
        object.xRot = rotX;
        object.yRot = rotY;
    }
}

/**
 * @brief Gets the current size of an entity
 * @param entity The entity to query
 * @return A pair containing the w and h sizes
 *
 * @note Can be called from Lua as: w, h = get_size(entity)
 */
std::tuple<int, int> GetSize(Entity entity) {
    const auto& sprite = entity.GetComponent<SpriteComponent>();
    const auto& transform = entity.GetComponent<TransformComponent>();

    int width = sprite.width * transform.scale.x;
    int height = sprite.height * transform.scale.y;

    return { width, height };
}

/**
 * @brief Gets the current box collider of an entity
 * @param entity The entity to query
 * @return A pair containing the w and h sizes
 *
 * @note Can be called from Lua as: w, h = get_box(entity)
 */
std::tuple<int, int> GetBox(Entity entity) {
    const auto& box = entity.GetComponent<BoxColliderComponent>();

    int width = box.width;
    int height = box.height;

    return { width, height };
}

// Property Component Functions
/**
 * @brief Gets the tag of an entity
 * @param entity The entity to query
 * @return The entity's tag or "none" if no tag exists
 *
 * @note Can be called from Lua as: tag = get_tag(entity)
 */
std::string GetTag(Entity entity) {
    if (entity.HasComponent<PropertyComponent>()) {
        return entity.GetComponent<PropertyComponent>().tag;
    }
    return "none";
}

/**
 * @brief Sets the tag of an entity
 * @param entity The entity to modify
 * @param tag New tag value
 *
 * @note Can be called from Lua as: set_tag(entity, "tag_name")
 */
void SetTag(Entity entity, const std::string& tag) {
    if (entity.HasComponent<PropertyComponent>()) {
        auto& property = entity.GetComponent<PropertyComponent>();
        property.tag = tag;
    }
}

/**
 * @brief Sets the rigid properties of an entity
 * @param entity The entity to modify
 * @param bool New value for Dynamic property
 * @param bool New value for Solid property
 *
 * @note Can be called from Lua as: set_rigid(entity, bool, bool)
 */
void SetRigid(Entity entity, bool dynamic, bool solid) {
    if (entity.HasComponent<RigidBodyComponent>()) {
        auto& property = entity.GetComponent<RigidBodyComponent>();
        property.isDynamic = dynamic;
        property.isSolid = solid;
    }
}

// Text Component Functions
/**
 * @brief Sets the text content of a text component
 * @param entity The entity to modify
 * @param newText New text content
 *
 * @note Can be called from Lua as: set_Text(entity, "new text")
 */
void SetText(Entity entity, std::string newText) {
    if (entity.HasComponent<TextComponent>()) {
        auto& text = entity.GetComponent<TextComponent>();
        text.text = newText;
    }
}

// Scene Management Functions
/**
 * @brief Transitions to a different scene
 * @param sceneName Name of the scene to transition to
 *
 * @note Can be called from Lua as: go_to_scene("scene_name")
 */
void GoToScene(const std::string& sceneName) {
    Game::GetInstance().sceneManager->SetNextScene(sceneName);
    Game::GetInstance().sceneManager->StopScene();
}

/**
 * @brief Creates a new entity from a FlyWeight Factory Instance
 * @param entityName Name of the FlyWeight Factory Instance
 *
 * @note Can be called from Lua as: load_replica("entityName")
 */
void LoadReplica(const std::string& entityName) {
    Game::GetInstance().sceneManager->AddToScene(entityName,
        Game::GetInstance().registry, -1.0f, -1.0f);
}

/**
 * @brief Creates a new entity from a FlyWeight Factory Instance
 * with custom coordinates
 * @param entityName Name of the FlyWeight Factory Instance
 * @param x, y coordinates
 *
 * @note Can be called from Lua as:
 * load_replica_xy("entityName", float x, float y)
 */
void LoadReplicaXY(const std::string& entityName,
    float x, float y) {
    Game::GetInstance().sceneManager->AddToScene(entityName,
        Game::GetInstance().registry, x, y);
}

/**
 * @brief Gets an entity's data from a template file
 * @param entity Name of the entity
 * @return The entity object
 *
 * @note Can be called from Lua as: entity = get_Data("entity")
 */
Entity GetEntityData(const std::string& entity) {
    return Game::GetInstance().sceneManager->GetFromScene(entity);
}

/**
 * @brief Removes an entity from the game
 * @param entity The entity to remove
 *
 * @note Can be called from Lua as: delete_Entity(entity)
 */
void DeleteEntity(Entity entity) {
    Game::GetInstance().registry->KillEntity(entity);
}

// Collision Management Functions

/**
 * @brief Checks an entity's collision from the left
 * @param entity Entity instance
 * @param entity Other entity that was collided
 * @return Boolean checking the collision
 *
 * @note Can be called from Lua as: entity = left_Collision("entity", "other")
 */
bool LeftCollision(Entity e, Entity other) {
    const auto& eCollider = e.GetComponent<BoxColliderComponent>();
    const auto& eTransform = e.GetComponent<TransformComponent>();

    const auto& oCollider = other.GetComponent<BoxColliderComponent>();
    const auto& oTransform = other.GetComponent<TransformComponent>();

    float eX = eTransform.previousPosition.x;
    float eY = eTransform.previousPosition.y;
    float eH = static_cast<float>(eCollider.height);

    float oX = oTransform.previousPosition.x;
    float oY = oTransform.previousPosition.y;
    float oH = static_cast<float>(oCollider.height);

    // Left side collides with Other
    return (
        oY < eY + eH &&
        oY + oH > eY &&
        oX < eX
    );
}

/**
 * @brief Checks an entity's collision from the right
 * @param entity Entity instance
 * @param entity Other entity that was collided
 * @return Boolean checking the collision
 *
 * @note Can be called from Lua as: entity = right_Collision("entity", "other")
 */
bool RightCollision(Entity e, Entity other) {
    const auto& eCollider = e.GetComponent<BoxColliderComponent>();
    const auto& eTransform = e.GetComponent<TransformComponent>();

    const auto& oCollider = other.GetComponent<BoxColliderComponent>();
    const auto& oTransform = other.GetComponent<TransformComponent>();

    float eX = eTransform.previousPosition.x;
    float eY = eTransform.previousPosition.y;
    float eH = static_cast<float>(eCollider.height);

    float oX = oTransform.previousPosition.x;
    float oY = oTransform.previousPosition.y;
    float oH = static_cast<float>(oCollider.height);

    // Right side collides with Other
    return (
        oY < eY + eH &&
        oY + oH > eY &&
        oX > eX
        );
}
/**
 * @brief Checks an entity's collisions
 * @param entity Entity instance
 * @param entity Other entity that was collided
 * @return String with the direction of the hit
 *
 * @note Can be called from Lua as: entity = get_dir_collision("entity", "other")
 */
std::string GetDirCollision(Entity a, Entity b) {
		auto& aCollider = a.GetComponent<BoxColliderComponent>();
		auto& bCollider = b.GetComponent<BoxColliderComponent>();
		auto& aTransform = a.GetComponent<TransformComponent>();
		auto& bTransform = b.GetComponent<TransformComponent>();

		float aX = aTransform.previousPosition.x;
		float aY = aTransform.previousPosition.y;
		float aW = static_cast<float>(aCollider.width);

		float bX = bTransform.previousPosition.x;
		float bY = bTransform.previousPosition.y;
		float bW = static_cast<float>(bCollider.width);

		// Top - A, Bottom - B
		if (
				aX < bX + bW &&
				aX + aW > bX &&
				aY > bY
				){
                    return "top";                
		}

		// Left - A, Right - B
/*		if (
				aY < bY + bH &&
				aY + aH > bY &&
				aX > bX
				){
                    return "left";
		}
*/
		// Bottom - A, Top - B
		if (
				aX < bX + bW &&
				aX + aW > bX &&
				aY < bY
				){
                    return "bottom";
		}

        // Right - A, Left - B
/*		if (
				aY < bY + bH &&
				aY + aH > bY &&
				aX < bX
				){
                    return "right";
		}
*/
		return "none";
}

/**
 * @brief Removes an entity from the game
 * @param entity The entity to remove
 *
 * @note Can be called from Lua as: delete_Entity(entity)
 */
void SetSolid(Entity entity, bool isSolid) {
    auto& platform = entity.GetComponent<RigidBodyComponent>();
    platform.isSolid=isSolid;
}

bool GetSolid(Entity entity){
    return entity.GetComponent<RigidBodyComponent>().isSolid;
}

void AddBoxExclusion(Entity entity, const std::string& tag) {
    auto& box = entity.GetComponent<BoxColliderComponent>();
    box.AddExclusion(tag);
}

void RemoveBoxExclusion(Entity entity, const std::string& tag) {
    auto& box = entity.GetComponent<BoxColliderComponent>();
    box.RemoveExclusion(tag);
}

bool IsBoxExcluded(Entity entity, const std::string& tag) {
    auto& box = entity.GetComponent<BoxColliderComponent>();
    bool status = box.IsExcluded(tag);
    return status;
}

// Time Management Functions
/**
 * @brief Gets the current game time in Ticks
 * @return Current time in Ticks since SDL initialization
 *
 * @note Can be called from Lua as: time = get_time()
 */
Uint32 GetTime() {
    return SDL_GetTicks();
}

/**
 * @brief Gets the current position of the mouse
 * @return A pair containing the x and y coordinates of the mouse
 *
 * @note Can be called from Lua as: x, y = get_mouse()
 */
std::pair<int, int> GetMousePosition() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return { x, y };
}

/**
 * @brief Sets the shadow color of a 3d model
 * @param entity Entity instance
 * @param r g b Colors of the shadow
 *
 * @note Can be called from Lua as: set_shadow(entity, r, g, b)
*/

void SetShadow(Entity entity, float r, float g, float b) {
    if (entity.HasComponent<ObjectComponent>()) {
        auto& object = entity.GetComponent<ObjectComponent>();
        object.sr = r;
        object.sg = g;
        object.sb = b;
    }
}

#endif // LUABINDING_HPP

/** @} */ // end of LuaBinding group