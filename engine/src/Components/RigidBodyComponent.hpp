/**
 * @file RigidBodyComponent.hpp
 * @brief Defines the RigidBodyComponent used to add physical movement 
 to entities in the game engine through velocity management and simulation of rigid body dynamics.
 * @author Juan Torres
 * @date 2024
 * @ingroup Component
 */

#ifndef RIGIDBODYCOMPONENT_HPP
#define RIGIDBODYCOMPONENT_HPP

#include <glm/glm.hpp>

/**
 * @brief Represents a component that adds velocity to an entity for simulating rigid body movement.
 *
 * This component is used to manage the velocity of an entity, enabling it to move
 * in the game world. The velocity can be updated to simulate physical forces or
 * movement over time.
 */
struct RigidBodyComponent {
    bool isDynamic;
    bool isSolid;

    glm::vec2 sumForces = glm::vec2(0);
    glm::vec2 acceleration = glm::vec2(0);
    glm::vec2 velocity = glm::vec2(0); /**< The velocity vector representing the entity's speed and direction. */

    float mass;
    float invMass;

    /**
     * @brief Constructs a RigidBodyComponent with specified physical properties.
     *
     * @param isDynamic Indicates whether the rigid body is dynamic (can move). Default is `false`.
     * @param isSolid Indicates whether the rigid body is solid (interacts with other objects). Default is `false`.
     * @param mass The mass of the rigid body. Must be greater than 0. Default is `1`.
     */
    RigidBodyComponent(bool isDynamic = false, bool isSolid = false,
        float mass = 1) {
        this->isDynamic = isDynamic;
        this->isSolid = isSolid;
        this->mass = mass;
        this->invMass = 1 / mass;

    }
};

#endif // RIGIDBODYCOMPONENT_HPP
