#ifndef NET_FRAME_EPOLLLOOP_H
#define NET_FRAME_EPOLLLOOP_H

#include "Loop.h"
#include "EPollConnection.h"
#include "EPollStream.h"
#include "EPollServer.h"
#include "EPollClient.h"
#include "Net.h"
#include "DataSink.h"
#include "NetLinux.h"
#include "Common.h"

#include <map>
#include <memory>
#include <thread>
#include <string>

#include <sys/epoll.h>


namespace luguang {

    class EventQueue;

    class EPollServer;

    class EPollLoop : public Loop {
    public:
        static EPollLoop* Get();

        virtual ~EPollLoop() override;

        void AddServer(NativeSocket socket, EPollServer* server);
        void AddStream(EPollStreamPtr stream);

        int32_t AddEpollEvents(int32_t events, int32_t fd);
        int32_t ModifyEpollEvents(int32_t events, int32_t fd);

    protected:
        EPollLoop();

        virtual void _Run() override;

    private:
        void _Initialize();

        void _EPollThread();

        void _HandleEvent(int32_t eventfd, struct epoll_event* events, int32_t nfds);

        int32_t _Accept(int32_t eventfd, int32_t listenfd);

        void _Read(int32_t eventfd, int32_t fd, uint32_t events);

        void _Enqueue(EPollStreamPtr connection, const char* buf, int64_t nread);

    private:
        int32_t _eventfd;
        bool _shutdown;

        std::map<NativeSocket, EPollServer*> _servers;
        std::map <NativeSocket, EPollStreamPtr> _streams;
    };
}

#endif //HURRICANE_EPOLLLOOP_H

