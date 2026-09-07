#pragma once
#include <vector>
#include <thread>
#include "Job.h"
#include "GT/Core/Memory/LinearAllocator.h"
// �����������
#include "GT/Core/Concurrency/concurrentqueue.h"

namespace GT {

    class JobSystem {
    public:
        // ��ʼ���̳߳أ�numThreads = 0 ��ʾʹ��Ӳ������������
        static void Init(uint32_t numThreads = 0);
        static void Shutdown();

        // ��������ȡ��ǰ֡�ķ�����
        static LinearAllocator& GetAllocator();
        // ���� Job �Ĺ�������
        template<typename F>
        static Job* CreateJob(F&& func, Job* parent = nullptr) {
            // 1. �ӷ��������ڴ�
            void* mem = GetAllocator().Allocate(sizeof(Job));
            // 2. ���ڴ��Ϲ������Placement New��
            return new (mem) Job(std::forward<F>(func), parent);
        }

        // �ύһ������
        static void Submit(Job* job);

        // �ȴ�������ɣ�������ǰ�̣߳�ֱ�� job->unfinishedJobs == 0��
        static void Wait(const Job* job);

    private:
        static void WorkerThreadMain(uint32_t threadIndex);

    private:
        static inline bool s_IsRunning = false;
        static inline std::vector<std::thread> s_WorkerThreads;

        // �ؼ�����������
        static inline moodycamel::ConcurrentQueue<Job*> s_JobQueue{ 1024, 8, 8 };

        // ���ڻ��������̵߳��ź�����C++20 �� std::counting_semaphore��
        // ���û�� C++20�������� condition_variable ģ��
        static inline std::condition_variable s_WakeCondition;
        static inline std::mutex s_WakeMutex;

        static inline LinearAllocator* s_FrameAllocator;
    };

}