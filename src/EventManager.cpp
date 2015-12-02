#include "EventManager.h"

// STD
#include <algorithm>

/*
 * Register an IEventListener with the specified event.
 * @param   name        The Event the listener is observing.
 * @param   listener    The listener to be attached.
 * @returns A boolean indicating if this operation was successful.
 */
bool EventManager::Register(const std::string& name, IEventListener* listener)
{
    std::vector<IEventListener*>& listeners = m_listeners[name];

	// Make sure there are no duplicate listeners.
	for (auto& l : listeners)
	{
		if (l == listener)
        {
			return false;
        }
	}

    // Attach the listener.
    listeners.push_back(listener);

    return true;
}

/*
 * Remove the given IEventListener from the given Event.
 * @param   name        The Event the listener is observing.
 * @param   listener   The listener to be detached.
 * @returns A boolea indicating if this operation was successful.
 */
bool EventManager::UnRegister(const std::string& name, IEventListener* listener)
{
    // Check that the listener exists.
    auto iter = m_listeners.find(name);
    if(iter == m_listeners.cend())
    {
        return false;
    }

    // C++ Erase-Remove Idiom
    std::vector<IEventListener*>& list = m_listeners[name];
    list.erase
    (
        std::remove_if
        (
            list.begin(),
            list.end(),
            [&listener](auto& l){ return listener == l; }
        ),
        list.end()
    );

    return true;
}

/*
 * Find all the IEventListeners registered to the Event and Route the Event
 * @param   name    The name of the Event to trigger.
 * @param   data    The data associated with the Event.
 * @returns A boolean indicating if this operation was successful.
 */
bool EventManager::Fire(const std::string& name, void* data)
{
    // Check that the listener exists.
    auto iter = m_listeners.find(name);
    if(iter == m_listeners.cend())
    {
        return false; // EARLY RETURN
    }

    // Notify all listening parties.
    std::vector<IEventListener*>& listeners = m_listeners[name];
	for (auto& l : listeners)
	{
		l->EventRouter(name, data);
	}

    return true;
}
