
/*********************************************************
 *
 *  Stub thread pool for single-threaded builds.
 *  Provides the same interface as ctpl::thread_pool but
 *  executes all tasks synchronously on the calling thread.
 *
 *********************************************************/

#ifndef __ctpl_stub_thread_pool_H__
#define __ctpl_stub_thread_pool_H__

#include <functional>
#include <future>
#include <memory>

namespace ctpl {

    // Stub thread_pool that executes tasks synchronously
    // Used when HAVE_BOOST_BASED_THREAD_POOL_LIBRARY is not defined
    class thread_pool {

    public:

        thread_pool() {}
        thread_pool(int nThreads, int queueSize = 100) {
            // Ignore thread count - we execute everything synchronously
            (void)nThreads;
            (void)queueSize;
        }

        ~thread_pool() {}

        // Always report 1 "thread" (the main thread)
        int size() { return 1; }

        // No idle threads in stub
        int n_idle() { return 0; }

        // Change thread count - no-op in stub
        void resize(int nThreads) {
            (void)nThreads;
        }

        // Clear queue - no-op since we execute immediately
        void clear_queue() {}

        // Pop - returns empty function since queue is always empty
        std::function<void(int)> pop() {
            return std::function<void(int)>();
        }

        // Stop - no-op in stub
        void stop(bool isWait = false) {
            (void)isWait;
        }

        // Push with additional arguments - execute synchronously and return future
        template<typename F, typename... Rest>
        auto push(F && f, Rest&&... rest) -> std::future<decltype(f(0, rest...))> {
            // Create a packaged task
            auto pck = std::make_shared<std::packaged_task<decltype(f(0, rest...))(int)>>(
                std::bind(std::forward<F>(f), std::placeholders::_1, std::forward<Rest>(rest)...)
            );

            // Execute immediately with thread id 0
            (*pck)(0);

            return pck->get_future();
        }

        // Push simple function - execute synchronously and return future
        template<typename F>
        auto push(F && f) -> std::future<decltype(f(0))> {
            auto pck = std::make_shared<std::packaged_task<decltype(f(0))(int)>>(std::forward<F>(f));

            // Execute immediately with thread id 0
            (*pck)(0);

            return pck->get_future();
        }

    private:
        // Deleted copy/move constructors and assignment operators
        thread_pool(const thread_pool &) = delete;
        thread_pool(thread_pool &&) = delete;
        thread_pool & operator=(const thread_pool &) = delete;
        thread_pool & operator=(thread_pool &&) = delete;
    };

}

#endif // __ctpl_stub_thread_pool_H__
