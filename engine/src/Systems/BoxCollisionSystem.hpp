/**
 * @file BoxCollisionSystem.hpp
 * @brief Defines the BoxCollisionSystem responsible for handling AABB collision detection between entities.
 * @author Juan Torres
 * @date 2024
 * @ingroup System
 */

#ifndef BOXCOLLISIONSYSTEM_HPP
#define BOXCOLLISIONSYSTEM_HPP

#include <memory>
#include <sol/sol.hpp>
#include "../Components/BoxColliderComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../Components/PropertyComponent.hpp"
#include "../Components/ScriptComponent.hpp"
#include "../ECS/ECS.hpp"
#include "../EventManager/EventManager.hpp"
#include "../Events/CollisionEvent.hpp"

 /**
  * @brief Represents the system that manages box collisions between entities.
  *
  * The BoxCollisionSystem detects and manages collisions between entities with
  * box colliders. It checks for Axis-Aligned Bounding Box (AABB) collisions
  * and triggers onCollision scripts when collisions occur.
  */
class BoxCollisionSystem : public System {
private:
    /**
     * @brief Checks for AABB collision between two rectangles.
     *
     * @param aX The x-position of the first rectangle.
     * @param aY The y-position of the first rectangle.
     * @param aw The width of the first rectangle.
     * @param aH The height of the first rectangle.
     * @param bX The x-position of the second rectangle.
     * @param bY The y-position of the second rectangle.
     * @param bw The width of the second rectangle.
     * @param bH The height of the second rectangle.
     * @return True if there is a collision; otherwise, false.
     *
     * This function checks if two rectangular bounding boxes intersect
     * based on their positions and dimensions.
     */
    bool CheckAABBCollision(float aX, float aY, float aw, float aH, float bX, float bY, float bw, float bH) {
        return (
            aX < bX + bw &&
            aX + aw > bX &&
            aY < bY + bH &&
            aY + aH > bY
            );
    }

public:
    /**
     * @brief Constructs a BoxCollisionSystem.
     *
     * This constructor specifies that entities using this system must have
     * BoxColliderComponent and TransformComponent.
     */
    BoxCollisionSystem() {
        RequireComponent<BoxColliderComponent>();
        RequireComponent<TransformComponent>();
    }

    /**
     * @brief Updates collision status and triggers collision scripts for entities.
     *
     * @param lua The Lua state, used for executing Lua scripts.
     *
     * This function iterates over all entities in the system, checks for collisions
     * between their box colliders, and triggers onCollision Lua scripts if a collision
     * occurs. Each entity’s onCollision script is executed if present.
     */
    void Update(const std::unique_ptr<EventManager>& eventManager, sol::state& lua) {
        auto entities = GetSystemEntities();

        for (auto i = entities.begin(); i != entities.end(); i++) {
            Entity a = *i;
            const auto& aCollider = a.GetComponent<BoxColliderComponent>();
            const auto& aTransform = a.GetComponent<TransformComponent>();

            glm::vec2 aPos = aTransform.position + aCollider.offset;

            for (auto j = i; j != entities.end(); j++) {
                Entity b = *j;

                if (a == b) continue;

                const auto& bCollider = b.GetComponent<BoxColliderComponent>();
                const auto& bTransform = b.GetComponent<TransformComponent>();

                glm::vec2 bPos = bTransform.position + bCollider.offset;

                // Check for exclusion based on PropertyComponent
                if (b.HasComponent<PropertyComponent>()) {
                    const auto& bProperty = b.GetComponent<PropertyComponent>();
                    if (aCollider.IsExcluded(bProperty.tag)) {
                        continue; // Skip processing collision for excluded tags
                    }
                }

                bool collision = CheckAABBCollision(
                    aPos.x, aPos.y,
                    static_cast<float>(aCollider.width), static_cast<float>(aCollider.height),
                    bPos.x, bPos.y,
                    static_cast<float>(bCollider.width), static_cast<float>(bCollider.height)
                );

                if (collision) {
                    eventManager->EmitEvent<CollisionEvent>(a, b);

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
};

#endif // BOXCOLLISIONSYSTEM_HPP
