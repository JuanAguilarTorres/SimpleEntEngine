/**
 * @file RenderSystem.hpp
 * @brief Defines the RenderSystem responsible for rendering entities on the screen.
 * @author Juan Torres
 * @date 2024
 * @ingroup System
 */

#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP

#include <SDL2/SDL.h>

#include "../AssetManager/AssetManager.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../ECS/ECS.hpp"

 /**
  * @brief Represents the system that manages the rendering of entities.
  *
  * The RenderSystem updates the display of entities based on their
  * position, scale, rotation, and sprite information.
  */
class RenderSystem : public System {
public:
    /**
     * @brief Constructs a RenderSystem.
     *
     * This constructor specifies that entities using this system must have
     * SpriteComponent and TransformComponent.
     */
    RenderSystem() {
        RequireComponent<SpriteComponent>();
        RequireComponent<TransformComponent>();
    }

    /**
     * @brief Renders entities to the screen.
     *
     * @param renderer Pointer to the SDL_Renderer used for rendering.
     * @param camera The camera's viewport for rendering adjustments.
     * @param AssetManager A unique pointer to the AssetManager for retrieving textures.
     *
     * This function iterates over all entities in the system and renders their
     * sprites to the screen, adjusting for camera position and entity transformation.
     */
    void Update(SDL_Renderer* renderer, SDL_Rect& camera,
        const std::unique_ptr<AssetManager>& AssetManager) {
        for (auto entity : GetSystemEntities()) {
            const auto sprite = entity.GetComponent<SpriteComponent>();
            const auto transform = entity.GetComponent<TransformComponent>();

            SDL_Rect srcRect = sprite.srcRect;

            int dstX = static_cast<int>(transform.position.x);
            int dstY = static_cast<int>(transform.position.y);

            // Adjust for camera position if not camera-free
            if (!transform.cameraFree) {
                dstX -= camera.x;
                dstY -= camera.y;
            }

            SDL_Rect dstRect = {
                dstX,
                dstY,
                static_cast<int>(sprite.width * transform.scale.x),
                static_cast<int>(sprite.height * transform.scale.y)
            };

            // Render the sprite with rotation
            SDL_RenderCopyEx(
                renderer,
                AssetManager->GetTexture(sprite.textureId),
                &srcRect,
                &dstRect,
                transform.rotation,
                NULL,
                (sprite.flip) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
            );
        }
    }
};

#endif // RENDERSYSTEM_HPP
