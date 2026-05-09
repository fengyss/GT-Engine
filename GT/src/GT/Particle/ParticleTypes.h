#pragma once
#include <glm/glm.hpp>
#include <cstdint>

namespace GT {

    enum class BlendMode {
        None,
        Alpha,          // SrcAlpha / OneMinusSrcAlpha
        Additive,       // One / One
        Premultiplied   // One / OneMinusSrcAlpha
    };

    enum class  EmitterShape
    {
        Point,
        Box,
        Sphere,
        Ring,
        Cone,
        // Total number of shapes
        Count
    };
    // 粒子基础数据（紧凑内存布局，32字节对齐）
    struct alignas(32) Particle {
        glm::vec3 position{ 0.0f };      // 12字节
        glm::vec3 velocity{ 0.0f };     // 12字节

        glm::vec3 rotation{ 0.0f };      // 12字节
        glm::vec3 Rvelocity{ 0.0f };      // 12字节

        glm::vec4 color{ 1.0f };         // 16字节
        float lifeRemaining{ 1.0f };    // 4字节
        float size{ 1.0f };             // 4字节
        uint32_t textureIndex{ 0 };     // 4字节
        // 总计48字节，适合GPU缓冲区
    };

    // 发射器配置
    struct ParticleEmitterConfig {

        EmitterShape shape = EmitterShape::Point;

        glm::vec3 direction = { 1.0f,0.0f,0.0f };
        float velocity = 0.0f;
        float radius = 1.0f;
        glm::vec3 position{ 0.0f };

        // if not will be OnUnitSphere
        bool InUnitSphere = false;

        std::function<void(Particle&)> init_func;

        // 发射参数
        float spawnRate = 100.0f;      // 每秒粒子数
        float initialSpeed = 5.0f;
        float lifetime = 2.0f;


        float sizeStart = 0.5f;
        float sizeEnd = sizeStart;


        float innerRadius = 0.0f;
        float outerRadius = 1.0f;

        float coneAngle = 0.3f;


        // 随机化范围
        glm::vec3 positionVariance{ 1.0f };
        glm::vec3 velocityVariance{ 1.0f };
        glm::vec3 rotationVariance{ 1.0f };
        glm::vec4 colorVariance{ 0.1f };
        float sizeVariance = 0.2f;

        glm::vec4 color{ 1.0f };

        glm::vec4 colorStart{ 1.0f };
        glm::vec4 colorEnd{ 1.0f, 0.0f, 0.0f, 0.0f };


        // 物理参数
        float gravity = -9.81f;
        float dragCoefficient = 0.01f;

        // 渲染参数
        std::string texturePath = "assets/textures/particle.png";
        BlendMode blendMode = BlendMode::Additive;
    };
}