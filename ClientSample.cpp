#include "Net.h"
#include "EventQueue.h"
#include "EventQueueLoop.h"
#include "IoLoop.h"
#include "PackageDataSink.h"
#include "EPollServer.h"
#include "EPollClient.h"

#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <memory>

using namespace std;

const int32_t DefaultPort = 9000;
class SampleEventQueueLoop : public luguang::EventQueueLoop {
public:
    SampleEventQueueLoop(luguang::EventQueue* eventQueue) :
            EventQueueLoop(eventQueue) {}

protected:
    virtual void OnEvent(std::shared_ptr<luguang::BaseEvent> event) override {
	std::cout<<"OnEvent"<<std::endl;
        /*
       LOG(KLOG_DEBUG) << ("SampleEventQueueLoop::OnEvent");
        char buf[BUFSIZ]; // BUFSIZ is 1024
        sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\nHello World", 11);

        event->GetConnection()->Send(ByteArray(buf, strlen(buf)));
        LOG(KLOG_DEBUG) << ("SampleEventQueueLoop::OnEvent end");
         */
    }
};


int32_t main() {

    luguang::EventQueue mainEventQueue(5);
    luguang::IoLoop::Get()->Start();

    luguang::PackageDataSink dataSink(&mainEventQueue);

    luguang::EPollClientPtr client = luguang::EPollClient::Connect("127.0.0.1", DefaultPort, &dataSink);

    client->Send(luguang::ByteArray("hello", 5));

    SampleEventQueueLoop sampleQueue(&mainEventQueue);
    sampleQueue.Start();

    return 0;
}

