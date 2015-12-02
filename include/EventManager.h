#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

// Singleton
#include "Singleton.h"

// Events
#include "IEventListener.h"

// STD
#include <vector>
#include <string>
#include <unordered_map>

class EventManager : public Singleton<EventManager>
{
private:
    std::unordered_map<std::string, std::vector<IEventListener*>> m_listeners;

public:
	virtual ~EventManager(void) {}

    /*
     * Register an IEventListener with the specified event.
     * @param   name        The Event the listener is observing.
     * @param   listener    The listener to be attached.
     * @returns A boolean indicating if this operation was successful.
     */
	bool Register(const std::string& name, IEventListener* listener);

    /*
     * Remove the given IEventListener from the given Event.
     * @param   name        The Event the listener is observing.
     * @param   listener   The listener to be detached.
     * @returns A boolea indicating if this operation was successful.
     */
	bool UnRegister(const std::string& name, IEventListener* listener);

    /*
     * Find all the IEventListeners registered to the Event and Route the Event
     * @param   name    The name of the Event to trigger.
     * @param   data    The data associated with the Event.
     * @returns A boolean indicating if this operation was successful.
     */
	bool Fire(const std::string& name, void* data);
};

#endif
