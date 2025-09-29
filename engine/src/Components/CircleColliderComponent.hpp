/**
 * @file CircleColliderComponent.hpp
 * @brief Defines the CircleColliderComponent used for circular collision detection in the game engine.
 * @author Juan Torres
 * @date 2024
 * @ingroup Component
 */

#ifndef CIRCLECOLLIDERCOMPONENT_HPP
#define CIRCLECOLLIDERCOMPONENT_HPP

 /**
  * @brief Represents a circular collider component for collision detection.
  *
  * This component is used to define a circular collision boundary around an entity.
  * The radius defines the size of the circle, while the width and height can be used
  * for additional scaling or positioning adjustments.
  */
struct CircleColliderComponent {
    int radius;  /**< The radius of the circle collider. */
    int width;   /**< The width used for scaling or positioning adjustments. */
    int height;  /**< The height used for scaling or positioning adjustments. */

    /**
     * @brief Constructs a CircleColliderComponent.
     *
     * @param radius The radius of the circular collider (default is 0).
     * @param width The width for scaling or positioning adjustments (default is 0).
     * @param height The height for scaling or positioning adjustments (default is 0).
     */
    CircleColliderComponent(int radius = 0, int width = 0, int height = 0) {
        this->radius = radius;
        this->width = width;
        this->height = height;
    }
};

#endif // CIRCLECOLLIDERCOMPONENT_HPP
