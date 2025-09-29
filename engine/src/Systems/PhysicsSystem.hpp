/**
 * @file OverlapSystem.hpp
 * @brief Responsible for processing physics-related computations.
 * @author Juan Torres
 * @date 2024
 * @ingroup System
 */


#ifndef PHYSICSSYSTEM_HPP
#define PHYSICSSYSTEM_HPP

#include "../Components/RigidBodyComponent.hpp"
#include "../ECS/ECS.hpp"

/**
 * @brief A system responsible for handling physics updates for entities.
 *
 * The `PhysicsSystem` is responsible for processing physics-related computations, such as
 * position updates, velocity adjustments, and applying forces to entities with the necessary
 * physics components. This system is typically updated each frame to simulate real-time physics.
 */
class PhysicsSystem : public System {
public:
	/**
	 * @brief Constructs a PhysicsSystem.
	 *
	 * The constructor initializes the `PhysicsSystem`, setting up the necessary components
	 * and configurations required to handle physics simulations for entities.
	 */
	PhysicsSystem() {
		RequireComponent<RigidBodyComponent>();
	}

	/**
	 * @brief Updates the physics system.
	 *
	 * This method processes all entities with the required physics components, updating their
	 * states based on their velocities, positions, and any other relevant physical properties.
	 * It also handles collision resolution and other physics-based interactions.
	 */
	void Update() {
		for (auto entity : GetSystemEntities()) {
			auto& rigidbody = entity.GetComponent<RigidBodyComponent>();

			// Aplicar la fuerza de gravedad
			if (rigidbody.isDynamic) {
				rigidbody.sumForces += glm::vec2(0.0f, 9.8 * rigidbody.mass * 64);
			}
		}
	}
};

#endif // PHYSICSSYSTEM_HPP