#pragma once

#include "Loop.h"
#include <memory>


namespace luguang {
    class EventQueue;

    class BaseEvent;

    class EventQueueLoop : public Loop
    {
    public:
        EventQueueLoop(EventQueue* queue);

    protected:
        virtual void _Run();

        virtual void OnEvent(std::shared_ptr <BaseEvent> event) = 0;

    private:
        EventQueue* _queue;
    };
}

