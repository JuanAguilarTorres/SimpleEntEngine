#include <algorithm>
#include "ECS.hpp"


int IComponent::nextId = 0;

int Entity::GetId() const {
	return id;
};

void Entity::Kill() {
	registry->KillEntity(*this);
}

void System::AddEntityToSystem(Entity entity) {
	entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
	auto it = std::remove_if(entities.begin(), entities.end()
		, [&entity](Entity other){ return entity == other; });
	entities.erase(it, entities.end()); 
}

std::vector<Entity> System::GetSystemEntities() const {
	return entities;
}

const Signature& System::GetComponentSignature() const {
	return componentSignature;
}

Registry::Registry() {
	std::cout << "[Registry] Constructor is executed" << std::endl;
}

Registry::~Registry() {
	std::cout << "[Registry] Destructor completed" << std::endl;
}

Entity Registry::CreateEntity() {
	int entityId;
	// std::cout << "[Registry] Current numEntity is: " << numEntity << std::endl;

	if (freeIds.empty()) {
		entityId = numEntity++;
		// std::cout << "[Registry] No free IDs! New id: " << entityId << std::endl;
		if (static_cast<size_t>(entityId) >= entityComponentSignatures.size()) {
			entityComponentSignatures.resize(entityId + 100);
		}
	}
	else {
		entityId = freeIds.front();
		freeIds.pop_front();
		// std::cout << "[Registry] Attempting to reuse entity ID: " << entityId << std::endl;
	}

	// Check if the ID is already active
	while (activeEntityIds.find(entityId) != activeEntityIds.end()) {
		// std::cout << std::endl << "[Registry][MANAGED] ID " << entityId 
		//	<< " is still active. Finding next available ID." << std::endl << std::endl;
		if (freeIds.empty()) {
			entityId = numEntity++;
			// std::cout << "[Registry] No more free IDs. Using new ID: " << entityId << std::endl;
			if (static_cast<size_t>(entityId) >= entityComponentSignatures.size()) {
				entityComponentSignatures.resize(entityId + 100);
			}
		}
		else {
			entityId = freeIds.front();
			freeIds.pop_front();
			// std::cout << "[Registry] Attempting to reuse next free ID: " << entityId << std::endl;
		}
	}

	Entity entity(entityId);
	entity.registry = this;

	// Ensure the entity is not in entitiesToBeKilled before adding it
	if (entitiesToBeKilled.find(entity) == entitiesToBeKilled.end()) {
		entitiesToBeAdded.insert(entity);
		activeEntityIds.insert(entityId);  // Mark the ID as active
		// std::cout << "[Registry] Marked entity ID " << entityId << " as active" << std::endl;
	}
	else {
		std::cerr << "[Registry] Attempted to reuse entity ID that was marked for killing: " << entityId << std::endl;
		// In this case, we should probably recursively call CreateEntity to get a valid ID
		return CreateEntity();
	}

	// std::cout << "[Registry] Entity created: " << entityId << std::endl;
	return entity;
}



void Registry::KillEntity(Entity entity) {
	int entityId = entity.GetId();
	if (activeEntityIds.find(entityId) == activeEntityIds.end()) {
		std::cerr << "[ERROR] Attempting to kill non-existent entity: " << entityId << std::endl;
		return;
	}

	RemoveEntityFromSystems(entity);
	entityComponentSignatures[entityId].reset();
	entitiesToBeKilled.insert(entity);
}


void Registry::AddEntityToSystems(Entity entity) {
	const int entityId = entity.GetId();

	const auto& entityComponentSignature = entityComponentSignatures[entityId];

	for (auto& system : systems) {
		const auto& systemComponentSignature
		= system.second->GetComponentSignature();

		bool isInterested = (entityComponentSignature & systemComponentSignature)
		== systemComponentSignature;

		if (isInterested) {
			system.second->AddEntityToSystem(entity);
		}
	}
}

void Registry::RemoveEntityFromSystems(Entity entity) {
	for (auto system : systems) {
		system.second->RemoveEntityFromSystem(entity);
	}
}

void Registry::Update() {
    for (auto entity : entitiesToBeAdded) {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();

    for (auto entity : entitiesToBeKilled) {
        int entityId = entity.GetId();
        RemoveEntityFromSystems(entity);
        entityComponentSignatures[entityId].reset();
        activeEntityIds.erase(entityId);  // Remove from active IDs
		// std::cout << "[REGISTRY][Freeing] Now Freeing ID: " << entityId << std::endl;
        freeIds.push_back(entityId);
    }
    entitiesToBeKilled.clear();
}


void Registry::ClearAllEntities() {
	std::cout << "[REGISTRY] Now Freeing All ID" << std::endl;
	for (int entityId : activeEntityIds) {
		RemoveEntityFromSystems(Entity(entityId));
		entityComponentSignatures[entityId].reset();
		// std::cout << "[REGISTRY][Freeing] Now Freeing All ID: " << entityId << std::endl;
		freeIds.push_back(entityId);
	}
	activeEntityIds.clear();
}

int Registry::GetNextAvailableId() {
	int entityId;
	if (!freeIds.empty()) {
		entityId = freeIds.front();
		freeIds.pop_front();
	}
	else {
		entityId = numEntity++;
	}
	return entityId;
}