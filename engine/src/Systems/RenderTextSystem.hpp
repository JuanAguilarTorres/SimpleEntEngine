/**
 * @file RenderTextSystem.hpp
 * @brief Defines the RenderTextSystem responsible for rendering text on the screen.
 * @author Juan Torres
 * @date 2024
 * @ingroup System
 */

#ifndef RENDERTEXTSYSTEM_HPP
#define RENDERTEXTSYSTEM_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <memory>

#include "../AssetManager/AssetManager.hpp"
#include "../Components/TextComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../ECS/ECS.hpp"

 /**
  * @brief Represents the system that manages the rendering of text entities.
  *
  * The RenderTextSystem updates the display of text based on their
  * position, scale, and associated font information.
  */
class RenderTextSystem : public System {
public:
    /**
     * @brief Constructs a RenderTextSystem.
     *
     * This constructor specifies that entities using this system must have
     * TextComponent and TransformComponent.
     */
    RenderTextSystem() {
        RequireComponent<TextComponent>();
        RequireComponent<TransformComponent>();
    }

    /**
     * @brief Renders text entities to the screen.
     *
     * @param renderer Pointer to the SDL_Renderer used for rendering.
     * @param assetManager A unique pointer to the AssetManager for retrieving fonts.
     *
     * This function iterates over all entities in the system and renders their
     * text to the screen, adjusting for transformation properties.
     */
    void Update(SDL_Renderer* renderer,
        const std::unique_ptr<AssetManager>& assetManager) {
        for (auto entity : GetSystemEntities()) {
            auto& text = entity.GetComponent<TextComponent>();
            auto& transform = entity.GetComponent<TransformComponent>();

            // Create a surface from the text
            SDL_Surface* surface = TTF_RenderText_Blended(
                assetManager->GetFont(text.fontId), text.text.c_str(), text.color);
            text.width = surface->w;
            text.height = surface->h;

            // Create a texture from the surface
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            // Define the destination rectangle for rendering
            SDL_Rect dstRect = {
                static_cast<int>(transform.position.x),
                static_cast<int>(transform.position.y),
                text.width * static_cast<int>(transform.scale.x),
                text.height * static_cast<int>(transform.scale.y),
            };

            // Render the text texture to the screen
            SDL_RenderCopy(renderer, texture, NULL, &dstRect);
            SDL_DestroyTexture(texture);
        }
    }
};

#endif // RENDERTEXTSYSTEM_HPP
