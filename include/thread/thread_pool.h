#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <queue>
#include <memory>
#include <vector>
#include <thread>
#include <functional>

#include "spin_lock.h"

class Task {
public:
    virtual void run() = 0;
    virtual ~Task() = default;
};

class ThreadPool {
public:
    static ThreadPool *getThreadPool();
    static void WorkerThread(ThreadPool *master);

    ~ThreadPool();

    void parallelFor(
        size_t width, size_t height, 
        const std::function<void(size_t, size_t)> &lambda,
        const bool complex = true
    );
    void wait() const;

    void addTask(std::shared_ptr<Task> task);
    std::shared_ptr<Task> getTask();

private:
    ThreadPool(size_t thread_count = 0);

    std::atomic<int> alive;
    std::atomic<int> pending_task_count;

    std::vector<std::thread> threads;
    std::queue<std::shared_ptr<Task>> tasks;
    mutable SpinLock spin_lock {};
};

#endif
