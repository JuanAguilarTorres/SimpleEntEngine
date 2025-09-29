/**
* Copyright (C), 2022-2023, Sara Echeverria (bl33h)
* @author Sara Echeverria
* @FileName: faces.h
*/
/**
 * @addtogroup 3D_Tools
 * @{
 * @brief Contains structures and functions for 3D geometry.
 */

#ifndef FACE_H
#define FACE_H
#include <array>
#include <glm/glm.hpp>

/**
 * @brief Represents a triangular face in a 3D model.
 *
 * A face is defined by three indices that correspond to the vertices of the triangle.
 * These indices reference the vertices in the model's vertex array.
 */
struct Face {
    /**
     * @brief The indices of the vertices that make up the face.
     *
     * This array contains three integers, each representing the index of a vertex in the
     * vertex list of the 3D model. These three vertices define a triangular face.
     */
    std::array<int, 3> vertexIndices;
};

#endif