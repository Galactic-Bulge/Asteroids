#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

struct GameEntity
{
private:
    std::uint32_t m_id; // Unique ID for this GameEntity.

public:

    /*
     * Construct an invalid GameEntity with an ID of 0.
     */
    GameEntity(void) : m_id(0) { /* Nothing to do */ }

    /*
     * Construct a GameEntity with the provided ID.
     * @param   id      The id for this GameEntity.
     */
    explicit GameEntity(std::uint32_t id) : m_id(id) { /* Nothing to do */ }

    /* Getters */
    std::uint32_t id(void) const { return m_id; }

    GameEntity(const GameEntity& rhs) = default;
    GameEntity& operator=(const GameEntity& rhs) = default;
    GameEntity(GameEntity&& rhs) = default;
    GameEntity& operator=(GameEntity&& rhs) = default;
};

#endif
