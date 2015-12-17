#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Singleton.h"
#include "GameEntity.h"
#include "Component.h"
#include "System.h"

// STD
#include <cstdint>
#include <vector>
#include <bitset>
#include <iostream>
#include <algorithm>
#include <utility>
#include <iterator>
#include <type_traits>

// Collisions
#include <Octant.h>

using ComponentMask = std::bitset<ComponentBase::MAX_COMPONENTS>;   // Bitset used to determine which Components are attached to a GameEntity, indexed via Component ID.
using ComponentPool = std::vector<ComponentBase*>;                  // Pool of Component<T> for a particular T, indexed via GameEntity ID.

class EntityManager : public Singleton<EntityManager>
{
private:
    constexpr static std::uint32_t ENTITY_COUNT = 50;               // Increment in which GameEntities are allocated.
    std::uint32_t               m_highestID{1};                     // Current max GameEntity ID generated.
    std::vector<GameEntity>     m_entities;                         // Vector containing all GameEntities, indexed via GameEntity ID
    std::vector<GameEntity>     m_markedForDestruction;             // Vector containing all GameEntities to destroy this frame.
    std::vector<std::uint32_t>  m_entityIDs;                        // Vector containing all available GameEntity IDs.
    std::vector<ComponentMask>  m_entityMasks;                      // Vector containing the ComponentMask for each GameEntity, indexed via GameEntity ID.
    std::vector<ComponentPool>  m_entityComponents;                 // Vector containing the ComponentPools for each Component<T>, indexed via Component ID.
	std::vector<Octant>			m_octants;							// Vector containing all of the Octants present in the current frames partitioned space

    // Simple object used to sort Systems by their priority.
    struct SystemPriority
    {
        std::uint32_t index;                                        // Index into m_systemPool, equivalent to System ID.
        std::uint32_t priority;                                     // Priority #.

        SystemPriority(std::uint32_t index, std::uint32_t priority)
            : index(index), priority(priority)
        { /* Nothing to do */ }
    };
    std::vector<SystemBase*>                m_systemPool;           // Vector containing all Systems, indexed via System ID.
    std::bitset<SystemBase::MAX_SYSTEMS>    m_attachedSystems;      // Bitset indicating which Systems are active, indexed via System ID.
    std::vector<SystemPriority>             m_systemPriorities;     // Vector containing the priority and index of each active System.

    /*
     * This method allocates an additional ENTITY_COUNT GameEntities once the
     * current cap has been reached.
     */
    void Resize(void)
    {
        // Generate ENTITY_COUNT additional GameEntities
        std::fill_n(std::back_inserter(m_entities), ENTITY_COUNT, GameEntity());

        // Generate ENTITY_COUNT additional IDs for GameEntities
        std::generate_n(std::back_inserter(m_entityIDs), ENTITY_COUNT, [this](void){ return this->m_highestID++; });

        // Generate ENTITY_COUNT additional ComponentMasks
        std::fill_n(std::back_inserter(m_entityMasks), ENTITY_COUNT, ComponentMask());

        // Generate ENTITY_COUNT additional ComponentPools
        for(auto& pool : m_entityComponents)
        {
            std::fill_n(std::back_inserter(pool), ENTITY_COUNT, nullptr);
        }
    }

public:

    /*
     * Construct an instance of the EntityManager.
     *
     * The EntityManager is constructed with an initial allocation of 50
     * GameEntities, and allocates an additional 50 every time the cap is
     * reached.
     */
    EntityManager(void)
    {
        // Generate ENTITY_COUNT initial GameEntities
        m_entities = std::vector<GameEntity>(ENTITY_COUNT, GameEntity());

        // Generate ENTITY_COUNT initial slots for GameEntities
        std::generate_n(std::back_inserter(m_entityIDs), ENTITY_COUNT, [this](void){ return this->m_highestID++; });

        // Generate ENTITY_COUNT initial GameEntity masks
        m_entityMasks = std::vector<ComponentMask>(ENTITY_COUNT, ComponentMask());

        // Generate ENTITY_COUNT initial GameEntity component pools
        m_entityComponents = std::vector<ComponentPool>(ComponentBase::MAX_COMPONENTS, ComponentPool(ENTITY_COUNT, nullptr));

        // Setup the System pool
        m_systemPool = std::vector<SystemBase*>(SystemBase::MAX_SYSTEMS, nullptr);
    }

    /*
     * Destruct this EntityManager instance.
     *
     * This method is resonsible for cleaning up any allocated Components and
     * Systems.
     */
    ~EntityManager(void)
    {
        // Cleanup created Components
        for(auto& pool : m_entityComponents)
        {
            for(auto& component : pool)
            {
                if(component)
                {
                    delete component;
                }
            }
        }

        // Cleanup created Systems
        for(auto& system : m_systemPool)
        {
            if(system)
            {
                delete system;
            }
        }
    }

#pragma region Entity Functions

    /*
     * This method returns all the GameEntities managed by this EntityManager.
     * @return A std::vector containing every GameEntity.
     */
    const std::vector<GameEntity>& Entities(void)
    {
        return m_entities;
    }

    /*
     * This method destroys all GameEntities and Components currently registered
     * to this EntityManager.
     */
    void Clear(void)
    {
        // Cleanup allocated components
        for(auto& pool : m_entityComponents)
        {
            for(auto& component : pool)
            {
                if(component)
                {
                    delete component;
                }
            }
        }

        // Reset all std containers.
        m_highestID = 1;
        m_entities.clear();
        m_markedForDestruction.clear();
        m_entityIDs.clear();
        m_entityMasks.clear();

        m_entityComponents.clear();
        m_entityComponents.resize(ComponentBase::MAX_COMPONENTS);

        // Reset to ENTITY_COUNT GameEntities.
        Resize();
    }

    /*
     * Destroy all currently attached systems.
     */
    void ClearSystems(void)
    {
        for(auto& system : m_systemPool)
        {
            if(system)
            {
                delete system;
            }
        }
        m_systemPool.clear();
        m_systemPool.resize(SystemBase::MAX_SYSTEMS);
        m_systemPriorities.clear();
        m_attachedSystems.reset();
    }

    /*
     * This method allocates a new GameEntity.
     * @return A reference to the newly allocated GameEntity.
     */
    GameEntity Create(char* t)
    {
        // If there are no unused IDs, create more
        if(m_entityIDs.empty())
        {
            Resize();
        }

        // Retrieve an ID for this GameEntity.
        std::uint32_t entity_id = m_entityIDs.back();

        // Remove the ID from the pool of available IDs.
        m_entityIDs.pop_back();

        // Construct a GameEntity at the indicated ID.
        m_entities[entity_id - 1] = GameEntity(entity_id, t);

        return GameEntity(entity_id, t);
    }

    /*
     * This method destroys the provided GameEntity.
     * @return A boolean indicating if this operation was successful.
     */
    bool Destroy(GameEntity e)
    {
        std::uint32_t entity_id = e.id();

        // Check that the ID is valid.
        if(entity_id == 0 || entity_id > m_highestID)
        {
            return false;
        }

        m_markedForDestruction.push_back(e);

        return true;
    }

#pragma endregion

#pragma region Component Functions

    /*
     * This function attaches a Component of type T to the provided GameEntity.
     * @param   e       The GameEntity to attach this Component to.
     * @param   args    The arguments forward to the constructor for T.
     * @return  A pointer to the newly constructed T, or the existing T.
     */
    template<typename T, typename... Args>
    T* AddComponent(GameEntity e, Args&&... args)
    {
        static_assert(std::is_base_of<Component<T>, T>::value, "Must be an instance of Component!");

        const std::uint32_t entity_id = e.id();
        const std::uint32_t component_id = Component<T>::GetComponentID();

        // Test that we have a valid ID
        if(entity_id == 0 || entity_id > m_highestID)
        {
            return nullptr;
        }

        // Test if we have reached MAX_COMPONENTS
        if(component_id >= ComponentBase::MAX_COMPONENTS)
        {
            return nullptr;
        }

        // Test if a component of type T is already attached.
        if(m_entityMasks[entity_id - 1].test(component_id))
        {
            return static_cast<T*>(m_entityComponents[component_id][entity_id - 1]);
        }

        // Create the Component of type T.
        ComponentBase* c = new T(std::forward<Args>(args)...);
        m_entityComponents[component_id][entity_id - 1] = c;

        // Set the mask for this Component type.
        m_entityMasks[entity_id - 1].set(component_id);

        return static_cast<T*>(c);
    }

    /*
     * Remove a Component of type T from the provided GameEntity.
     * @param   e       The GameEntity to remove the Component from.
     * @return A boolean indicating if this operation was successful.
     */
    template <typename T>
    bool RemoveComponent(GameEntity e)
    {
        static_assert(std::is_base_of<Component<T>, T>::value, "Must be an instance of Component!");

        const std::uint32_t entity_id = e.id();
        const std::uint32_t component_id = Component<T>::GetComponentID();

        // Test that we have a valid ID.
        if(entity_id == 0 || entity_id > m_highestID)
        {
            return false;
        }

        // Test if we have reached MAX_COMPONENTS.
        if(component_id >= ComponentBase::MAX_COMPONENTS)
        {
            return false;
        }

        // Test if a component of type T is attached.
        if(!m_entityMasks[entity_id - 1].test(component_id))
        {
            return false;
        }

        // Delete the Component.
        delete static_cast<T*>(m_entityComponents[component_id][entity_id - 1]);

        // Set the mask to false.
        m_entityMasks[entity_id - 1].set(component_id, false);

        // Null the entry in the pool.
        m_entityComponents[component_id][entity_id - 1] = nullptr;

        return true;
    }

    /*
     * Test if there is a Component of type T attached to the provided
     * GameEntity.
     * @param   e       The GameEntity to test.
     * @return A boolean indicating if a Component of type T is attached.
     */
    template <typename T>
    bool HasComponent(GameEntity e)
    {
        static_assert(std::is_base_of<Component<T>, T>::value, "Must be an instance of Component!");

        const std::uint32_t entity_id = e.id();
        const std::uint32_t component_id = Component<T>::GetComponentID();

        // Test that we have a valid ID.
        if(entity_id == 0 || entity_id > m_highestID)
        {
            return false;
        }

        // Test if we have reached MAX_COMPONENTS
        if(component_id >= ComponentBase::MAX_COMPONENTS)
        {
            return false;
        }

        return m_entityMasks[entity_id - 1][component_id];
    }

    /*
     * This function gets a Component of type T attached to the provided
     * GameEntity.
     * @param   e       The GameEntity to retrieve the Component from.
     * @return A pointer to the Component of type T attached to the GameEntity.
     */
    template <typename T>
    T* GetComponent(GameEntity e)
    {
        static_assert(std::is_base_of<Component<T>, T>::value, "Must be an instance of Component!");

        const std::uint32_t entity_id = e.id();
        const std::uint32_t component_id = Component<T>::GetComponentID();

        // Test that we have a valid ID.
        if(entity_id == 0 || entity_id > m_highestID)
        {
            return nullptr;
        }

        // Test if we have reached MAX_COMPONENTS.
        if(component_id >= ComponentBase::MAX_COMPONENTS)
        {
            return nullptr;
        }

        // Test if this GameEntity has a component of type T attached.
        if(!m_entityMasks[entity_id - 1].test(component_id))
        {
			return nullptr;
        }

        // Return the component.
        return static_cast<T*>(m_entityComponents[component_id][entity_id - 1]);
    }

    template <typename T>
    std::vector<T*> GetComponents(void)
    {
        static_assert(std::is_base_of<Component<T>,T>::value, "Must be an instance of Component!");

        const std::uint32_t component_id = Component<T>::GetComponentID();

        std::vector<T*> components;
        if(component_id >= ComponentBase::MAX_COMPONENTS || component_id >= m_entityComponents.size())
        {
            return components;
        }

        ComponentPool pool = m_entityComponents[component_id];
        for(auto& component : pool)
        {
            if(component)
            {
                components.push_back(static_cast<T*>(component));
            }
        }

        return components;
    }

    /*
     * This function returns the ComponentMask corresponding to the Component of type T.
     * @return A ComponentMask where the bit corresponding to T is set.
     */
    template <typename T>
    static ComponentMask GetComponentMask(void)
    {
        static_assert(std::is_base_of<Component<T>, T>::value, "Must be an instance of Component!");

        const std::uint32_t component_id = Component<T>::GetComponentID();

        ComponentMask mask;
        mask.set(component_id);

        return mask;
    }

    /*
     * This function returns a ComponentMask with the bits corresponding to the
     * provided Components set.
     * @return A ComponentMask where the bits corresponding to T1, T2, and Args... are set.
     */
    template <typename T1, typename T2, typename... Args>
    static ComponentMask GetComponentMask(void)
    {
        static_assert(std::is_base_of<Component<T1>, T1>::value, "Must be an instance of Component!");
        return GetComponentMask<T1>() | GetComponentMask<T2, Args...>();
    }

    /*
     * This function returns all the GameEntities with the provided Components attached.
     * @return A std::vector containing the GameEntities with Args... Components attached.
     */
    template <typename... Args>
    std::vector<GameEntity> EntitiesWithComponents(void)
    {
        // Generate a ComponentMask based on the template arguments.
        ComponentMask mask = GetComponentMask<Args...>();

        // Copy all GameEntities with the matching ComponentMask.
        std::vector<GameEntity> entities;
        std::copy_if
        (
            m_entities.begin(),
            m_entities.end(),
            std::back_inserter(entities),
            // Test that the GameEntity has a valid ID, and then compare ComponentMasks
            [this,&mask](auto& entity) -> bool { return entity.id() != 0 && ( mask == ( this->m_entityMasks[entity.id() - 1] & mask ) ); }
        );

		return entities;
    }
#pragma endregion

#pragma region System Functions

    /*
     * Register a System of type T with the EntityManager.
     * @return A boolean indicating if this operation was successful.
     */
    template <typename T>
    bool AddSystem(void)
    {
        static_assert(std::is_base_of<System<T>,T>::value, "Must be an instance of System!");

        const std::uint32_t system_id = System<T>::GetSystemID();
        const std::uint32_t system_priority = system_id;            // Priority defaults to System ID.

        // Test if we have attached max # of Systems
        if(system_id >= SystemBase::MAX_SYSTEMS)
        {
            return false;
        }

        // Test if we already have a System of this type attached.
        if(m_attachedSystems.test(system_id))
        {
            return false;
        }

        // Create the System
        T* system = new T();

        // Set the bit in the SystemMask.
        m_attachedSystems.set(system_id);

        // Store the System in the SystemPool.
        m_systemPool[system_id] = system;

        // Store the index and priority.
        m_systemPriorities.emplace_back(system_id, system_priority);

        return true;
    }

    /*
     * Register a System of type T with the EntityManager, with priority P.
     * @return A boolean indicating if this operation was successful.
     */
    template <typename T, std::uint32_t P>
    bool AddSystemWithPriority(void)
    {
        static_assert(std::is_base_of<System<T>,T>::value, "Must be an instance of System!");

        const std::uint32_t system_id = System<T>::GetSystemID();
        const std::uint32_t system_priority = P;

        // Test if we have attached max # of Systems.
        if(system_id >= SystemBase::MAX_SYSTEMS)
        {
            return false;
        }

        // Test if we already have a System of this type attached.
        if(m_attachedSystems.test(system_id))
        {
            return false;
        }

        // Create the System.
        T* system = new T();

        // Set the SystemMask.
        m_attachedSystems.set(system_id);

        // Store the System in SystemPool.
        m_systemPool[system_id] = system;

        // Store the index and priority.
        m_systemPriorities.emplace_back(system_id, system_priority);

        return true;
    }

    /*
     * Remove a System of type T from this EntityManager.
     * @return A boolean indicating if this operation was successful.
     */
    template <typename T>
    bool RemoveSystem(void)
    {
        static_assert(std::is_base_of<System<T>,T>::value, "Must be an instance of System!");

        const std::uint32_t system_id = System<T>::GetSystemID();

        // Test if we have attached max # of Systems
        if(system_id >= SystemBase::MAX_SYSTEMS)
        {
            return false;
        }

        // Test if we even have a System of type T attached.
        if(!m_attachedSystems.test(system_id))
        {
            return false;
        }

        // Delete the System
        delete static_cast<T*>(m_systemPool[system_id]);

        // Null out the location in the SystemPool.
        m_systemPool[system_id] = nullptr;

        // Reset the SystemMask.
        m_attachedSystems.set(system_id, false);

        // Remove the SystemPriority
        m_systemPriorities.erase
        (
            std::remove_if
            (
                m_systemPriorities.begin(),
                m_systemPriorities.end(),
                [&system_id](auto& obj) -> bool { return obj.index == system_id; }
            ),
            m_systemPriorities.end()
        );

        return true;
    }

    /*
     * Update all Systems registered to this EntityManager.
     * @param   dt      The delta time value this frame.
     * @param   tt      The total time value this frame.
     */
    void Update(float dt, float tt)
    {
		// Clear old Octants
		m_octants.clear();

        // Sort based on System priority.
        std::stable_sort
        (
            m_systemPriorities.begin(),
            m_systemPriorities.end(),
            [](auto& lhs, auto& rhs) -> bool { return lhs.priority < rhs.priority; }
        );

        // Update Systems in order of sorted priority.
        for(auto& priorityObj : m_systemPriorities)
        {
            std::uint32_t system_index = priorityObj.index;
            SystemBase* pSystem = m_systemPool[system_index];
            if(pSystem)
            {
                pSystem->Update(this, dt, tt);
            }
        }

        // Destroy all GameEntities marked for destruction this frame.
        for(auto& entity : m_markedForDestruction)
        {
            std::uint32_t entity_id = entity.id();

            // Clear the mask
            m_entityMasks[entity_id - 1].reset();

            // Delete all Components belonging to this GameEntity
            for(auto& pool : m_entityComponents)
            {
                if(pool[entity_id - 1])
                {
                    delete pool[entity_id - 1];
                    pool[entity_id - 1] = nullptr;
                }
            }

            // Re-add the ID to pool
            m_entityIDs.push_back(entity_id);
        }

        // Reset the vector of GameEntities marked for destruction.
        m_markedForDestruction.clear();
    }
#pragma endregion


	std::vector<Octant> Octants() {
		return m_octants;
	}

	void AddOctant(Octant o) {
		m_octants.push_back(o);
	}
};
#endif
