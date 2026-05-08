#pragma once
#include <vector>
#include <thread>
#include "Job.h"
#include "GT/Core/Memory/LinearAllocator.h"
// 引入第三方库
#include "GT/Core/Concurrency/concurrentqueue.h"

namespace GT {

    class JobSystem {
    public:
        // 初始化线程池（numThreads = 0 表示使用硬件最大核心数）
        static void Init(uint32_t numThreads = 0);
        static void Shutdown();

        // 新增：获取当前帧的分配器
        static LinearAllocator& GetAllocator();
        // 创建 Job 的工厂方法
        template<typename F>
        static Job* CreateJob(F&& func, Job* parent = nullptr) {
            // 1. 从分配器拿内存
            void* mem = GetAllocator().Allocate(sizeof(Job));
            // 2. 在内存上构造对象（Placement New）
            return new (mem) Job(std::forward<F>(func), parent);
        }

        // 提交一个任务
        static void Submit(Job* job);

        // 等待任务完成（阻塞当前线程，直到 job->unfinishedJobs == 0）
        static void Wait(const Job* job);

    private:
        static void WorkerThreadMain(uint32_t threadIndex);

    private:
        static inline bool s_IsRunning = false;
        static inline std::vector<std::thread> s_WorkerThreads;

        // 关键：无锁队列
        static inline moodycamel::ConcurrentQueue<Job*> s_JobQueue;

        // 用于唤醒休眠线程的信号量（C++20 有 std::counting_semaphore）
        // 如果没有 C++20，可以用 condition_variable 模拟
        static inline std::condition_variable s_WakeCondition;
        static inline std::mutex s_WakeMutex;

        static inline LinearAllocator* s_FrameAllocator;
    };

}