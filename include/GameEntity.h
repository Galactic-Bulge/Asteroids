#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

// STD
#include <cstdint>
#include <string>

struct GameEntity
{
private:
    std::uint32_t m_id; // Unique ID for this GameEntity.
    std::string m_tag;	// Tag used to identify entity type

public:

    /*
     * Construct an invalid GameEntity with an ID of 0.
     */
    GameEntity(void) 
        : m_id(0), m_tag("Invalid") 
    { 
        /* Nothing to do */ 
    }

    /*
     * Construct a GameEntity with the provided ID.
     * @param   id      The id for this GameEntity.
     */
    GameEntity(std::uint32_t id, const std::string& t) 
        : m_id(id), m_tag(t) 
    { 
        /* Nothing to do */ 
    }

    /* Getters */
    const std::uint32_t id(void) const { return m_id; }
    const std::string& GetTag(void) const { return m_tag; }

    GameEntity(const GameEntity& rhs) = default;
    GameEntity& operator=(const GameEntity& rhs) = default;
    GameEntity(GameEntity&& rhs) = default;
    GameEntity& operator=(GameEntity&& rhs) = default;
};

#endif
