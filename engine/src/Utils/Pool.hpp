/**
 * @file Pool.hpp
 * @brief Component pool implementation for entity component system
 * @author Juan Torres
 * @date 2024
 */

#ifndef POOL_HPP
#define POOL_HPP

#include <vector>

 /**
  * @defgroup Pool Pool
  * @{
  * @brief Works managing pools of components in an entity-component system
  *
  * The Pool system provides efficient storage and access to components
  * in an entity component system architecture. It manages dynamic arrays
  * of components with type safety and convenient access methods.
  */

  /**
   * @brief Interface class for component pools
   *
   * Base interface class that defines the common behavior for all component pools.
   * Provides a virtual destructor for proper cleanup of derived classes.
   */
class IPool {
public:
    virtual ~IPool() = default;
};

/**
 * @brief Template class for storing and managing components of type TComponent
 *
 * @tparam TComponent The type of component to be stored in the pool
 *
 * This class provides a type-safe container for storing and accessing components.
 * It manages a dynamic array of components with methods for adding, accessing,
 * and modifying elements.
 */
template <typename TComponent>
class Pool : public IPool {
private:
    std::vector<TComponent> data; /**< Internal storage for components */

public:
    /**
     * @brief Construct a new Pool object
     * @param size Initial size of the pool (default: 1000)
     */
    Pool(int size = 1000) {
        data.resize(size);
    }

    virtual ~Pool() = default;

    /**
     * @brief Check if the pool is empty
     * @return true if pool contains no elements
     * @return false if pool contains elements
     */
    bool IsEmpty() const {
        return data.empty();
    }

    /**
     * @brief Get the current size of the pool
     * @return int Number of elements in the pool
     */
    int GetSize() const {
        return static_cast<int>(data.size());
    }

    /**
     * @brief Resize the pool to contain n elements
     * @param n New size of the pool
     */
    void Resize(int n) {
        data.resize(n);
    }

    /**
     * @brief Remove all elements from the pool
     */
    void Clear() {
        data.clear();
    }

    /**
     * @brief Add a new component to the pool
     * @param object Component to add
     */
    void Add(TComponent object) {
        data.push_back(object);
    }

    /**
     * @brief Set the component at the specified index
     * @param index Index where to set the component
     * @param object Component to set at the index
     */
    void Set(unsigned int index, TComponent object) {
        data[index] = object;
    }

    /**
     * @brief Get the component at the specified index
     * @param index Index of the component to retrieve
     * @return TComponent& Reference to the component
     */
    TComponent& Get(unsigned int index) {
        return static_cast<TComponent&>(data[index]);
    }

    /**
     * @brief Array subscript operator for accessing components
     * @param index Index of the component to access
     * @return TComponent& Reference to the component
     */
    TComponent& operator[] (unsigned int index) {
        return static_cast<TComponent&>(data[index]);
    }
};

/** @} */ // end of ComponentPool group

#endif //POOL_HPP