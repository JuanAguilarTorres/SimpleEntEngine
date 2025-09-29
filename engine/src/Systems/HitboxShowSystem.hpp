/**
 * @file HitboxShowSystem.hpp
 * @brief A system responsible for displaying hitboxes in the game.
 * @author Juan Torres
 * @date 2024
 * @ingroup System
 */

#ifndef HITBOXSHOWSYSTEM_HPP
#define HITBOXSHOWSYSTEM_HPP

#include <SDL2/SDL.h>

#include "../Components/BoxColliderComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../ECS/ECS.hpp"

/**
 * @brief A system responsible for displaying hitboxes in the game.
 *
 * This system is responsible for rendering the hitboxes of entities that have a
 * `BoxColliderComponent` and a `TransformComponent`. It is typically used for debugging
 * purposes to visualize collision boundaries.
 */
class HitboxShowSystem : public System {
public:
    /**
     * @brief Constructs a HitboxShowSystem.
     *
     * The constructor ensures that entities in this system must have both a `BoxColliderComponent`
     * and a `TransformComponent` to be processed by the system.
     */
	HitboxShowSystem() {
		RequireComponent<BoxColliderComponent>();
		RequireComponent<TransformComponent>();
	}

    /**
     * @brief Updates the system and renders hitboxes.
     *
     * This method processes all entities with the required components,
     * then renders their hitboxes onto the screen using the SDL renderer.
     * The camera's position is taken into account to correctly position the hitboxes.
     *
     * @param renderer Pointer to the SDL renderer used for drawing.
     * @param camera The SDL_Rect representing the camera's position and dimensions.
     */
    void Update(SDL_Renderer* renderer, SDL_Rect& camera) {
        for (auto entity : GetSystemEntities()) {
            const auto& collider = entity.GetComponent<BoxColliderComponent>();
            const auto& transform = entity.GetComponent<TransformComponent>();

            glm::vec2 boxPosition = transform.position + collider.offset;

            SDL_Rect box = {
                static_cast<int>(boxPosition.x - camera.x),
                static_cast<int>(boxPosition.y - camera.y),
                static_cast<int>(collider.width * transform.scale.x),
                static_cast<int>(collider.height * transform.scale.y)
            };

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &box);
        }
    }

};

#endif // HITBOXSHOWSYSTEM_HPP