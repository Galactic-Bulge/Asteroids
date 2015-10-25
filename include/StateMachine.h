#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// STD
#include <map>
#include <type_traits>
#include <utility>

// IGameState interface
#include "IGameState.h"

template <typename T>
class StateMachine
{
private:
    IGameState* m_pCurrState;
    std::map<T, IGameState*> m_states;

    // Private to prevent instantiation outside instance method.
    StateMachine::StateMachine( void ) : m_pCurrState( nullptr )
    {
        static_assert( std::is_enum<T>::value, "Must be an Enum type!" );
    }

    ~StateMachine( void )
    {
        // Cleanup IGameStates
        for( auto& pair : m_states )
        {
            delete pair.second;
        }
    }

    // Deleted to prevent copying/moving
    StateMachine( const StateMachine<T>& rhs ) = delete;
    StateMachine( StateMachine<T>&& rhs ) = delete;
    StateMachine& operator=( const StateMachine& rhs ) = delete;
    StateMachine& operator=( StateMachine&& rhs ) = delete;

public:
    static StateMachine<T>* instance( void )
    {
        static StateMachine<T> machine;
        return &machine;
    }

    IGameState* const GetCurrentState( T state )
    {
        const auto& iter = m_states.find( state );
        if( iter == m_states.cend() )
        {
            return nullptr;
        }

        return iter->second;
    }

    template <typename U, typename... Args>
    bool RegisterState( T state, Args&&... args )
    {
        static_assert( std::is_base_of<IGameState, U>::value, "Must be an instance of IGameState!" );

        const auto& iter = m_states.find( state );
        if( iter == m_states.cend() )
        {
            U* newState = new U( std::forward<Args>( args )... );
            m_states.emplace( state, newState );

            return true;
        }

        return false;
    }

    bool GoToState( T state )
    {
        const auto& iter = m_states.find( state );
        if( iter == m_states.cend() )
        {
            return false;
        }

        if( m_pCurrState )
        {
            m_pCurrState->Exit();
        }

        m_pCurrState = iter->second;

        if( m_pCurrState )
        {
            m_pCurrState->Enter();
        }

        return true;
    }

    void Update( float deltaTime, float totalTime )
    {
        if( m_pCurrState )
        {
            m_pCurrState->Update( deltaTime, totalTime );
        }
    }
};

#endif
