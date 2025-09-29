/**
 * @file CameraFollowComponent.hpp
 * @brief Defines the CameraFollowComponent used to make the camera follow an entity.
 * @author Juan Torres
 * @date 2024
 * @ingroup Component
 */

#ifndef CAMERAFOLLOWCOMPONENT_HPP
#define CAMERAFOLLOWCOMPONENT_HPP

 /**
  * @brief Represents a component that enables the camera to follow an entity.
  *
  * When this component is added to an entity, the camera will automatically
  * follow the position of that entity in the game world.
  */
struct CameraFollowComponent {
    /**
     * @brief Constructs a CameraFollowComponent.
     *
     * The default constructor initializes the component with no specific parameters.
     */
    CameraFollowComponent() {}
};

#endif // CAMERAFOLLOWCOMPONENT_HPP
