#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// STD
#include <map>
#include <type_traits>
#include <utility>

// Interfaces
#include "IGameState.h"

// Singleton
#include "Singleton.h"

template <typename T>
class StateMachine : public Singleton<StateMachine<T>>
{
private:
    std::map<T, IGameState*> m_states;
    IGameState* m_pCurrState;
    T m_currState;
    T m_targetState;
    bool m_shouldTransition;

public:
#pragma region Constructor & Destructor
    StateMachine(void)
        : m_pCurrState(nullptr)
    {
        static_assert(std::is_enum<T>::value, "Must be an Enum type!");
    }

    virtual ~StateMachine(void)
    {
        // Cleanup IGameStates
        for(auto& pair : m_states)
        {
            delete pair.second;
        }
    }
#pragma endregion

#pragma region Getters
    const IGameState* const GetCurrentIGameState(void) const
    {
        return m_pCurrState;
    }

    T const GetCurrentState(void) const
    {
        return m_currState;
    }
#pragma endregion

#pragma region StateMachine Methods
    template <typename U, typename... Args>
    bool RegisterState(T state, Args&&... args)
    {
        static_assert(std::is_base_of<IGameState, U>::value, "Must be an instance of IGameState!");

        const auto& iter = m_states.find(state);
        if(iter == m_states.cend())
        {
            U* newState = new U(std::forward<Args>(args)...);
            m_states.emplace(state, newState);

            return true;
        }

        return false;
    }

    bool GoToState(T state)
    {
        const auto& iter = m_states.find(state);
        if(iter == m_states.cend())
        {
            return false;
        }

        m_shouldTransition = true;
        m_targetState = state;

        return true;
    }

    void Update(float deltaTime, float totalTime)
    {
        if(m_shouldTransition)
        {
            if(m_pCurrState)
            {
                m_pCurrState->Exit();
            }

            m_pCurrState = m_states[m_targetState];
            m_currState = m_targetState;

            if(m_pCurrState)
            {
                m_pCurrState->Enter();
            }

            m_shouldTransition = false;
        }

        if(m_pCurrState)
        {
            m_pCurrState->Update(deltaTime, totalTime);
        }
    }
#pragma endregion

};

#endif
