/**
 * @file ScriptComponent.hpp
 * @brief Defines the ScriptComponent used to attach scripting functionality 
 to entities in the game engine, allowing custom behavior through Lua scripting 
 functions for events and updates.
 * @author Juan Torres
 * @date 2024
 * @ingroup Component
 */

#ifndef SCRIPTCOMPONENT_HPP
#define SCRIPTCOMPONENT_HPP

#include <sol/sol.hpp>

 /**
  * @brief Represents a component that allows attaching Lua script functions to an entity.
  *
  * This component provides scripting capabilities for an entity, enabling custom behavior
  * through Lua functions for various events, such as collisions, clicks, and updates.
  */
struct ScriptComponent {
    sol::function onCollision; /**< Lua function called when the entity collides with another. */
    sol::function onClick;     /**< Lua function called when the entity is clicked. */
    sol::function update;      /**< Lua function called during the entity's update cycle. */

    /**
     * @brief Constructs a ScriptComponent with specified Lua functions.
     *
     * @param onCollision The Lua function for handling collision events (default is nil).
     * @param onClick The Lua function for handling click events (default is nil).
     * @param update The Lua function for handling update cycles (default is nil).
     */
    ScriptComponent(sol::function onCollision = sol::lua_nil,
        sol::function onClick = sol::lua_nil,
        sol::function update = sol::lua_nil) {
        this->onCollision = onCollision;
        this->onClick = onClick;
        this->update = update;
    }
};

#endif // SCRIPTCOMPONENT_HPP
