#pragma once
#include <functional>
#include <atomic>

namespace GT {

    // 前向声明
    struct Job;

    // 任务函数类型
    using JobFunction = std::function<void()>;

    struct Job {
        JobFunction function;
        std::atomic<int> unfinishedJobs{ 0 }; // 未完成的子任务数
        Job* parent{ nullptr };              // 父任务指针

        // 构造函数
        Job(JobFunction func, Job* p = nullptr)
            : function(func), parent(p) {
        }

        void Finish();
    };

}