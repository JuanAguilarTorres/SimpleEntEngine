/**
 * @file AnimationComponent.hpp
 * @brief Animation support for entities via component
 * @author Juan Torres
 * @date 2024
 * @ingroup Component
 */

#ifndef ANIMATIONCOMPONENT_HPP
#define ANIMATIONCOMPONENT_HPP

#include <SDL2/SDL.h>

/**
 * @brief Represents the animation component for an entity.
 *
 * This component is used to control the animation state of an entity,
 * including the number of frames in the animation, the current frame,
 * the speed at which frames change, and whether the animation loops.
 */
struct AnimationComponent {
    int numFrames;       /**< The total number of frames in the animation. */
    int currentFrame;    /**< The current frame being displayed (1-based index). */
    int frameSpeedRate;  /**< The speed rate for changing frames, where higher values mean slower animations. */
    bool isLoop;         /**< Indicates whether the animation should loop. */
    int startTime;       /**< The start time for the animation, used to determine frame timing. */

    /**
     * @brief Constructs an AnimationComponent.
     *
     * @param numFrames The total number of frames in the animation (default is 1).
     * @param frameSpeedRate The speed rate for changing frames (default is 1).
     * @param isLoop Indicates whether the animation should loop (default is true).
     */
    AnimationComponent(int numFrames = 1, int frameSpeedRate = 1
        , bool isLoop = true) {
        this->numFrames = numFrames;
        this->currentFrame = 1;
        this->frameSpeedRate = frameSpeedRate;
        this->isLoop = isLoop;
        this->startTime = SDL_GetTicks();
    }
};

#endif // ANIMATIONCOMPONENT_HPP