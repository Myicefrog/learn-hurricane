#include "EPollStream.h"
#include "EPollLoop.h"
#include "ByteArray.h"

#include <unistd.h>

namespace luguang {
    int32_t EPollStream::Receive(char* buffer, int32_t bufferSize, int32_t& readSize) {
        readSize = 0;
        int32_t nread = 0;
        NativeSocketEvent ev;

        while ((nread = read(GetNativeSocket(), buffer + readSize, bufferSize - 1)) > 0) {
            readSize += nread;
        }

        return nread;
    }

    int32_t EPollStream::Send(const meshy::ByteArray& byteArray) {
        LOG(LOG_DEBUG) << "EPollConnection::Send";

        struct epoll_event ev;
        NativeSocket clientSocket = GetNativeSocket();

        if ( EPollLoop::Get()->ModifyEpollEvents(_events | EPOLLOUT, clientSocket) ) {
            // TODO: MARK ERASE
            LOG(LOG_ERROR) << "FATAL epoll_ctl: mod failed!";
        }

        const char* buf = byteArray.data();
        int32_t size = byteArray.size();
        int32_t n = size;

        while (n > 0) {
            int32_t nwrite;
            nwrite = write(clientSocket, buf + size - n, n);
            if (nwrite < n) {
                if (nwrite == -1 && errno != EAGAIN) {
                    LOG(LOG_ERROR) << "FATAL write data to peer failed!";
                }
                break;
            }
            n -= nwrite;
        }

        return 0;
    }
}

