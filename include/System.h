#ifndef SYSTEM_H
#define SYSTEM_H

// STD
#include <cstdint>
#include <vector>

#include "GameEntity.h"

// Forward Declaration
class EntityManager;

struct SystemBase
{
    static std::uint32_t systemCounter; // Number of unique Systems that have been instantiated.
    static constexpr std::uint32_t MAX_SYSTEMS{50};

    virtual ~SystemBase(void) {}

    /* System interface */
    virtual void Update(EntityManager* pManager, float dt, float tt ) = 0; 
};

template <typename T>
class System : public SystemBase
{
public:
    virtual ~System(void) {}

    static std::uint32_t GetSystemID(void)
    {
       static_assert(std::is_base_of<System,T>::value, "Must be an instance of System!");
       static std::uint32_t system_id = SystemBase::systemCounter++;
       return system_id;
    }
};

#endif
