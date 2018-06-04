#ifndef NET_FRAMEWORK_EPOLLSTREAM_H_H
#define NET_FRAMEWORK_EPOLLSTREAM_H_H

#include "NetLinux.h"
#include "Net.h"

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

namespace luguang {
	//class EPollLoop;
	class EPollStream : public BasicStream {
	public:
		EPollStream(NativeSocket nativeSocket) :
                	BasicStream(nativeSocket) {}
		virtual ~EPollStream() { }
		EPollStream(const EPollStream& stream) = delete;
		        
		virtual int32_t Receive(char* buffer, int32_t bufferSize, int32_t& readSize) override;
        	virtual int32_t Send(const ByteArray& byteArray) override;

		uint32_t GetEvents() const {
            		return _events;
        	}

        	void SetEvents(uint32_t events) {
            		_events = events;
        	}

        	void OnData(DataHandler handler) override {
            		_dataHandler = handler;
       		}

	        DataHandler GetDataHandler() override {
            		return _dataHandler;
        	}

	private:
        	uint32_t _events;
        	DataHandler _dataHandler;
	};

	typedef std::shared_ptr <EPollStream> EPollStreamPtr;
}

#endif //NET_FRAMEWORK_EPOLLSTREAM_H_H
