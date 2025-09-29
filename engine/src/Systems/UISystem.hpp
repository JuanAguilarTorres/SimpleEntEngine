/**
 * @file UISystem.hpp
 * @brief Defines the UISystem responsible for managing user interface interactions.
 * @author Juan Torres
 * @date 2024
 * @ingroup System
 */

#ifndef UISYSTEM_HPP
#define UISYSTEM_HPP

#include <SDL2/SDL.h>

#include <memory>
#include <string>

#include "../Components/ClickableComponent.hpp"
#include "../Components/ScriptComponent.hpp"
#include "../Components/TextComponent.hpp"
#include "../Components/TransformComponent.hpp"
#include "../ECS/ECS.hpp"
#include "../EventManager/EventManager.hpp"
#include "../Events/ClickEvent.hpp"

 /**
  * @brief Represents the system that handles user interface interactions.
  *
  * The UISystem manages clickable entities and triggers corresponding
  * actions when they are clicked by the user.
  */
class UISystem : public System {
public:
    /**
     * @brief Constructs a UISystem.
     *
     * This constructor specifies that entities using this system must have
     * ClickableComponent, TextComponent, and TransformComponent.
     */
    UISystem() {
        RequireComponent<ClickableComponent>();
        RequireComponent<TextComponent>();
        RequireComponent<TransformComponent>();
    }

    /**
     * @brief Subscribes to click events.
     *
     * @param eventManager A unique pointer to the EventManager for managing events.
     *
     * This function registers the UISystem to listen for ClickEvent instances.
     */
    void SubscribeToClickEvent(std::unique_ptr<EventManager>& eventManager) {
        eventManager->SubscribeToEvent<ClickEvent, UISystem>(this,
            &UISystem::OnClickEvent);
    }

    /**
     * @brief Handles click events and triggers associated actions.
     *
     * @param e A reference to the ClickEvent that contains the click position.
     *
     * This function checks if any clickable entities were clicked and
     * executes their onClick scripts if defined.
     */
    void OnClickEvent(ClickEvent& e) {
        for (auto entity : GetSystemEntities()) {
            const auto& text = entity.GetComponent<TextComponent>();
            const auto& transform = entity.GetComponent<TransformComponent>();

            if (transform.position.x < e.posX
                && e.posX < transform.position.x + text.width
                && transform.position.y < e.posY
                && e.posY < transform.position.y + text.height) {
                if (entity.HasComponent<ScriptComponent>()) {
                    const auto& script = entity.GetComponent<ScriptComponent>();
                    if (script.onClick != sol::nil) {
                        script.onClick();
                    }
                }
            }
        }
    }
};

#endif // UISYSTEM_HPP
