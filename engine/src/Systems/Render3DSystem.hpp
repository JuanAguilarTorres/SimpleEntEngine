/**
 * @file Render3DSystem.hpp
 * @brief Defines the Render3DSystem responsible for rendering 3D models. 
 * Wireframe logic created by Sara Echeverria, modified by Juan Torres.
 * 3D Model Face rendering by Juan Torres.
 * 
 * Reminder: When loading a 3D model, ensure it is triangulated, as there can 
   be a maximum of three vertices per face (use CTRL+T in Blender to triangulate).
*
 * @author Juan Torres, Based on code by Sara Echeverria
 * @date 2024
 * @ingroup System
 */

#ifndef RENDER3DSYSTEM_HPP
#define RENDER3DSYSTEM_HPP

#include <SDL2/SDL.h>
#include <SDL2_gfx/SDL2_gfxPrimitives.h>  // Include SDL_gfx for polygon drawing
#include <glm/vec3.hpp> // For glm::vec3
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <string>
#include <iostream>
#include <algorithm>

#include "../ECS/ECS.hpp"
#include "../Utils/colors.h"
#include "../Utils/faces.h"
#include "../AssetManager/AssetManager.hpp"
#include "../Components/ObjectComponent.hpp"
#include "../Components/TransformComponent.hpp"

 /**
  * @brief Represents the system that handles 3D Rendering.
  *
  * The Render3DSystem manages 3D Models.
  */
class Render3DSystem : public System {
private:
    std::vector<std::vector<glm::vec3>> VertexArrays;

    // Helper struct to manage face rendering
    struct RenderableFace {
        glm::vec3 v1, v2, v3;
        std::string materialName;
        float averageDepth;
        bool isVisible;

        // Calculate face normal to determine visibility
        bool calculateVisibility() {
            // Calculate two edge vectors
            glm::vec3 edge1 = v2 - v1;
            glm::vec3 edge2 = v3 - v1;

            // Calculate face normal using cross product
            glm::vec3 normal = glm::cross(edge1, edge2);

            // Simple view direction (For Backface Culling)
            glm::vec3 viewDirection(0, 0, -1);

            // Face is visible if normal points towards the camera (dot product > 0)
            return glm::dot(normal, viewDirection) > 0;
        }

        // Calculate average depth for sorting
        void computeAverageDepth() {
            averageDepth = (v1.z + v2.z + v3.z) / 3.0f;
        }
    };

    // Depth sorting function
    static bool compareFaceDepth(const RenderableFace& a, const RenderableFace& b) {
        // Sort from back to front (furthest to nearest)
        return a.averageDepth < b.averageDepth;
    }

public:

    /**
     * @brief Constructs a Render3DSystem.
     *
     * This constructor specifies what entities using this system must have*/
    Render3DSystem() {
        RequireComponent<ObjectComponent>();
        RequireComponent<TransformComponent>();
    }

    /**
     * @brief Draws a line between two 3D points using the SDL renderer.
     *
     * @param renderer Pointer to the SDL renderer used for drawing.
     * @param start The starting point of the line in 3D space.
     * @param end The ending point of the line in 3D space.
     */
 void linesDrawing(SDL_Renderer* renderer, const glm::vec3& start, const glm::vec3& end)   
    {
        SDL_RenderDrawLine(renderer, static_cast<int>(start.x), static_cast<int>(start.y),
            static_cast<int>(end.x), static_cast<int>(end.y));
    }

    /**
     * @brief Draws a triangle using three 3D vertices with the SDL renderer.
     *
     * @param renderer Pointer to the SDL renderer used for drawing.
     * @param v1 The first vertex of the triangle in 3D space.
     * @param v2 The second vertex of the triangle in 3D space.
     * @param v3 The third vertex of the triangle in 3D space.
     */    
    void trianglesDrawing(SDL_Renderer* renderer, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
    {
        linesDrawing(renderer, v1, v2);
        linesDrawing(renderer, v2, v3);
        linesDrawing(renderer, v3, v1);
    }

    /**
     * @brief Draws a wireframe representation of a 3D object using the provided vertices and transformation.
     *
     * @param renderer Pointer to the SDL renderer used for drawing.
     * @param vertex A vector of pairs, where each pair consists of a 3D vertex and an optional label.
     * @param transformC The transformation component to apply to the vertices (e.g., translation, rotation, scaling).
     */

    void drawWireframe(SDL_Renderer* renderer,
        const std::vector<std::pair<glm::vec3, std::string>>& vertex,
         const TransformComponent transformC)
    {
        // Iterate through vertices and draw triangles using lines
        for (size_t i = 0; i < vertex.size(); i += 3) {
            if (i + 2 < vertex.size()) {
                // Extract vertices (first element of each pair)
                glm::vec3 v1 = vertex[i].first;
                glm::vec3 v2 = vertex[i + 1].first;
                glm::vec3 v3 = vertex[i + 2].first;

                // Window Size
                int offsetX = transformC.position.x;
                int offsetY = transformC.position.y;

                // Set the renderer color
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

                trianglesDrawing(renderer,
                    v1 + glm::vec3(offsetX, offsetY, 0),
                    v2 + glm::vec3(offsetX, offsetY, 0),
                    v3 + glm::vec3(offsetX, offsetY, 0));
            }
        }
    }

    /**
     * @brief Renders a 3D model using the SDL renderer.
     *
     * Combines vertex data, object properties, and transformations to draw
     * the 3D model represented by the provided OBJ asset.
     *
     * @param renderer Pointer to the SDL renderer used for drawing.
     * @param vertex A vector of 3D vertices and optional labels for the model.
     * @param objAsset The OBJ asset containing model data such as faces and materials.
     * @param objectC The object component containing additional model properties.
     * @param transformC The transformation component to apply to the model (e.g., translation, rotation, scaling).
     */
    void drawModel(SDL_Renderer* renderer,
        const std::vector<std::pair<glm::vec3, std::string>>& vertex,
        const AssetManager::ObjAsset& objAsset,
        const ObjectComponent objectC, const TransformComponent transformC)
    {
        std::vector<RenderableFace> renderFaces;
        for (size_t i = 0; i < vertex.size(); i += 3) {
            if (i + 2 < vertex.size()) {
                RenderableFace face;
                face.v1 = vertex[i].first;
                face.v2 = vertex[i + 1].first;
                face.v3 = vertex[i + 2].first;
                face.computeAverageDepth();
                face.isVisible = face.calculateVisibility();

                // Store the material name for this face
                face.materialName = vertex[i].second; // Assuming material is consistent for the face

                renderFaces.push_back(face);
            }
        }

        std::sort(renderFaces.begin(), renderFaces.end(), compareFaceDepth);

        int offsetX = transformC.position.x;
        int offsetY = transformC.position.y;
        glm::vec3 lightDirection(0.0f, 0.0f, -1.0f);

        auto calculateShading = [](const glm::vec3& normal, const glm::vec3& lightDir) -> float {
            float intensity = glm::dot(normal, glm::normalize(lightDir));
            return glm::clamp(intensity, 0.5f, 1.0f);
            };

        for (const auto& face : renderFaces) {
            if (!face.isVisible) continue;

            glm::vec3 edge1 = face.v2 - face.v1;
            glm::vec3 edge2 = face.v3 - face.v1;
            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

            float shadingIntensity = calculateShading(normal, lightDirection);

            // Use the material name stored in the face
            glm::vec3 baseColor = getMaterialColor(objAsset, face.materialName);

            // Blend shadow color with material color based on shading intensity
            glm::vec3 shadowColor = glm::vec3(objectC.sr, objectC.sg, objectC.sb);
            glm::vec3 shadedColor = baseColor * shadingIntensity + shadowColor * (1.0f - shadingIntensity);

            // Convert the shaded color to 8-bit per channel
            uint8_t shadedRed = static_cast<uint8_t>(shadedColor.r * 255);
            uint8_t shadedGreen = static_cast<uint8_t>(shadedColor.g * 255);
            uint8_t shadedBlue = static_cast<uint8_t>(shadedColor.b * 255);

            SDL_Point points[3];
            points[0] = { static_cast<int>(face.v1.x + offsetX), static_cast<int>(face.v1.y + offsetY) };
            points[1] = { static_cast<int>(face.v2.x + offsetX), static_cast<int>(face.v2.y + offsetY) };
            points[2] = { static_cast<int>(face.v3.x + offsetX), static_cast<int>(face.v3.y + offsetY) };

            filledTrigonRGBA(renderer,
                points[0].x, points[0].y,
                points[1].x, points[1].y,
                points[2].x, points[2].y,
                shadedRed, shadedGreen, shadedBlue, 255);
        }
    }


    /**
     * @brief Sets up a vertex array from 3D vertices and face definitions.
     *
     * Converts the vertices and faces of an OBJ model into a format suitable for rendering.
     *
     * @param vertex A vector of 3D vertex positions.
     * @param faces A vector of face definitions from the OBJ asset.
     * @return A vector of 3D vertices paired with optional labels, ready for rendering.
     */    
    std::vector<std::pair<glm::vec3, std::string>> setupVertexArray(
        const std::vector<glm::vec3>& vertex,
        const std::vector<AssetManager::Face>& faces)
    {
        // Scale factor for vertices
        float scale = 40.0f;
        // Initialize vertex array with material information
        std::vector<std::pair<glm::vec3, std::string>> vertexArray;

        // Process object's faces
        for (const auto& face : faces)
        {
            // Get vertex positions and scale
            glm::vec3 vertexPosition1 = vertex[face.vertexIndices[0]];
            glm::vec3 vertexPosition2 = vertex[face.vertexIndices[1]];
            glm::vec3 vertexPosition3 = vertex[face.vertexIndices[2]];

            glm::vec3 vertexScaled1 = glm::vec3(vertexPosition1.x * scale, -vertexPosition1.y * scale, vertexPosition1.z * scale);
            glm::vec3 vertexScaled2 = glm::vec3(vertexPosition2.x * scale, -vertexPosition2.y * scale, vertexPosition2.z * scale);
            glm::vec3 vertexScaled3 = glm::vec3(vertexPosition3.x * scale, -vertexPosition3.y * scale, vertexPosition3.z * scale);

            // Add scaled vertices with their corresponding material name
            vertexArray.push_back({ vertexScaled1, face.materialName });
            vertexArray.push_back({ vertexScaled2, face.materialName });
            vertexArray.push_back({ vertexScaled3, face.materialName });
        }
        return vertexArray;
    }

    /**
     * @brief Rotates a 3D model around the X and Y axes.
     *
     * Applies rotation transformations to the vertices of a 3D model.
     *
     * @param baseModel The base model represented as a vector of 3D vertices and optional labels.
     * @param angleX The angle (in degrees) to rotate the model around the X-axis.
     * @param angleY The angle (in degrees) to rotate the model around the Y-axis.
     * @return A vector of transformed 3D vertices paired with optional labels.
     */
    std::vector<std::pair<glm::vec3, std::string>> rotateModel(
        const std::vector<std::pair<glm::vec3, std::string>>& baseModel,
        float angleX,
        float angleY)
    {
        // Create a rotation matrix for the X-axis (vertical rotation)
        glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), angleX, glm::vec3(1.0f, 0.0f, 0.0f));
        // Create a rotation matrix for the Y-axis (horizontal rotation)
        glm::mat4 rotationMatrixY = glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0.0f, 1.0f, 0.0f));
        // Combine the rotation matrices (Y * X for proper order)
        glm::mat4 combinedRotationMatrix = rotationMatrixY * rotationMatrixX;

        // Apply the combined rotation matrix to each vertex
        std::vector<std::pair<glm::vec3, std::string>> rotatedVertexArray;
        for (const auto& [vertex, materialName] : baseModel) {
            glm::vec4 rotatedVertex = combinedRotationMatrix * glm::vec4(vertex, 1.0f);
            rotatedVertexArray.push_back({
                glm::vec3(rotatedVertex),
                materialName
                });
        }
        return rotatedVertexArray;
    }

    /**
     * @brief Retrieves the color associated with a specific material in an OBJ asset.
     *
     * This function searches for the material within the provided OBJ asset
     * and returns its color.
     *
     * @param objAsset The OBJ asset containing material data.
     * @param materialName The name of the material whose color is being requested.
     * @return The color of the specified material as a `glm::vec3`.
     */
    glm::vec3 getMaterialColor(const AssetManager::ObjAsset& objAsset, const std::string& materialName) {
        auto it = objAsset.Mtl.find(materialName);
        if (it != objAsset.Mtl.end()) {
            // Return diffuse color or a default color
            return glm::vec3(it->second.Kd.r, it->second.Kd.g, it->second.Kd.b);
        }
        // Default color if material not found
        return glm::vec3(1.0f, 1.0f, 1.0f);
    }

    /**
     * @brief Updates the game state and renders the scene using the provided SDL renderer and asset manager.
     *
     * This method handles the game update cycle, including drawing and updating objects.
     *
     * @param renderer Pointer to the SDL renderer used for drawing.
     * @param assetManager A unique pointer to the asset manager that manages the game's assets.
     */
    void Update(SDL_Renderer* renderer, const std::unique_ptr<AssetManager>& assetManager) {
        for (auto entity : GetSystemEntities()) {
            const auto objectComponent = entity.GetComponent<ObjectComponent>();
            const auto transformComponent = entity.GetComponent<TransformComponent>();
            const auto objectAsset = assetManager->Get3dObject(objectComponent.assetId);

            std::vector<std::pair<glm::vec3, std::string>>
                baseModel = setupVertexArray(objectAsset.Vertex, objectAsset.Faces);
            std::vector<std::pair<glm::vec3, std::string>>
                rotatedModel = rotateModel(baseModel,
                    objectComponent.xRot, objectComponent.yRot);

            // Pass the entire objectAsset to draw with material colors
            drawModel(renderer, rotatedModel, objectAsset, objectComponent, transformComponent);
        }
    }

    /**
     * @brief Updates and renders the wireframe representation of the model using the provided SDL renderer and asset manager.
     *
     * This method handles updating and rendering the wireframe of the model, typically for debugging or visualizing structure.
     *
     * @param renderer Pointer to the SDL renderer used for drawing.
     * @param assetManager A unique pointer to the asset manager that manages the game's assets.
     */
    void UpdateWireframe(SDL_Renderer* renderer, const std::unique_ptr<AssetManager>& assetManager) {
        for (auto entity : GetSystemEntities()) {
            const auto objectComponent = entity.GetComponent<ObjectComponent>();
            const auto transformComponent = entity.GetComponent<TransformComponent>();
            const auto objectAsset = assetManager->Get3dObject(objectComponent.assetId);

            std::vector<std::pair<glm::vec3, std::string>> 
                baseModel = setupVertexArray(objectAsset.Vertex, objectAsset.Faces);
            std::vector<std::pair<glm::vec3, std::string>>
                rotatedModel = rotateModel(baseModel,
                objectComponent.xRot, objectComponent.yRot);

            drawWireframe(renderer, rotatedModel, transformComponent);
        }
    }
};

#endif // RENDER3DSYSTEM_HPP
