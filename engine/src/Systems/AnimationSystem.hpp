/**
 * @file AnimationSystem.hpp
 * @brief Defines the AnimationSystem responsible for updating 
 animations for entities in the game engine.
 * @author Juan Torres
 * @date 2024
 * @ingroup System
 */

#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP

#include <SDL2/SDL.h>
#include "../Components/AnimationComponent.hpp"
#include "../Components/SpriteComponent.hpp"
#include "../ECS/ECS.hpp"

 /**
  * @brief Represents the system that manages and updates animations for entities.
  *
  * The AnimationSystem processes all entities with AnimationComponent and SpriteComponent,
  * updating their current animation frame based on elapsed time and frame speed rate.
  */
    class AnimationSystem : public System {
    public:
        /**
         * @brief Constructs an AnimationSystem and specifies required components.
         *
         * This constructor requires that entities using this system must have both
         * AnimationComponent and SpriteComponent.
         */
        AnimationSystem() {
            RequireComponent<AnimationComponent>();
            RequireComponent<SpriteComponent>();
        }

        /**
         * @brief Updates the animation state for all entities in the system.
         *
         * This function calculates the current frame for each entity's animation based on the
         * time elapsed since the animation started and updates the source rectangle for the sprite.
         */
        void Update() {
            for (auto entity : GetSystemEntities()) {
                auto& animation = entity.GetComponent<AnimationComponent>();
                auto& sprite = entity.GetComponent<SpriteComponent>();

                // Calculate the total elapsed time since the animation started
                int elapsedTime = SDL_GetTicks() - animation.startTime;

                // Determine the new frame index
                int frameIndex = (elapsedTime * animation.frameSpeedRate / 1000);

                if (animation.isLoop) {
                    // Loop the animation
                    animation.currentFrame = frameIndex % animation.numFrames;
                }
                else {
                    // Stop at the last frame if not looping
                    animation.currentFrame = std::min(frameIndex, animation.numFrames - 1);
                }

                // Update the sprite's source rectangle based on the current frame
                sprite.srcRect.x = animation.currentFrame * sprite.width;
            }
        }

};

#endif // ANIMATIONSYSTEM_HPP
