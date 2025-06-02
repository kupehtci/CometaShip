#ifndef COMETA_EVENT_BUS_H
#define COMETA_EVENT_BUS_H

#include <vector>
#include <unordered_map>

#include "core/Singleton.h"

#include "Layer.h"
#include "Event.h"
#include "EventType.h"

/**
 * Subscription class that allow Layers to subscribe to the event bus.
 * This allow that in case of event creation they get notified
 */
class EventBus : public Singleton<EventBus>{

private: 
    std::unordered_map<EventType, std::vector<Layer*>> _subscribers; 

public: 
    EventBus();

    void Subscribe(EventType eventType, Layer* layer);
    void Unsubscribe(EventType eventType, Layer* layer);

    void Notify(Event& event);
}; 

#endif