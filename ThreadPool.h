#ifndef NET_FRAME_THREAD_POOL_H
#define NET_FRAME_THREAD_POOL_H

#include "ConcurrentQueue.h"

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#define MIN_THREADS 10

namespace luguang {
    template<class Type>
    class ThreadPool {
        ThreadPool& operator=(const ThreadPool&) = delete;

        ThreadPool(const ThreadPool& other) = delete;

    public:
        ThreadPool(int32_t threads, std::function<void(Type& record)> handler);

        virtual ~ThreadPool();

        void Submit(Type record);

    private:

    private:
        bool _shutdown;
        int32_t _threads;
        std::function<void(Type& record)> _handler;
        std::vector <std::thread> _workers;
        ConcurrentQueue <Type> _tasks;
    };

}
#include "thread_pool.tcc"

#endif //NET_FRAME_THREAD_POOL_H

