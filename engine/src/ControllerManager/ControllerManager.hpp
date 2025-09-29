/**
 * @file ControllerManager.hpp
 * @brief Defines the ControllerManager class for handling keyboard and mouse input.
 * @author Juan Torres
 * @date 2024
 * @defgroup ControllerManagement Controller Management
 * @{
 * @brief Manages all the inputs of the project
 */

#ifndef CONTROLLERMANAGER_HPP
#define CONTROLLERMANAGER_HPP

#include <SDL2/SDL.h>
#include <map>
#include <string>
#include <tuple>

 /**
  * @class ControllerManager
  * @brief Manages keyboard and mouse input actions.
  *
  * The ControllerManager class handles mapping of keyboard keys and mouse buttons
  * to specific actions, tracking their states (pressed or released), and
  * storing the current mouse position.
  */
class ControllerManager {
private:
    std::map<std::string, int> actionKeyName; ///< Maps action names to their corresponding key codes.
    std::map<int, bool> keyDown; ///< Tracks the state (pressed or not) of keys.

    std::map<std::string, int> mouseButtonName; ///< Maps mouse button names to their corresponding button codes.
    std::map<int, bool> mouseButtonDown; ///< Tracks the state (pressed or not) of mouse buttons.

    int mousePosX; ///< The current X-coordinate of the mouse position.
    int mousePosY; ///< The current Y-coordinate of the mouse position.

public:
    /**
     * @brief Constructs a new ControllerManager object.
     *
     * Initializes internal maps and variables.
     */
    ControllerManager();

    /**
     * @brief Destroys the ControllerManager object.
     */
    ~ControllerManager();

    /**
     * @brief Clears all input states.
     *
     * Resets the state of all keys, mouse buttons, and mouse position.
     */
    void Clear();

    // Keyboard

    /**
     * @brief Adds a keyboard action mapping.
     *
     * Associates a keyboard key with a specific action.
     *
     * @param action The name of the action.
     * @param keyCode The SDL key code to associate with the action.
     */
    void AddActionKey(const std::string& action, int keyCode);

    /**
     * @brief Marks a key as pressed.
     *
     * Updates the internal state to indicate that a key is currently pressed.
     *
     * @param keyCode The SDL key code of the key that was pressed.
     */
    void KeyDown(int keyCode);

    /**
     * @brief Marks a key as released.
     *
     * Updates the internal state to indicate that a key is no longer pressed.
     *
     * @param keyCode The SDL key code of the key that was released.
     */
    void KeyUp(int keyCode);

    /**
     * @brief Checks if an action is currently activated.
     *
     * Determines if the key associated with the specified action is pressed.
     *
     * @param action The name of the action to check.
     * @return True if the action is activated, false otherwise.
     */
    bool IsActionActivated(const std::string& action);

    // Mouse

    /**
     * @brief Adds a mouse button mapping.
     *
     * Associates a mouse button with a specific name.
     *
     * @param name The name of the mouse button action.
     * @param buttonCode The SDL button code to associate with the name.
     */
    void AddMouseButton(const std::string& name, int buttonCode);

    /**
     * @brief Marks a mouse button as pressed.
     *
     * Updates the internal state to indicate that a mouse button is currently pressed.
     *
     * @param buttonCode The SDL button code of the button that was pressed.
     */
    void MouseButtonDown(int buttonCode);

    /**
     * @brief Marks a mouse button as released.
     *
     * Updates the internal state to indicate that a mouse button is no longer pressed.
     *
     * @param buttonCode The SDL button code of the button that was released.
     */
    void MouseButtonUp(int buttonCode);

    /**
     * @brief Checks if a mouse button is currently pressed.
     *
     * Determines if the mouse button associated with the specified name is pressed.
     *
     * @param name The name of the mouse button action to check.
     * @return True if the mouse button is pressed, false otherwise.
     */
    bool IsMouseButtonDown(const std::string& name);

    /**
     * @brief Sets the current mouse position.
     *
     * Updates the stored mouse position with the provided coordinates.
     *
     * @param x The new X-coordinate of the mouse.
     * @param y The new Y-coordinate of the mouse.
     */
    void SetMousePosition(int x, int y);

    /**
     * @brief Retrieves the current mouse position.
     *
     * @return A tuple containing the X and Y coordinates of the mouse.
     */
    std::tuple<int, int> GetMousePosition();
};

#endif // CONTROLLERMANAGER_HPP

/** @} */ // end of group