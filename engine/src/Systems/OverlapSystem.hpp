/**
 * @file OverlapSystem.hpp
 * @brief Designed to check collisions between entities and handle the overlap behavior based 
 * on their positions and components.
 * @author Juan Torres
 * @date 2024
 * @ingroup System
 */

#ifndef OVERLAPSYSTEM_HPP
#define OVERLAPSYSTEM_HPP

#include <memory>

#include "../Components/BoxColliderComponent.hpp"
#include "../Components/RigidBodyComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../ECS/ECS.hpp"
#include "../EventManager/EventManager.hpp"
#include "../Events/CollisionEvent.hpp"

/**
 * @brief Enum representing the four cardinal directions.
 *
 * This enum is used to specify directions for collision detection and movement logic.
 */
enum Direction {
    top,    /**< Represents the top direction. */
    left,   /**< Represents the left direction. */
    bottom, /**< Represents the bottom direction. */
    right   /**< Represents the right direction. */
};

/**
 * @brief A system responsible for detecting and handling overlaps between entities.
 *
 * The `OverlapSystem` is designed to check collisions between entities and handle
 * the overlap behavior based on their positions and components.
 */
class OverlapSystem : public System {
private:

    /**
     * @brief Checks if two entities collide in a specific direction.
     *
     * This method checks for collisions between two entities and determines if they overlap
     * in a given direction. It uses the entities' position and collider components for the check.
     *
     * @param a The first entity to check for collision.
     * @param b The second entity to check for collision.
     * @param dir The direction in which to check for overlap.
     * @return `true` if the entities overlap in the specified direction, `false` otherwise.
     */
    bool CheckCollision(Entity a, Entity b, Direction dir) {
        auto& aCollider = a.GetComponent<BoxColliderComponent>();
        auto& bCollider = b.GetComponent<BoxColliderComponent>();
        auto& aTransform = a.GetComponent<TransformComponent>();
        auto& bTransform = b.GetComponent<TransformComponent>();

        // Adjust for offset
        glm::vec2 aPos = aTransform.previousPosition + aCollider.offset;
        glm::vec2 bPos = bTransform.previousPosition + bCollider.offset;

        float aX = aPos.x;
        float aY = aPos.y;
        float aW = static_cast<float>(aCollider.width);
        float aH = static_cast<float>(aCollider.height);

        float bX = bPos.x;
        float bY = bPos.y;
        float bW = static_cast<float>(bCollider.width);
        float bH = static_cast<float>(bCollider.height);

        // Top side of A colliding with bottom side of B
        if (Direction::top == dir) {
            return (
                aX < bX + bW &&
                aX + aW > bX &&
                aY > bY
                );
        }

        // Left side of A colliding with right side of B
        if (Direction::left == dir) {
            return (
                aY < bY + bH &&
                aY + aH > bY &&
                aX > bX
                );
        }

        // Bottom side of A colliding with top side of B
        if (Direction::bottom == dir) {
            return (
                aX < bX + bW &&
                aX + aW > bX &&
                aY < bY
                );
        }

        // Right side of A colliding with left side of B
        if (Direction::right == dir) {
            return (
                aY < bY + bH &&
                aY + aH > bY &&
                aX < bX
                );
        }

        return false;
    }

    /**
     * @brief Resolves overlap between two entities.
     *
     * This method handles the logic to prevent or resolve overlap between two entities by adjusting
     * their positions or other necessary actions. It is typically used to prevent entities from
     * intersecting when they should not.
     *
     * @param a The first entity involved in the potential overlap.
     * @param b The second entity involved in the potential overlap.
     */
    void AvoidOverlap(Entity a, Entity b) {
        auto& aCollider = a.GetComponent<BoxColliderComponent>();
        auto& aTransform = a.GetComponent<TransformComponent>();

        auto& bCollider = b.GetComponent<BoxColliderComponent>();
        auto& bTransform = b.GetComponent<TransformComponent>();
        auto& bRigidbody = b.GetComponent<RigidBodyComponent>();

        // Adjust for offset
        glm::vec2 aPos = aTransform.position + aCollider.offset;
        // glm::vec2 bPos = bTransform.position + bCollider.offset;

        if (CheckCollision(a, b, Direction::top)) {
            // Move entity B upwards
            bTransform.position.y = aPos.y - bCollider.height - bCollider.offset.y;
            bRigidbody.velocity.y = 0.0f;
        }

        if (CheckCollision(a, b, Direction::bottom)) {
            // Move entity B downwards
            bTransform.position.y = aPos.y + aCollider.height - bCollider.offset.y;
            bRigidbody.velocity.y = 0.0f;
        }

        if (CheckCollision(a, b, Direction::left)) {
            // Move entity B to the left
            bTransform.position.x = aPos.x - bCollider.width - bCollider.offset.x;
            bRigidbody.velocity.x = 0.0f;
        }

        if (CheckCollision(a, b, Direction::right)) {
            // Move entity B to the right
            bTransform.position.x = aPos.x + aCollider.width - bCollider.offset.x;
            bRigidbody.velocity.x = 0.0f;
        }
    }

public:
    /**
     * @brief Constructs an OverlapSystem.
     *
     * The constructor initializes the `OverlapSystem`, setting up necessary components and
     * configurations for handling collisions and overlaps between entities.
     */
    OverlapSystem() {
        RequireComponent<BoxColliderComponent>();
        RequireComponent<RigidBodyComponent>();
        RequireComponent<TransformComponent>();
    }

    /**
     * @brief Subscribes to collision events.
     *
     * This method subscribes the `OverlapSystem` to listen for collision events. When a collision event
     * occurs, the system will respond by processing the entities involved in the collision.
     *
     * @param eventManager A reference to the event manager used to subscribe to the collision events.
     */
    void SubscribeToCollisionEvent(
        const std::unique_ptr<EventManager>& eventManager) {
        eventManager->SubscribeToEvent<CollisionEvent, OverlapSystem>(this,
            &OverlapSystem::OnCollisionEvent);
    }

    /**
     * @brief Handles a collision event.
     *
     * This method processes a `CollisionEvent` and resolves the collision between the involved entities.
     * It checks the necessary conditions and applies appropriate responses such as adjusting entity positions
     * or triggering event-based reactions.
     *
     * @param e The collision event containing the information about the collision.
     */
    void OnCollisionEvent(CollisionEvent& e) {
        auto& aRigidbody = e.a.GetComponent<RigidBodyComponent>();
        auto& bRigidbody = e.b.GetComponent<RigidBodyComponent>();

        if (aRigidbody.isSolid && bRigidbody.isSolid) {
            if (aRigidbody.mass >= bRigidbody.mass) {
                AvoidOverlap(e.a, e.b);
            }
            else {
                AvoidOverlap(e.b, e.a);
            }
        }
    }
};

#endif // OVERLAPSYSTEM_HPP
