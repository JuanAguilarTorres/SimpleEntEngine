/**
 * @file CameraMovementSystem.hpp
 * @brief Defines the CameraMovementSystem responsible for managing camera movements in the game.
 * @author Juan Torres
 * @date 2024
 * @ingroup System
 */

#ifndef CAMERAMOVEMENTSYSTEM_HPP
#define CAMERAMOVEMENTSYSTEM_HPP

#include <SDL2/SDL.h>

#include "../Components/CameraFollowComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../ECS/ECS.hpp"
#include "../Game/Game.hpp"

 /**
  * @brief Represents the system that manages camera movement based on entity positions.
  *
  * The CameraMovementSystem updates the camera position to follow entities with a CameraFollowComponent
  * and adjusts its position based on their TransformComponent.
  */
class CameraMovementSystem : public System {
public:
    /**
     * @brief Constructs a CameraMovementSystem.
     *
     * This constructor specifies that entities using this system must have both
     * CameraFollowComponent and TransformComponent.
     */
    CameraMovementSystem() {
        RequireComponent<CameraFollowComponent>();
        RequireComponent<TransformComponent>();
    }

    /**
     * @brief Updates the camera position based on the entities it follows.
     *
     * @param camera A reference to the SDL_Rect representing the camera's viewport.
     *
     * This function adjusts the camera's position based on the positions of entities
     * with the TransformComponent. It ensures the camera stays within the bounds of the game map.
     */
    void Update(SDL_Rect& camera) {
        for (auto entity : GetSystemEntities()) {
            const auto& transform = entity.GetComponent<TransformComponent>();

            // Center the camera on the entity's new position
            camera.x = static_cast<int>(transform.position.x) - (camera.w / 2);
            camera.y = static_cast<int>(transform.position.y) - (camera.h / 2);

            // Clamp camera position to prevent it from going out of bounds
            camera.x = std::max(0, std::min(camera.x, Game::GetInstance().mapWidth - camera.w));
            camera.y = std::max(0, std::min(camera.y, Game::GetInstance().mapHeight - camera.h));
        }
    }
};

#endif // CAMERAMOVEMENTSYSTEM_HPP
