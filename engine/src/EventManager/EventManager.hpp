/**
 * @file EventManager.hpp
 * @brief Manager Class for Events
 * @author Juan Torres
 * @date 2024
 * @defgroup EventManager Event Management
 * @{
 * @brief Manages all the events of the project
 */

#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <typeindex>

#include "Event.hpp"

/**
 * @brief Interface for event callback handling.
 *
 * Provides a base interface for classes that can handle events. Implementations
 * of this interface should define the Call function to perform the callback.
 */
class IEventCallback {
private:
    /**
     * @brief Pure virtual function to be implemented by derived classes to call the event.
     *
     * @param e The event to be processed.
     */
    virtual void Call(Event& e) = 0;

public:
    /**
     * @brief Virtual destructor for the IEventCallback class.
     */
    virtual ~IEventCallback() = default;

    /**
     * @brief Executes the callback for a given event.
     *
     * @param e The event to execute the callback for.
     */
    void Execute(Event& e) {
        Call(e);
    }
};

/**
 * @brief Template class for handling event callbacks.
 *
 * The EventCallback class allows the binding of a member function of an owner
 * class to an event, so that the member function will be called when the event
 * occurs.
 *
 * @tparam TOwner The type of the owner class.
 * @tparam TEvent The type of the event.
 */
template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {
private:
    typedef void (TOwner::* CallbackFunction)(TEvent&);

    TOwner* ownerInstance;            ///< Pointer to the instance of the owner class.
    CallbackFunction callbackFunction; ///< The member function to call when the event occurs.

    /**
     * @brief Calls the callback function with the given event.
     *
     * @param e The event to process.
     */
    virtual void Call(Event& e) override {
        std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
    }

public:
    /**
     * @brief Constructs an EventCallback.
     *
     * @param ownerInstance Pointer to the owner class instance.
     * @param callbackFunction The member function to call when the event occurs.
     */
    EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction) {
        this->callbackFunction = callbackFunction;
        this->ownerInstance = ownerInstance;
    }
};

/**
 * @brief Alias for a list of event handlers.
 */
typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

/**
 * @brief Manages event subscription and emission.
 *
 * The EventManager class is responsible for managing event listeners and
 * notifying them when events are emitted. It supports subscribing to events
 * and emitting events to notify subscribers.
 */
class EventManager {
private:
    std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers; ///< Map of event subscribers.

public:
    /**
     * @brief Constructs an EventManager.
     *
     * Outputs a message indicating that the constructor has been executed.
     */
    EventManager() {
        std::cout << "[EventManager] Constructor is executed" << std::endl;
    }

    /**
     * @brief Destructor for the EventManager.
     *
     * Outputs a message indicating that the destructor has been executed.
     */
    ~EventManager() {
        std::cout << "[EventManager] Destructor completed" << std::endl;
    }

    /**
     * @brief Clears all event subscriptions.
     *
     * Resets the EventManager, removing all subscribers.
     */
    void Reset() {
        subscribers.clear();
    }

    /**
     * @brief Subscribes an owner instance to a specific event type.
     *
     * @tparam TEvent The type of the event to subscribe to.
     * @tparam TOwner The type of the owner instance.
     * @param ownerInstance Pointer to the owner instance.
     * @param callbackFunction The member function to be called when the event is emitted.
     */
    template <typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::* callbackFunction)(TEvent&)) {
        if (!subscribers[typeid(TEvent)].get()) {
            subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }

        auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
        subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    /**
     * @brief Emits an event, notifying all subscribers.
     *
     * Creates an event of type TEvent and invokes all subscribed callbacks for that event type.
     *
     * @tparam TEvent The type of the event to emit.
     * @tparam TArgs The types of the arguments used to construct the event.
     * @param args The arguments used to construct the event.
     */
    template <typename TEvent, typename ...TArgs>
    void EmitEvent(TArgs&& ... args) {
        auto handlers = subscribers[typeid(TEvent)].get();
        if (handlers) {
            for (auto it = handlers->begin(); it != handlers->end(); it++) {
                auto handler = it->get();
                TEvent event(std::forward<TArgs>(args)...);
                handler->Execute(event);
            }
        }
    }
};

#endif // EVENTMANAGER_HPP

/** @} */ // end of group