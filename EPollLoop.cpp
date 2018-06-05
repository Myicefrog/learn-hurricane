#include "EPollLoop.h"

#include <signal.h>
#include <cassert>
#include <iostream>

namespace luguang {
    using namespace std::placeholders;

    EPollLoop* EPollLoop::Get()
    {
        static EPollLoop epollLoop;
        return &epollLoop;
    }

    EPollLoop::EPollLoop()
    {
        std::cout << "EPollLoop::EPollLoop"<<std::endl;

        // TODO: temproray approach to avoid crash
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGPIPE);
        sigprocmask(SIG_BLOCK, &set, NULL);

        _Initialize();
    }

    EPollLoop::~EPollLoop()
    {
        _shutdown = true;
    }
    
    void EPollLoop::AddServer(NativeSocket socket, EPollServer* server)
    {
        _servers.insert({socket, server});
    }

    void EPollLoop::AddStream(EPollStreamPtr stream)
    {
        _streams[stream->GetNativeSocket()] = stream;
    }

    int32_t EPollLoop::AddEpollEvents(int32_t events, int32_t fd)
    {
        NativeSocketEvent ev;
        ev.events = events;
        ev.data.fd = fd;

        return epoll_ctl(_eventfd, EPOLL_CTL_ADD, fd, &ev);
    }

    int32_t EPollLoop::ModifyEpollEvents(int32_t events, int32_t fd)
    {
        NativeSocketEvent ev;
        ev.events = events;
        ev.data.fd = fd;

        return epoll_ctl(_eventfd, EPOLL_CTL_MOD, fd, &ev);
    }

    void EPollLoop::_Initialize()
    {
        _eventfd = epoll_create(MAX_EVENT_COUNT);
        if (_eventfd == -1) {
            std::cout << "FATAL epoll_create failed!"<<std::endl;
            assert(0);
        }
    }

    void EPollLoop::_Run()
    {
        auto func = std::bind(&EPollLoop::_EPollThread, this);
        std::thread listenThread(func);
        listenThread.detach();
    }

    void EPollLoop::_EPollThread()
    {
        std::cout<< "_EPollThread"<<std::endl;
        NativeSocketEvent events[MAX_EVENT_COUNT];

        while (!_shutdown) {
            int32_t nfds;
            nfds = epoll_wait(_eventfd, events, MAX_EVENT_COUNT, -1);
            if (-1 == nfds) {
                std::cout<< "FATAL epoll_wait failed!"<<std::endl;
                exit(EXIT_FAILURE);
            }

            _HandleEvent(_eventfd, events, nfds);
        }
    }

    void EPollLoop::_HandleEvent(int32_t eventfd, NativeSocketEvent* events, int32_t nfds)
    {
        for (int32_t i = 0; i < nfds; ++i) {
            int32_t fd;
            fd = events[i].data.fd;

            if (_servers.find(fd) != _servers.end()) {
                _Accept(eventfd, fd);
                continue;
            }

            int32_t n = 0;
            if (events[i].events & EPOLLIN) {
                _Read(eventfd, fd, events[i].events);
            }

            if (events[i].events & EPOLLOUT) {
            }
        }
    }

    int32_t EPollLoop::_Accept(int32_t eventfd, int32_t listenfd)
    {
        std::cout<< "_Accept"<<std::endl;
        EPollServer* server = _servers.find(listenfd)->second;
        EPollConnectionPtr connection = server->Accept(eventfd);

        if (connection != nullptr) {
            _streams[connection->GetNativeSocket()] = connection;
        }
    }

    void EPollLoop::_Read(int32_t eventfd, int32_t fd, uint32_t events)
    {
        std::cout<< "_Read"<<std::endl;

        EPollStreamPtr stream = _streams[fd];

        char buffer[BUFSIZ];
        int32_t readSize;
        int32_t nread = stream->Receive(buffer, BUFSIZ, readSize);

        stream->SetEvents(events);

        if ((nread == -1 && errno != EAGAIN) || readSize == 0) {
            _streams.erase(fd);
		std::cout<<"error"<<std::endl;
            return;
        }

        // Write buf to the receive queue.
        _Enqueue(stream, buffer, readSize);
    }

    void EPollLoop::_Enqueue(EPollStreamPtr stream, const char* buf, int64_t nread)
    {
        std::cout<< "_Enqueue"<<std::endl;

        if ( stream->GetDataHandler() ) {
            stream->GetDataHandler()(buf, nread);
        }
    }

}

