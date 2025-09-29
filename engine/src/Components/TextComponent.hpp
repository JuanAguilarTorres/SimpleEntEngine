/**
 * @file TextComponent.hpp
 * @brief Defines the TextComponent used to render text entities in the game engine, 
 managing text content, font, color, and dimensions for display purposes.
 * @author Juan Torres
 * @date 2024
 * @ingroup Component
 */

#ifndef TEXTCOMPONENT_HPP
#define TEXTCOMPONENT_HPP

#include <SDL2/SDL.h>
#include <string>

 /**
  * @brief Represents a component that handles the rendering of text for an entity.
  *
  * This component manages text content, font selection, color settings, and
  * dimensions for rendering text on the screen. It allows customization of
  * text appearance and supports color specification using RGBA values.
  */
struct TextComponent {
    std::string text;    /**< The text content to be rendered. */
    std::string fontId;  /**< The ID of the font used for rendering the text. */
    SDL_Color color;     /**< The color of the text, specified as an RGBA value. */
    int width;           /**< The width of the rendered text (initialized to 0). */
    int height;          /**< The height of the rendered text (initialized to 0). */

    /**
     * @brief Constructs a TextComponent with specified text, font, and color.
     *
     * @param text The text content to render (default is an empty string).
     * @param fontId The ID of the font used for rendering (default is an empty string).
     * @param r The red component of the text color (default is 0).
     * @param g The green component of the text color (default is 0).
     * @param b The blue component of the text color (default is 0).
     * @param a The alpha (transparency) component of the text color (default is 0).
     */
    TextComponent(const std::string& text = "", const std::string& fontId = "",
        unsigned char r = 0, unsigned char g = 0, unsigned char b = 0,
        unsigned char a = 0) {
        this->text = text;
        this->fontId = fontId;
        this->color.r = r;
        this->color.g = g;
        this->color.b = b;
        this->color.a = a;
        this->width = 0;
        this->height = 0;
    }
};

#endif // TEXTCOMPONENT_HPP
