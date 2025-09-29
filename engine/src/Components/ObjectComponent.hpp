/**
 * @file ObjectComponent.hpp
 * @brief Defines the ObjectComponent used to manage 3D Instances.
 * @author Juan Torres
 * @date 2024
 * @ingroup Component
 */

#ifndef OBJECTCOMPONENT_HPP
#define OBJECTCOMPONENT_HPP

#include <string>
#include <SDL2/SDL.h>

 /**
  * @brief Represents a component that handles 3D Objects for an entity.
  *
  * This component manages properties related 3D OBJ Files.
  */
struct ObjectComponent {
    std::string assetId; /**< The ID of the object to be show. */
    double xRot; /**< The x Rotation. */
    double yRot; /**< The y Rotation. */
    // shadow colors
    float sr;
    float sg;
    float sb;

    /**
     * @brief Constructs a ObjectComponent with specified 3D properties.
     *
     * @param assetId The ID of the video (default is "none").
     * @param xRot X rotation (default is 0).
     * @param yRot Y Rotation (default is 0).
     * @param sr Shadow Red color (default is 0.2f).
     * @param sg Shadow green color (default is 0.2f).
     * @param sb Shadow blue color (default is 0.2f).
     */
    ObjectComponent(const std::string& assetId = "none", 
        double xRot = 0, double yRot = 0, 
        float sr = 0.2f, float sg = 0.2f, float sb = 0.2f) {
        this->assetId = assetId;
        this->xRot = xRot;
        this->yRot = yRot;
        this->sr = sr;
        this->sg = sg;
        this->sb = sb;
    }
};

#endif // OBJECTCOMPONENT_HPP
