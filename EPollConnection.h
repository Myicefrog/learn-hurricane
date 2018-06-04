#ifndef CPPSTORM_EPOLL_H
#define CPPSTORM_EPOLL_H

#include "NetLinux.h"
#include "Net.h"

#include "EPollStream.h"

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

namespace luguang {
    class EPollLoop;

    class EPollConnection : public EPollStream {
    public:
        EPollConnection(NativeSocket nativeSocket) :
                EPollStream(nativeSocket) { }
        virtual ~EPollConnection() { }

        EPollConnection(const EPollConnection& connection) = delete;
    };

    typedef std::shared_ptr <EPollConnection> EPollConnectionPtr;

}

#endif //CPPSTORM_EPOLL_H
