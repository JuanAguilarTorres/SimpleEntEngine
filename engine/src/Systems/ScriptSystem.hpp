/**
 * @file ScriptSystem.hpp
 * @brief Defines the ScriptSystem responsible for managing Lua scripts for entities.
 * @author Juan Torres
 * @date 2024
 * @ingroup System
 */

#ifndef SCRIPTSYSTEM_HPP
#define SCRIPTSYSTEM_HPP

#include <memory>
#include <sol/sol.hpp>

#include "../Binding/LuaBinding.hpp"
#include "../Components/ScriptComponent.hpp"
#include "../ECS/ECS.hpp"

 /**
  * @brief Represents the system that manages Lua scripting for entities.
  *
  * The ScriptSystem handles the execution of Lua scripts associated with
  * entities and provides Lua functions for entity manipulation.
  */
class ScriptSystem : public System {
public:
    /**
     * @brief Constructs a ScriptSystem.
     *
     * This constructor specifies that entities using this system must have
     * a ScriptComponent.
     */
    ScriptSystem() {
        RequireComponent<ScriptComponent>();
    }

    /**
     * @brief Creates Lua bindings for entity-related functions.
     *
     * @param lua A reference to the Lua state where the bindings will be added.
     *
     * This function sets up various functions and user types that can be
     * accessed from Lua scripts.
     */
    void CreateLuaBinding(sol::state& lua) {
        // Classes
        lua.new_usertype<Entity>("entity");

        // Functions
        lua.set_function("change_animation", ChangeAnimation);
        lua.set_function("flip_sprite", FlipSprite);
        lua.set_function("is_action_activated", IsActionActivated);
        lua.set_function("play_sfx", PlaySound);
        lua.set_function("get_velocity", GetVelocity);
        lua.set_function("set_velocity", SetVelocity);
        lua.set_function("add_force", AddForce);
        lua.set_function("go_to_scene", GoToScene);
        lua.set_function("get_tag", GetTag);
        lua.set_function("set_tag", SetTag);
        lua.set_function("get_Position", GetPosition);
        lua.set_function("set_Position", SetPosition);
        lua.set_function("get_Rotation", GetRotation);
        lua.set_function("set_Rotation", SetRotation);
        lua.set_function("get_3DRotation", Get3DRotation);
        lua.set_function("set_3DRotation", Set3DRotation);
        lua.set_function("get_size", GetSize);
        lua.set_function("set_Text", SetText);
        lua.set_function("load_replica", LoadReplica);
        lua.set_function("load_replica_xy", LoadReplicaXY);
        lua.set_function("get_Data", GetEntityData);
        lua.set_function("delete_Entity", DeleteEntity);
        lua.set_function("get_time", GetTime);
        lua.set_function("left_collision", LeftCollision);
        lua.set_function("right_collision", RightCollision);
        lua.set_function("set_rigid", SetRigid);
        lua.set_function("get_box", GetBox);
        lua.set_function("add_exclusion", AddBoxExclusion);
        lua.set_function("remove_exclusion", RemoveBoxExclusion);
        lua.set_function("is_excluded", IsBoxExcluded);
        lua.set_function("get_dir_collision", GetDirCollision);
        lua.set_function("get_mouse", GetMousePosition);
        lua.set_function("get_dir_collision", GetDirCollision);
        lua.set_function("set_solid", SetSolid);
        lua.set_function("get_solid", GetSolid);
        lua.set_function("set_shadow", SetShadow);
    }

    /**
     * @brief Updates the script components of entities.
     *
     * @param lua A reference to the Lua state for executing scripts.
     *
     * This function iterates over all entities with a ScriptComponent and
     * executes their associated update function if defined.
     */
    void Update(sol::state& lua) {
        for (auto entity : GetSystemEntities()) {
            const auto& script = entity.GetComponent<ScriptComponent>();

            if (script.update != sol::lua_nil) {
                lua["this"] = entity;
                script.update();
            }
        }
    }
};

#endif // SCRIPTSYSTEM_HPP
