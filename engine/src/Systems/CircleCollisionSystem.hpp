/**
 * @file CircleCollisionSystem.hpp
 * @brief Defines the CollisionSystem responsible for detecting and handling collisions between entities.
 * @author Juan Torres
 * @date 2024
 * @ingroup System
 */

#ifndef CIRCLECOLLISIONSYSTEM_HPP
#define CIRCLECOLLISIONSYSTEM_HPP

#include <memory>
#include <sol/sol.hpp>
#include "../Components/CircleColliderComponent.hpp"
#include "../Components/ScriptComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../ECS/ECS.hpp"
#include "../EventManager/EventManager.hpp"
#include "../Events/CollisionEvent.hpp"

 /**
  * @brief Represents the system that manages collision detection and response between entities.
  *
  * The CollisionSystem checks for circular collisions between entities with CircleColliderComponent
  * and triggers collision events if necessary.
  */
class CircleCollisionSystem : public System {
public:
    /**
     * @brief Constructs a CollisionSystem.
     *
     * This constructor specifies that entities using this system must have
     * CircleColliderComponent and TransformComponent.
     */
    CircleCollisionSystem() {
        RequireComponent<CircleColliderComponent>();
        RequireComponent<TransformComponent>();
    }

    /**
     * @brief Updates the collision system to check for collisions between entities.
     *
     * @param lua A reference to the Lua state used for scripting.
     *
     * This function iterates through all entities in the system, checking for circular
     * collisions between pairs of entities. If a collision is detected, it triggers
     * the onCollision scripts associated with the entities involved.
     */
    void Update(sol::state& lua) {
        auto entities = GetSystemEntities();

        for (auto i = entities.begin(); i != entities.end(); i++) {
            Entity a = *i;
            auto aCollider = a.GetComponent<CircleColliderComponent>();
            auto aTransform = a.GetComponent<TransformComponent>();

            for (auto j = i; j != entities.end(); j++) {
                Entity b = *j;

                if (a == b) {
                    continue; // Skip self-collision
                }

                auto bCollider = b.GetComponent<CircleColliderComponent>();
                auto bTransform = b.GetComponent<TransformComponent>();

                glm::vec2 aCenterPos = glm::vec2(
                    aTransform.position.x - (aCollider.width / 2) * aTransform.scale.x,
                    aTransform.position.y - (aCollider.height / 2) * aTransform.scale.y
                );

                glm::vec2 bCenterPos = glm::vec2(
                    bTransform.position.x - (bCollider.width / 2) * bTransform.scale.x,
                    bTransform.position.y - (bCollider.height / 2) * bTransform.scale.y
                );

                int aRadius = aCollider.radius * aTransform.scale.x;
                int bRadius = bCollider.radius * bTransform.scale.x;

                bool collision = CheckCircularCollision(aRadius, bRadius, aCenterPos, bCenterPos);

                if (collision) {
                    // Trigger onCollision script for entity a
                    if (a.HasComponent<ScriptComponent>()) {
                        const auto& script = a.GetComponent<ScriptComponent>();
                        if (script.onCollision != sol::nil) {
                            lua["this"] = a;
                            script.onCollision(b);
                        }
                    }

                    // Trigger onCollision script for entity b
                    if (b.HasComponent<ScriptComponent>()) {
                        const auto& script = b.GetComponent<ScriptComponent>();
                        if (script.onCollision != sol::nil) {
                            lua["this"] = b;
                            script.onCollision(a);
                        }
                    }
                }
            }
        }
    }

    /**
     * @brief Checks for circular collision between two entities.
     *
     * @param aRadius The radius of the first entity's collider.
     * @param bRadius The radius of the second entity's collider.
     * @param aPos The position of the first entity's collider.
     * @param bPos The position of the second entity's collider.
     * @return true if a collision is detected, false otherwise.
     */
    bool CheckCircularCollision(int aRadius, int bRadius, glm::vec2 aPos, glm::vec2 bPos) {
        glm::vec2 dif = aPos - bPos;
        double length = glm::sqrt((dif.x * dif.x) + (dif.y * dif.y));
        return (aRadius + bRadius) >= length;
    }
};

#endif // CIRCLECOLLISIONSYSTEM_HPP
