#include "PackageDataSink.h"
#include "EventQueue.h"
#include "Net.h"
//#include "rest/HttpContext.h"
//#include "rest/HttpRequest.h"
//#include "rest/HttpResponse.h"

namespace meshy {
    PackageDataSink::PackageDataSink(EventQueue* eventQueue) : _eventQueue(eventQueue), _totalSize(0) {
        _threadPool = new ThreadPool<BaseEvent>(10, [&](BaseEvent& event) {
            std::cout << "Thread onEvent sink!"<< std::endl;

            std::string requestText = event.GetData().ToStdString();
/*
            HttpRequest request;
            request.ParseStdString(requestText);

            HttpResponse response;
            response.SetVersion("HTTP/1.1");
            response.SetStatusCode(200);
            response.SetStatusMessage("OK");
            response.SetContent("Hello! Sink in Thread!");
*/
            //event.GetStream()->Send(ByteArray(response.ToStdString())); // Send to peer
            event.GetStream()->Send(ByteArray("I am response")); // Send to peer

            std::cout << "Thread onEvent sink end."<<std::endl;
        });
    }

    PackageDataSink::~PackageDataSink() {
        if (_threadPool) {
            delete _threadPool;
            _threadPool = nullptr;
        }
    }

    int32_t PackageDataSink::Write(IStream* stream, const char* buf, int64_t bytes) {
        _data.Concat(ByteArray(buf, static_cast<int32_t>(bytes)));
        // The package is Complete
        if (_data.size() >= _totalSize) {
            //_eventQueue->PostEvent(new BaseEvent("data", _data, connection));
            _threadPool->Submit(BaseEvent("data", _data, stream));

            _data.clear();
            _totalSize = 0;
        }

        return static_cast<int32_t>(bytes);
    }
}

