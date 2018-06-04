#ifndef NET_FRAMEWORK_COMMON_UTILS_H
#define NET_FRAMEWORK_COMMON_UTILS_H

#include <fcntl.h>
#include <cstdint>

namespace luguang {

    int32_t SetNonBlocking(int32_t sockfd);

}

#endif //NET_FRAMEWORK_COMMON_UTILS_H_H
