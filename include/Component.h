#ifndef COMPONENT_H
#define COMPONENT_H

// STD
#include <type_traits>
#include <cstdint>

struct ComponentBase
{
    static std::uint32_t componentCounter;                // Number of component types that have been instantiated.
    static constexpr std::uint32_t MAX_COMPONENTS{50};    // Constant indicating the max # of components that can be attached to a GameEntity.

    virtual ~ComponentBase(void){}
};

template <typename T>
struct Component : public ComponentBase
{
    virtual ~Component(void){}

    /*
     * Gets the unique ID # of the component T.
     * @return  The ID # associated with components of class T.
     */
    static std::uint32_t GetComponentID(void)
    {
        static_assert(std::is_base_of<Component, T>::value, "Must be an instance of Component!");
        static std::uint32_t id = ComponentBase::componentCounter++; // This will only be incremented the first time.
        return id;
    }
};

#endif
