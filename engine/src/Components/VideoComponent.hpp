/**
 * @file VideoComponent.hpp
 * @brief Defines the VideoComponent used to manage video playback properties 
 for an entity in the game engine.
 * @author Juan Torres
 * @date 2024
 * @ingroup Component
 */

#ifndef VIDEOCOMPONENT_HPP
#define VIDEOCOMPONENT_HPP

#include <string>
#include <SDL2/SDL.h>

 /**
  * @brief Represents a component that handles video playback for an entity.
  *
  * This component manages properties related to video playback, including the
  * video ID, dimensions for rendering the video, position on the screen, and
  * warmup count for the video to ensure safe playback.
  */
struct VideoComponent {
    std::string videoId; /**< The ID of the video to be played. */
    int width;           /**< The width of the video display in pixels. */
    int height;          /**< The height of the video display in pixels. */
    int posX;           /**< The x-coordinate for positioning the video on the screen. */
    int posY;           /**< The y-coordinate for positioning the video on the screen. */
    int warmupCount;     /**< The count for warming up the video before playback. */

    /**
     * @brief Constructs a VideoComponent with specified video properties.
     *
     * @param videoId The ID of the video (default is "none").
     * @param width The width of the video display (default is 0).
     * @param height The height of the video display (default is 0).
     * @param posX The x-coordinate for positioning the video (default is 0).
     * @param posY The y-coordinate for positioning the video (Should be 0).
     */
    VideoComponent(const std::string& videoId = "none", int width = 0,
        int height = 0, int posX = 0, int posY = 0) {
        this->videoId = videoId;
        this->width = width;
        this->height = height;
        this->posX = posX;
        this->posY = posY;
        this->warmupCount = 0;
    }
};

#endif // VIDEOCOMPONENT_HPP
