/**
 * @file TransformComponent.hpp
 * @brief Defines the TransformComponent used to manage an entity's 
 position, scale, rotation, and camera behavior in the game engine.
 * @author Juan Torres
 * @date 2024
 * @ingroup Component
 */

#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

#include <glm/glm.hpp>

 /**
  * @brief Represents a component that defines the transformation properties of an entity.
  *
  * This component manages an entity's position, scale, rotation, and whether the
  * camera should be free to move independently of the entity. It is essential for
  * positioning and rendering entities in the game world.
  */
struct TransformComponent {
    glm::vec2 position;   /**< The position of the entity in 2D space. */
    glm::vec2 previousPosition;  /**< The last position of the entity in 2D space. */
    glm::vec2 scale;      /**< The scale factors for the entity in the x and y dimensions. */
    double rotation;      /**< The rotation angle of the entity in degrees. */
    bool cameraFree;      /**< Indicates if the entity can move freely, independent of the camera. */

    /**
     * @brief Constructs a TransformComponent with specified position, scale, rotation, and camera behavior.
     *
     * @param position The initial position of the entity (default is glm::vec2(0.0, 0.0)).
     * @param scale The initial scale of the entity (default is glm::vec2(1.0, 1.0)).
     * @param rotation The initial rotation of the entity in degrees (default is 0.0).
     * @param cameraFree Whether the entity can move independently of the camera (default is false).
     */
    TransformComponent(glm::vec2 position = glm::vec2(0.0, 0.0),
        glm::vec2 scale = glm::vec2(1.0, 1.0),
        double rotation = 0.0,
        bool cameraFree = false) {
        this->position = position;
        this->previousPosition = position;
        this->scale = scale;
        this->rotation = rotation;
        this->cameraFree = cameraFree;
    }
};

#endif // TRANSFORMCOMPONENT_HPP
