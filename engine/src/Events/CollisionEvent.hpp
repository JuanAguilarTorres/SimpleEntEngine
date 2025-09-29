/**
 * @file CollisionEvent.hpp
 * @brief Class Event for Collision
 * @author Juan Torres
 * @date 2024
 * @ingroup EventManager
 */

#ifndef COLLISIONEVENT_HPP
#define COLLISIONEVENT_HPP

#include "../ECS/ECS.hpp"
#include "../EventManager/Event.hpp"

/**
 * @brief Represents a collision event between two entities.
 *
 * The CollisionEvent class is used to encapsulate information about a collision
 * that occurs between two entities within the system.
 */
class CollisionEvent : public Event {
public:
    /**
     * @brief The first entity involved in the collision.
     */
    Entity a;

    /**
     * @brief The second entity involved in the collision.
     */
    Entity b;

    /**
     * @brief Constructs a new CollisionEvent.
     *
     * @param a The first entity involved in the collision.
     * @param b The second entity involved in the collision.
     */
    CollisionEvent(Entity a, Entity b) : a(a), b(b) {}
};

#endif // COLLISIONEVENT_HPP
