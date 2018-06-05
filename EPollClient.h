#ifndef NET_FRAMEWORK_EPOLLCLIENT_H
#define NET_FRAMEWORK_EPOLLCLIENT_H

#include "EPollStream.h"
#include "Net.h"
#include "DataSink.h"

#include <memory>

namespace luguang {

    	class EPollClient;

    	typedef std::shared_ptr<EPollClient> EPollClientPtr;

	class EPollClient : public EPollStream, public IConnectable {
	public:
    		EPollClient(const EPollClient& client) = delete;
    		virtual ~EPollClient() { }

    		virtual int32_t Receive(char* buffer, int32_t bufferSize, int32_t& readSize) override;
    		virtual int32_t Send(const ByteArray& byteArray) override;

    		uint32_t GetEvents() const {
        		return _events;
    		}

    		void SetEvents(uint32_t events) {
        		_events = events;
    		}

    		void Connect(const std::string& host, int32_t port) override;
    		static EPollClientPtr Connect(const std::string& ip, int32_t port, DataSink* dataSink);

	private:
    		EPollClient(NativeSocket clientSocket) :
            		EPollStream(clientSocket){
        		this->SetNativeSocket(clientSocket);
    		}

	private:
    		uint32_t _events;
};


}

#endif //NET_FRAMEWORK_EPOLLCLIENT_H

