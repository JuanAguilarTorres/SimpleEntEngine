/**
 * @file Event.hpp
 * @brief Event Class
 * @author Juan Torres
 * @date 2024
 * @ingroup EventManager
 */

#ifndef EVENT_HPP
#define EVENT_HPP

/**
 * @brief Base class for all events in the system.
 *
 * The Event class serves as a base class for creating different types of events
 * in the system. It provides a common interface that can be extended by various
 * event types.
 */
class Event {
public:
    /**
     * @brief Default constructor for the Event class.
     *
     * Constructs an Event object. This constructor is marked as `default`,
     * indicating that the default behavior is sufficient.
     */
    Event() = default;
};

#endif // EVENT_HPP
