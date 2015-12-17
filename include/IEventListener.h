#ifndef IEVENT_LISTENER_H
#define IEVENT_LISTENER_H

// STD
#include <string>

/*
 * Implement the IEventListener interface to listen for incoming events.
 * Pass 'this' into EventManager.Register along with the name of the event.
 */
class IEventListener
{
public:
	virtual ~IEventListener(void){}

    /*
     * Receive an incoming Event.
     * @param   name        The name of the incoming event.
     * @param   data        The data associated with the event.
     */
	virtual void EventRouter(const std::string& name, void* data) = 0;
};

#endif
