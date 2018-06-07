#ifndef NET_FRAME_DATASINK_H
#define NET_FRAME_DATASINK_H

#include "Net.h"

#include <cstdint>
#include <functional>

namespace luguang {
    class DataSink {
    public:
        virtual int32_t Write(IStream* stream, const char* buf, int64_t bytes) = 0;
        IStream::DataHandler StreamDataHandler(IStream* stream) {
            return std::bind(&DataSink::Write, this, stream, std::placeholders::_1, std::placeholders::_2);
        }
    };
}

#endif //NET_FRAME_DATASINK_H
