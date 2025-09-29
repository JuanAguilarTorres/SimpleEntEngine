/**
 * @file MovementSystem.hpp
 * @brief Defines the MovementSystem responsible for updating entity positions based on their velocities.
 * @author Juan Torres
 * @date 2024
 * @ingroup System
 */

#ifndef MOVEMENTSYSTEM_HPP
#define MOVEMENTSYSTEM_HPP

#include <glm/glm.hpp>

#include "../Components/RigidBodyComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../ECS/ECS.hpp"

 /**
  * @brief Represents the system that manages the movement of entities.
  *
  * The MovementSystem updates the position of entities based on their
  * velocity as defined in the RigidBodyComponent.
  */
class MovementSystem : public System {
public:
    /**
     * @brief Constructs a MovementSystem.
     *
     * This constructor specifies that entities using this system must have
     * RigidBodyComponent and TransformComponent.
     */
    MovementSystem() {
        RequireComponent<RigidBodyComponent>();
        RequireComponent<TransformComponent>();
    }

    /**
     * @brief Updates the position of entities based on their velocity.
     *
     * @param dt The delta time since the last update.
     *
     * This function iterates over all entities in the system and updates their
     * positions according to their velocity and the elapsed time.
     */
    void Update(double dt) {
        for (auto entity : GetSystemEntities()) {
            auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
            auto& transform = entity.GetComponent<TransformComponent>();

            transform.previousPosition = transform.position;

            if (rigidbody.isDynamic) {
                rigidbody.acceleration = rigidbody.sumForces * rigidbody.invMass;
                rigidbody.velocity += rigidbody.acceleration * static_cast<float>(dt);
                transform.position += rigidbody.velocity * static_cast<float>(dt);
                rigidbody.sumForces = glm::vec2(0);
            }
            else {
                // Update position based on velocity and delta time
                transform.position.x += rigidbody.velocity.x * dt;
                transform.position.y += rigidbody.velocity.y * dt;
            }
        }
    }
};

#endif // MOVEMENTSYSTEM_HPP
