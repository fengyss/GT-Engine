#include "gtpch.h"
#include "JobSystem.h"
#include <iostream>

namespace GT {
    void JobSystem::Init(uint32_t numThreads) {

        s_FrameAllocator = new LinearAllocator(10 * 1024 * 1024);

        s_IsRunning = true;

        unsigned int hc = std::thread::hardware_concurrency();
        uint32_t threadCount = (numThreads == 0) ? (hc == 0 ? 1u : hc) : numThreads;

        //std::thread k = std::thread(WorkerThreadMain, 0); //
        for (uint32_t i = 0; i < threadCount; ++i) {
            s_WorkerThreads.emplace_back(WorkerThreadMain, i);
        }
    }

    void JobSystem::Shutdown() {
        delete s_FrameAllocator;

        s_IsRunning = false;
        s_WakeCondition.notify_all(); // 唤醒所有线程准备退出
        for (auto& t : s_WorkerThreads) {
            if (t.joinable()) t.join();
        }
    }

    LinearAllocator& JobSystem::GetAllocator() {
        return *s_FrameAllocator;
    }

    void JobSystem::WorkerThreadMain(uint32_t threadIndex) {
        std::cout << "Job System Thread " << threadIndex << " started.\n";

        while (s_IsRunning) {
            Job* job = nullptr;

            // 尝试从队列取任务
            if (s_JobQueue.try_dequeue(job)) {
                // 执行任务
                if (job->function) {
                    job->function();
                }
                // 标记完成
                job->Finish();
            }
            else {
                // 没活干，休眠等待信号量唤醒
                std::unique_lock<std::mutex> lock(s_WakeMutex);
                s_WakeCondition.wait(lock);
            }
        }
    }

  

    // JobSystem.cpp
    void JobSystem::Submit(Job* job) {
        // 初始状态：1 表示“正在执行”
        job->unfinishedJobs.store(1, std::memory_order_relaxed);
        s_JobQueue.enqueue(job);
        s_WakeCondition.notify_one(); // 唤醒一个休眠的线程
    }

    void JobSystem::Wait(const Job* job) {
        // 自旋等待（Spinlock）
        // 注意：在生产环境中，这里最好用 yield 或者 sleep_for 避免占满 CPU
        while (job->unfinishedJobs.load(std::memory_order_acquire) > 0) {
            // 忙等待，或者尝试偷一个任务来做（防止死锁）
            std::this_thread::yield();
        }
    }

    // Job.h 中实现
    inline void Job::Finish() {
        // 使用更保守的 acq_rel，确保前序写对等待方可见
        int prev = unfinishedJobs.fetch_sub(1, std::memory_order_acq_rel);

        // 如果我是最后一个完成的子任务，通知父任务
        if (parent && prev == 1) {
            parent->unfinishedJobs.fetch_sub(1, std::memory_order_acq_rel);
        }
    }
}