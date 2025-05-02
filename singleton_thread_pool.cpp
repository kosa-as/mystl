#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <atomic>
#include <memory>

class SingletonThreadPool {

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop;
    explicit SingletonThreadPool(size_t threads) : stop(false) {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] {
                            return this->stop.load() || !this->tasks.empty();
                        });
                        if (this->stop.load() && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }


public:
    SingletonThreadPool& operator= (const SingletonThreadPool &) = delete;
    SingletonThreadPool(const SingletonThreadPool &) = delete;
    SingletonThreadPool(SingletonThreadPool &&) = delete;
    SingletonThreadPool& operator= (SingletonThreadPool &&) = delete;

    ~SingletonThreadPool() {
        stop.store(true);
        condition.notify_all();
        for (std::thread &worker : workers)
            if (worker.joinable())
                worker.join();
    }
    
    static SingletonThreadPool* get_thread_pool(size_t threads) {
        static std::unique_ptr<SingletonThreadPool> ptr(new SingletonThreadPool(threads));
        return ptr.get();
    }

    template<class F, class... Args>
    auto submit(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
        using return_type = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );


        std::future<return_type> res = task->get_future();
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            if (stop.load())
                throw std::runtime_error("submit on stopped ThreadPool");
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }
};

// ==================== 使用示例 ====================
#include <iostream>
#include <chrono>

int main() {
    auto pool = SingletonThreadPool::get_thread_pool(4);

    std::vector<std::future<int>> results;

    for (int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool->submit([i] {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << "Task " << i << " done\n";
                return i;
            })
        );
    }
    for (auto &&result : results)
        std::cout << result.get() << std::endl;
    return 0;
}