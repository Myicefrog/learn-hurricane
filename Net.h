#pragma once
#include "NetLinux.h"
#include <unistd.h>
#include <netinet/in.h>

#include "ByteArray.h"

#include <memory>
#include <string>
#include <iostream>
#include <functional>

#include <unistd.h>
#include <netinet/in.h>

namespace luguang {
	class Socket {
	public:
		Socket() : _nativeSocket(0) {}
		Socket(NativeSocket nativeSocket) : _nativeSocket(nativeSocket) { }
		virtual ~Socket() {
			close(_nativeSocket);
		}
		NativeSocket GetNativeSocket() const {
            return _nativeSocket;
        }

        void SetNativeSocket(NativeSocket nativeSocket) {
            _nativeSocket = nativeSocket;
        }

   	private:
        	NativeSocket _nativeSocket;
    	};
	

	class DataSink;

	class IStream {
    	public:
        	typedef std::function<int32_t(const char* buf, int64_t size)> DataHandler;

        	virtual int32_t Receive(char* buffer, int32_t bufferSize, int32_t& readSize) = 0;
        	virtual int32_t Send(const ByteArray& byteArray) = 0;

        	virtual void OnData(DataHandler handler) = 0;
        	virtual DataHandler GetDataHandler() = 0;
    	};	

	class IConnectable {
    	public:
        	virtual void Connect(const std::string& host, int32_t port) = 0;
    	};

	template<class ConnectionType>
    	class BasicServer : public Socket {
    	public:
        	typedef std::function<void(IStream* stream)> ConnectHandler;
        	typedef std::function<void(IStream* stream)> DisconnectHandler;

        	BasicServer() { }

        	virtual int32_t Listen(const std::string& host, int32_t port, int32_t backlog) = 0;
        	virtual void OnConnect(ConnectHandler handler) = 0;
        	virtual void OnDisconnec(DisconnectHandler handler) = 0;

        	virtual ConnectionType Accept(int32_t listenfd) = 0;
    	};


	class BasicStream : public IStream, public Socket {
   	public:
        	BasicStream() = default;
        	BasicStream(NativeSocket nativeSocket) : Socket(nativeSocket) {}

        	BasicStream(const BasicStream& stream) = delete;

        	virtual void SetDataSink(DataSink* dataSink) {
            		_dataSink = dataSink;
        	}

        	virtual DataSink* GetDataSink() {
            		return _dataSink;
        	}

        	virtual const DataSink* GetDataSink() const {
            		return _dataSink;
        	}

    	private:
        	DataSink* _dataSink;
    	};

	
}
