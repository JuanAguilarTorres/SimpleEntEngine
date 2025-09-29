/**
* Copyright (C), 2022-2023, Sara Echeverria (bl33h)
* @author Sara Echeverria
* @FileName: colors.h
*/

#pragma once

#include <algorithm>
#include <iostream>

/**
 * @defgroup 3D_Tools 3D_Tools
 * @{
 * @brief Works managing 3D Attributes.
 *
 * Various 3D Elements Tools.
 */

/**
 * @brief Clamps a value between a minimum and maximum.
 *
 * This function ensures the given value does not exceed the specified range.
 *
 * @tparam T The type of the value, typically numeric.
 * @param value The value to be clamped.
 * @param min The minimum allowable value.
 * @param max The maximum allowable value.
 * @return The clamped value, within the specified range.
 */
template <typename T>
constexpr const T& clamp(const T& value, const T& min, const T& max) {
    return (value < min) ? min : ((value > max) ? max : value);
}

/**
 * @brief Represents a color in RGBA format.
 *
 * A color is represented by four components: red, green, blue, and alpha (transparency).
 * Each component is an unsigned char, with values ranging from 0 to 255.
 */
struct Color {
    unsigned char r; /**< Red component of the color (0-255). */
    unsigned char g; /**< Green component of the color (0-255). */
    unsigned char b; /**< Blue component of the color (0-255). */
    unsigned char a; /**< Alpha component of the color (0-255). */

    /**
     * @brief Constructs a Color object with specified RGBA values.
     *
     * Each color component is clamped between 0 and 255 to ensure valid values.
     *
     * @param red The red component (0-255). Defaults to 0.
     * @param green The green component (0-255). Defaults to 0.
     * @param blue The blue component (0-255). Defaults to 0.
     * @param alpha The alpha component (0-255). Defaults to 255.
     */
    Color(unsigned char red = 0, unsigned char green = 0, unsigned char blue = 0, unsigned char alpha = 255)
        : r(clamp(red, static_cast<unsigned char>(0), static_cast<unsigned char>(255))),
        g(clamp(green, static_cast<unsigned char>(0), static_cast<unsigned char>(255))),
        b(clamp(blue, static_cast<unsigned char>(0), static_cast<unsigned char>(255))),
        a(clamp(alpha, static_cast<unsigned char>(0), static_cast<unsigned char>(255))) {}

    /**
     * @brief Adds two Color objects together, clamping the result.
     *
     * This operation adds the color components of two `Color` objects, and ensures that
     * the resulting components do not exceed 255.
     *
     * @param other The `Color` object to be added.
     * @return A new `Color` object with the summed components, clamped to 255.
     */
    Color operator+(const Color& other) const {
        return Color(
            std::min(static_cast<int>(r) + other.r, 255),
            std::min(static_cast<int>(g) + other.g, 255),
            std::min(static_cast<int>(b) + other.b, 255)
        );
    }

    /**
     * @brief Multiplies the color by a scalar factor.
     *
     * This operation multiplies each color component by a scalar value, clamping the result
     * to ensure the components remain in the 0-255 range.
     *
     * @param factor The scalar factor to multiply the color by.
     * @return A new `Color` object with the multiplied components, clamped to 255.
     */
    Color operator*(float factor) const {
        return Color(
            clamp(static_cast<int>(r * factor), 0, 255),
            clamp(static_cast<int>(g * factor), 0, 255),
            clamp(static_cast<int>(b * factor), 0, 255)
        );
    }

    /**
     * @brief Streams a `Color` object to an output stream in RGBA format.
     *
     * This allows the `Color` object to be printed to an output stream (e.g., `std::cout`).
     *
     * @param os The output stream to write to.
     * @param color The `Color` object to be written.
     * @return The output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const Color& color) {
        os << "RGBA(" << static_cast<int>(color.r) << ", "
            << static_cast<int>(color.g) << ", "
            << static_cast<int>(color.b) << ", "
            << static_cast<int>(color.a) << ")";
        return os;
    }

    /**
     * @brief Multiplies a color by a scalar factor (reverse order).
     *
     * This is the reverse order of multiplication, allowing for the factor to come first.
     *
     * @param factor The scalar factor to multiply the color by.
     * @param color The `Color` object to be multiplied.
     * @return A new `Color` object with the multiplied components, clamped to 255.
     */
    friend Color operator*(float factor, const Color& color) {
        return color * factor;
    }
};

/** @} */ // end of 3D group