/**
 * @file SpriteComponent.hpp
 * @brief Defines the SpriteComponent used for rendering textured entities in the game engine, 
 managing their visual representation through a texture and source rectangle settings.
 * @author Juan Torres
 * @date 2024
 * @ingroup Component
 */

#ifndef SPRITECOMPONENT_HPP
#define SPRITECOMPONENT_HPP

#include <SDL2/SDL.h>
#include <string>

 /**
  * @brief Represents a component that handles the rendering of an entity's sprite.
  *
  * This component manages the visual appearance of an entity by specifying
  * a texture and its dimensions. It also provides source rectangle coordinates
  * for selecting a portion of the texture to render.
  */
struct SpriteComponent {
    std::string textureId; /**< The ID of the texture used for the sprite. */
    int width;             /**< The width of the sprite in pixels. */
    int height;            /**< The height of the sprite in pixels. */
    SDL_Rect srcRect;      /**< The source rectangle specifying the texture area to render. */
    bool flip = false;

    /**
     * @brief Constructs a SpriteComponent with a specified texture and dimensions.
     *
     * @param textureId The ID of the texture (default is "none").
     * @param width The width of the sprite (default is 0).
     * @param height The height of the sprite (default is 0).
     * @param srcRectX The x-coordinate of the source rectangle's top-left corner (default is 0).
     * @param srcRectY The y-coordinate of the source rectangle's top-left corner (default is 0).
     */
    SpriteComponent(const std::string& textureId = "none", int width = 0,
        int height = 0, int srcRectX = 0, int srcRectY = 0) {
        this->textureId = textureId;
        this->width = width;
        this->height = height;
        this->srcRect = { srcRectX, srcRectY, width, height };
    }
};

#endif // SPRITECOMPONENT_HPP
