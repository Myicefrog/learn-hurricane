#include "EPollServer.h"
#include "EPollLoop.h"
#include "CommonUtils.h"

#include <cstdint>
#include <cassert>

#ifndef DISABLE_ASSERT
#ifdef assert
#undef assert
#endif

#define assert(x)
#endif

using namespace std;
namespace luguang {
    int32_t EPollServer::_Bind(const std::string& host, int32_t port) {
        int32_t listenfd;
        if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            cout << "Create socket failed!"<<endl;
            exit(1);
        }

        SetNativeSocket(listenfd);
        int32_t option = 1;
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

        // make socket non-blocking
        luguang::SetNonBlocking(listenfd);

        NativeSocketAddress addr;
        bzero(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(host.c_str());

        int32_t errorCode = bind(listenfd, (struct sockaddr*)&addr, sizeof(addr));
        if (errorCode < 0) {
            cout << "Bind socket failed!"<<endl;
            assert(0);
            return errorCode;
        }
        
        return 0;
    }

    int32_t EPollServer::Listen(const std::string& host, int32_t port, int32_t backlog) {
        _Bind(host, port);

        int32_t listenfd = GetNativeSocket();

        int32_t errorCode = listen(listenfd, backlog);
        if (-1 == errorCode) {
            cout << "Listen socket failed!"<<endl;
            assert(0);
            return errorCode;
        }

        errorCode = EPollLoop::Get()->AddEpollEvents(EPOLLIN, listenfd);

        if (errorCode == -1) {
            std::cout << "FATAL epoll_ctl: listen_sock!"<< std::endl;
            assert(0);
            return errorCode;
        }

        EPollLoop::Get()->AddServer(listenfd, this);
    }

    EPollConnectionPtr EPollServer::Accept(int32_t sockfd) {
        int32_t conn_sock = 0;
        int32_t addrlen = 0;
        int32_t remote = 0;

        int32_t listenfd = GetNativeSocket();
        while ((conn_sock = accept(listenfd, (struct sockaddr*)&remote,
                                   (socklen_t*)& addrlen)) > 0) {
            luguang::SetNonBlocking(conn_sock);

            NativeSocketEvent ev;
            ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = conn_sock;

            if (epoll_ctl(sockfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                perror("epoll_ctl: add");
                exit(EXIT_FAILURE);
            }

            EPollConnectionPtr connection = std::make_shared<EPollConnection>(conn_sock);
            if ( _connectHandler ) {
                _connectHandler(connection.get());
            }

            return connection;
        } // while

        if (conn_sock == -1) {
            if (errno != EAGAIN && errno != ECONNABORTED
                && errno != EPROTO && errno != EINTR)
                perror("accept");
        }

        return EPollConnectionPtr(nullptr);
    }
}

