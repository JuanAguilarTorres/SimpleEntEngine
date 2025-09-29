/**
 * @file ClickEvent.hpp
 * @brief Class Event for Click
 * @author Juan Torres
 * @date 2024
 * @ingroup EventManager
 */

#ifndef CLICKEVENT_HPP
#define CLICKEVENT_HPP

#include "../ECS/ECS.hpp"
#include "../EventManager/Event.hpp"

/**
 * @brief Represents a click event in the system.
 *
 * The ClickEvent class is used to encapsulate information about a click event,
 * including the button pressed and the position of the click on the screen.
 */
class ClickEvent : public Event {
public:
    /**
     * @brief The code of the button that was clicked.
     *
     * This represents the mouse button code for the click event, where
     * different values correspond to different buttons (e.g., left, right, middle).
     */
    int buttonCode;

    /**
     * @brief The X position of the click event.
     *
     * This represents the horizontal position of the mouse cursor when the
     * click event occurred.
     */
    int posX;

    /**
     * @brief The Y position of the click event.
     *
     * This represents the vertical position of the mouse cursor when the
     * click event occurred.
     */
    int posY;

    /**
     * @brief Constructs a new ClickEvent.
     *
     * @param buttonCode The code of the button that was clicked. Default is 0.
     * @param posX The X position of the click event. Default is 0.
     * @param posY The Y position of the click event. Default is 0.
     */
    ClickEvent(int buttonCode = 0, int posX = 0, int posY = 0) {
        this->buttonCode = buttonCode;
        this->posX = posX;
        this->posY = posY;
    }
};

#endif // CLICKEVENT_HPP
