#pragma once
#include <NetLinux.h>
#include <unistd.h>
#include <netinet/in.h>

namespace luguang {
	class Socket {
	public:
		Socket() : _nativeSocket(0) {}
		Socket(NativeSocket nativeSocket) : _nativeSocket(nativeSocket) { }
		virtual ~Socket() {
			close(_nativeSocket)
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
	
	class IStream {
    	public:
        	typedef std::function<int32_t(const char* buf, int64_t size)> DataHandler;

        	virtual int32_t Receive(char* buffer, int32_t bufferSize, int32_t& readSize) = 0;
        	virtual int32_t Send(const ByteArray& byteArray) = 0;

        	virtual void OnData(DataHandler handler) = 0;
        	virtual DataHandler GetDataHandler() = 0;
    	};	


}
