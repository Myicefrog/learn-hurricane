#include "EventQueueLoop.h"
#include "EventQueue.h"

namespace luguang {
    EventQueueLoop::EventQueueLoop(EventQueue* queue) : _queue(queue) {
    }

    void EventQueueLoop::_Run() {
        while ( true ) {
            std::shared_ptr <BaseEvent> event = _queue->GetEvent();
            if (!event) {
                continue;
            }

            OnEvent(event);
        }
    }
}

