#include "EventBus.h"

EventBus::EventBus(){
    
}

void EventBus::Subscribe(EventType type, Layer* layer) {
    _subscribers[type].push_back(layer);
}

void EventBus::Unsubscribe(EventType type, Layer* layer) {
    auto it = std::find(_subscribers[type].begin(), _subscribers[type].end(), layer);

    if(it != _subscribers[type].end()) {
        _subscribers[type].erase(it);
    }
}

void EventBus::Notify(Event& event){
    if(event.HasBeenHandled()){
        return;
    }

    for(Layer* layer : _subscribers[event.GetEventType()]){
        layer->HandleEvent(event);

        if (event.HasBeenHandled()) break;

    }

    // Implement here some ececution if the event prooagated through the layers has not been handled
    if (!event.HasBeenHandled())
    {
        // {code here}
        // COMETA_WARNING("Event propagated has not been handled");
    }
}