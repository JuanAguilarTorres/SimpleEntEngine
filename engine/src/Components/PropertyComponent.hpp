/**
 * @file PropertyComponent.hpp
 * @brief Defines the PropertyComponent used to assign a tag or property 
 to an entity in the game engine for identification or categorization purposes.
 * @author Juan Torres
 * @date 2024
 * @ingroup Component
 */

#ifndef PROPERTYCOMPONENT_HPP
#define PROPERTYCOMPONENT_HPP

#include <string>
#include <SDL2/SDL.h>

 /**
  * @brief Represents a component that assigns a tag or property to an entity.
  *
  * This component can be used to categorize or identify entities by assigning
  * a tag, which can help with filtering or grouping entities during gameplay.
  */
struct PropertyComponent {
    std::string tag; /**< The tag or property associated with the entity. */

    /**
     * @brief Constructs a PropertyComponent with a specified tag.
     *
     * @param tag The tag assigned to the entity (default is "none").
     */
    PropertyComponent(const std::string& tag = "none") {
        this->tag = tag;
    }
};

#endif // PROPERTYCOMPONENT_HPP
