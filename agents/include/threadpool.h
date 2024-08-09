#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <barrier>

class AsyncModelWrapper;

class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>;
    void close();
    std::shared_ptr<AsyncModelWrapper> model; // hack

    bool stop;
private:
    std::unique_ptr<std::barrier<>> b;
    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;
    // the task queue

    // synchronization
    std::mutex mtx;
    std::condition_variable_any condition;
};

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads)
    :   stop(false)
{
    b.reset(new std::barrier(threads + 1));
    for (size_t i = 0;i<threads;++i) {
        workers.emplace_back(
            [this]
            {
                this->b->arrive_and_wait();
                while(true) {
                    std::unique_lock<std::mutex> lock(this->mtx);
                    this->condition.wait(lock,
                        [this]{ return this->stop; });
                    if(this->stop)
                        return;
                }
            }
        );
    }
    b->arrive_and_wait();
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    if(stop)
        throw std::runtime_error("enqueue on stopped ThreadPool");
    auto res = std::async(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    return res;
}

inline void ThreadPool::close()
{
    model.reset();
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}