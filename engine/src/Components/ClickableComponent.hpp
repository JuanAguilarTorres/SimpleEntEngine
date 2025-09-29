/**
 * @file ClickableComponent.hpp
 * @brief Defines the ClickableComponent used to detect and handle click events on entities in the game engine.
 * @author Juan Torres
 * @date 2024
 * @ingroup Component
 */

#ifndef CLICKABLECOMPONENT_HPP
#define CLICKABLECOMPONENT_HPP

 /**
  * @brief Represents a component that enables click detection on an entity.
  *
  * This component is used to determine whether an entity has been clicked.
  * It can be used to trigger actions when the entity is clicked on by the player.
  */
struct ClickableComponent {
    bool isClicked; /**< Indicates whether the entity has been clicked. */

    /**
     * @brief Constructs a ClickableComponent.
     *
     * Initializes the component with the clicked state set to false.
     */
    ClickableComponent() {
        isClicked = false;
    }
};

#endif // CLICKABLECOMPONENT_HPP
