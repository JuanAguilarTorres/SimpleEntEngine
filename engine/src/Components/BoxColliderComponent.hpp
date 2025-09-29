/**
 * @file BoxColliderComponent.hpp
 * @brief Box collider component for entity collision detection.
 * @author Juan Torres
 * @date 2024
 * @ingroup Component
 */

#ifndef BOXCOLLIDERCOMPONENT_HPP
#define BOXCOLLIDERCOMPONENT_HPP

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <algorithm>

 /**
  * @brief Represents the box collider component for an entity.
  *
  * This component is used to define the dimensions and offset of a
  * rectangular collider attached to an entity. It is useful for collision
  * detection and bounding box calculations.
  */
struct BoxColliderComponent {
    int width;               /**< The width of the collider box. */
    int height;              /**< The height of the collider box. */
    glm::vec2 offset;        /**< The offset of the collider box from the entity's position. */
    std::vector<std::string> excludeCollisionList; /**< List of tags to exclude from collision. */

    /**
     * @brief Constructs a BoxColliderComponent.
     *
     * @param width The width of the collider box (default is 0).
     * @param height The height of the collider box (default is 0).
     * @param offset The offset of the collider box from the entity's position (default is (0, 0)).
     */
    BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0))
        : width(width), height(height), offset(offset) {}

    /**
     * @brief Adds a tag to the exclusion list.
     *
     * @param tag The tag to exclude.
     */
    void AddExclusion(const std::string& tag) {
        if (std::find(excludeCollisionList.begin(), excludeCollisionList.end(), tag) == excludeCollisionList.end()) {
            excludeCollisionList.push_back(tag);
        }
    }

    /**
     * @brief Removes a tag from the exclusion list.
     *
     * @param tag The tag to remove.
     */
    void RemoveExclusion(const std::string& tag) {
        auto it = std::find(excludeCollisionList.begin(), excludeCollisionList.end(), tag);
        if (it != excludeCollisionList.end()) {
            excludeCollisionList.erase(it);
        }
    }

    /**
     * @brief Checks if a tag is in the exclusion list.
     *
     * @param tag The tag to check.
     * @return True if the tag is excluded; otherwise, false.
     */
    bool IsExcluded(const std::string& tag) const {
        return std::find(excludeCollisionList.begin(), excludeCollisionList.end(), tag) != excludeCollisionList.end();
    }
};

#endif // BOXCOLLIDERCOMPONENT_HPP
