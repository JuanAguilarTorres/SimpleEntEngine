/**
 * @file ECS.hpp
 * @brief Entity–component–system, architectural pattern used in this game development
 * @author Juan Torres
 * @date 2024
 */

#ifndef ECS_HPP
#define ECS_HPP

#include <bitset>
#include <cstddef>
#include <deque>
#include <iostream>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include "../Utils/Pool.hpp"

const unsigned int MAX_COMPONENTS = 64;

/**
 * @typedef Signature
 * @brief Represents a bitset for tracking the presence of components in an entity.
 * @defgroup Component Component
 * @{
 * @brief This module handles all components in the entity-component system
 */

typedef std::bitset<MAX_COMPONENTS> Signature;

/**
 * @brief Base class for all components.
 */
struct IComponent {
protected:
    static int nextId; /**< Static variable to keep track of component IDs */
};

/**
 * @brief Template class for specific component types.
 * @tparam TComponent The type of component.
 */
template <typename TComponent>
class Component : public IComponent {
public:
    /**
     * @brief Gets the unique ID for the component type.
     * @return The component ID.
     */
    static int GetId() {
        static int id = nextId++;
        return id;
    }
};

/** @} */ // end of group

/**
 * @brief Represents an entity in the ECS architecture.
 * @defgroup Entity Entity
 * @{
 * @brief This module handles all Entity related classes in the ECS architecture
 */

class Entity {
private:
    int id; /**< Unique identifier for the entity */

public:
    /**
     * @brief Constructs an entity with a specific ID.
     * @param id The entity's ID.
     */
    Entity(int id) : id(id) {}

    /**
     * @brief Gets the entity's unique identifier.
     * @return The entity ID.
     */
    int GetId() const;

    /**
     * @brief Marks the entity for destruction.
     */
    void Kill();

    // Comparison operators for entities
    bool operator==(const Entity& other) const { return id == other.id; }
    bool operator!=(const Entity& other) const { return id != other.id; }
    bool operator>(const Entity& other) const { return id > other.id; }
    bool operator<(const Entity& other) const { return id < other.id; }

    // Component management methods
    template <typename TComponent, typename... TArgs>
    void AddComponent(TArgs&&... args);

    template <typename TComponent>
    void RemoveComponent();

    template <typename TComponent>
    bool HasComponent() const;

    template <typename TComponent>
    TComponent& GetComponent() const;

    class Registry* registry; /**< Pointer to the registry managing the entity */
};

/** @} */ // end of group

/**
 * @brief Represents a system that operates on entities with specific component types.
 * @defgroup System System
 * @{
 * @brief This module handles all Systems in the entity-component system
 */

class System {
private:
    Signature componentSignature; /**< Signature indicating required components */
    std::vector<Entity> entities; /**< List of entities associated with the system */

public:
    System() = default;
    ~System() = default;

    /**
     * @brief Adds an entity to the system.
     * @param entity The entity to add.
     */
    void AddEntityToSystem(Entity entity);

    /**
     * @brief Removes an entity from the system.
     * @param entity The entity to remove.
     */
    void RemoveEntityFromSystem(Entity entity);

    /**
     * @brief Gets all entities associated with the system.
     * @return A vector of entities.
     */
    std::vector<Entity> GetSystemEntities() const;

    /**
     * @brief Gets the component signature for the system.
     * @return The component signature.
     */
    const Signature& GetComponentSignature() const;

    /**
     * @brief Requires a component for the system to operate on.
     * @tparam TComponent The component type required.
     */
    template <typename TComponent>
    void RequireComponent();
};

/** @} */ // end of group


/**
 * @brief Manages entities, components, and systems in the ECS architecture.
 * @defgroup Registry Registry
 * @{
 * @brief This module handles the main Registry in the ECS architecture
 */
class Registry {
private:
    int numEntity = 0; /**< Number of entities created */
    std::vector<std::shared_ptr<IPool>> componentsPools; /**< Pools for storing components */
    std::vector<Signature> entityComponentSignatures; /**< Signatures for each entity */
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems; /**< Map of systems */
    std::set<Entity> entitiesToBeAdded; /**< Entities to be added to systems */
    std::set<Entity> entitiesToBeKilled; /**< Entities to be removed from systems */
    std::deque<int> freeIds; /**< Queue of free entity IDs */
    std::set<int> activeEntityIds; /**< Set of active entity IDs */

public:
    /**
     * @brief Constructs the Registry.
     */
    Registry();

    /**
     * @brief Destructs the Registry.
     */
    ~Registry();

    /**
     * @brief Updates the registry, processing entity additions and removals.
     */
    void Update();

    // Entity management methods
    /**
     * @brief Creates a new entity.
     * @return The newly created entity.
     */
    Entity CreateEntity();

    /**
     * @brief Marks an entity for destruction.
     * @param entity The entity to be killed.
     */
    void KillEntity(Entity entity);

    // Component management methods
    template <typename TComponent, typename... TArgs>
    void AddComponent(Entity entity, TArgs&&... args);

    template <typename TComponent>
    void RemoveComponent(Entity entity);

    template <typename TComponent>
    bool HasComponent(Entity entity) const;

    template <typename TComponent>
    TComponent& GetComponent(Entity entity) const;

    // System management methods
    template <typename TSystem, typename... TArgs>
    void AddSystem(TArgs&&... args);

    template <typename TSystem>
    void RemoveSystem();

    template <typename TSystem>
    bool HasSystem() const;

    template <typename TSystem>
    TSystem& GetSystem() const;

    // Methods for managing entities in systems
    /**
     * @brief Adds an entity to relevant systems.
     * @param entity The entity to add.
     */
    void AddEntityToSystems(Entity entity);

    /**
     * @brief Removes an entity from all systems.
     * @param entity The entity to remove.
     */
    void RemoveEntityFromSystems(Entity entity);

    /**
     * @brief Clears all entities from the registry.
     */
    void ClearAllEntities();

private:
    /**
     * @brief Gets the next available entity ID.
     * @return The next available ID.
     */
    int GetNextAvailableId();
};

/**
 * @brief Specifies that the system requires a particular component.
 *
 * Adds the component type to the system's signature, indicating that
 * entities with this component will be added to the system.
 *
 * @tparam TComponent The type of the component required by the system.
 */
template <typename TComponent>
void System::RequireComponent() {
    const int componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

/**
 * @brief Adds a component to the specified entity.
 *
 * This method creates a new component of type TComponent for the given entity.
 * If the component pool does not exist for this component type, it is created.
 * The entity's component signature is updated to reflect the addition.
 *
 * @tparam TComponent The type of the component to add.
 * @tparam TArgs Variadic arguments for the component's constructor.
 * @param entity The entity to which the component will be added.
 * @param args Arguments used to construct the component.
 */
template <typename TComponent, typename ... TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ... args) {
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();

    if (static_cast<long unsigned int>(componentId) >= componentsPools.size()) {
        componentsPools.resize(componentId + 10, nullptr);
    }

    if (!componentsPools[componentId]) {
        std::shared_ptr<Pool<TComponent>> newComponentPool
            = std::make_shared<Pool<TComponent>>();
        componentsPools[componentId] = newComponentPool;
    }

    std::shared_ptr<Pool<TComponent>> componentPool
        = std::static_pointer_cast<Pool<TComponent>>(componentsPools[componentId]);

    if (entityId >= componentPool->GetSize()) {
        componentPool->Resize(numEntity + 100);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);
    componentPool->Set(entityId, newComponent);
    entityComponentSignatures[entityId].set(componentId);
}

/**
 * @brief Removes a component from the specified entity.
 *
 * This method updates the entity's component signature to indicate
 * that the specified component has been removed.
 *
 * @tparam TComponent The type of the component to remove.
 * @param entity The entity from which the component will be removed.
 */
template <typename TComponent>
void Registry::RemoveComponent(Entity entity) {
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();
    entityComponentSignatures[entityId].set(componentId, false);
}

/**
 * @brief Checks if an entity has a specific component.
 *
 * @tparam TComponent The type of the component to check for.
 * @param entity The entity to check for the component.
 * @return True if the entity has the component, otherwise false.
 */
template <typename TComponent>
bool Registry::HasComponent(Entity entity) const {
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();
    return entityComponentSignatures[entityId].test(componentId);
}

/**
 * @brief Retrieves a component of the specified type from the entity.
 *
 * @tparam TComponent The type of the component to retrieve.
 * @param entity The entity from which to get the component.
 * @return A reference to the component of type TComponent.
 */
template <typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const {
    const int componentId = Component<TComponent>::GetId();
    const int entityId = entity.GetId();
    auto componentPool
        = std::static_pointer_cast<Pool<TComponent>>(componentsPools[componentId]);
    return componentPool->Get(entityId);
}

/**
 * @brief Adds a system to the registry.
 *
 * Creates a new system of the specified type and adds it to the registry.
 *
 * @tparam TSystem The type of the system to add.
 * @tparam TArgs Variadic arguments for the system's constructor.
 * @param args Arguments used to construct the system.
 */
template <typename TSystem, typename ... TArgs>
void Registry::AddSystem(TArgs&& ... args) {
    std::shared_ptr<TSystem> newSystem
        = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

/**
 * @brief Removes a system from the registry.
 *
 * @tparam TSystem The type of the system to remove.
 */
template <typename TSystem>
void Registry::RemoveSystem() {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
}

/**
 * @brief Checks if the registry has a specific system.
 *
 * @tparam TSystem The type of the system to check for.
 * @return True if the registry has the system, otherwise false.
 */
template <typename TSystem>
bool Registry::HasSystem() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

/**
 * @brief Retrieves a system from the registry.
 *
 * @tparam TSystem The type of the system to retrieve.
 * @return A reference to the system of type TSystem.
 */
template <typename TSystem>
TSystem& Registry::GetSystem() const {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}

/**
 * @brief Adds a component to an entity.
 *
 * @tparam TComponent The type of the component to add.
 * @tparam TArgs Variadic arguments for the component's constructor.
 * @param args Arguments used to construct the component.
 */
template <typename TComponent, typename ... TArgs>
void Entity::AddComponent(TArgs&& ... args) {
    registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

/**
 * @brief Removes a component from the entity.
 *
 * @tparam TComponent The type of the component to remove.
 */
template <typename TComponent>
void Entity::RemoveComponent() {
    registry->RemoveComponent<TComponent>(*this);
}

/**
 * @brief Checks if the entity has a specific component.
 *
 * @tparam TComponent The type of the component to check for.
 * @return True if the entity has the component, otherwise false.
 */
template <typename TComponent>
bool Entity::HasComponent() const {
    return registry->HasComponent<TComponent>(*this);
}

/**
 * @brief Retrieves a component from the entity.
 *
 * @tparam TComponent The type of the component to retrieve.
 * @return A reference to the component of type TComponent.
 */
template <typename TComponent>
TComponent& Entity::GetComponent() const {
    return registry->GetComponent<TComponent>(*this);
}

/** @} */ // end of group

#endif // ECS_HPP