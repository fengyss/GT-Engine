#pragma once


#include "GT/Core/Base.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

// This ignore warnings in external code
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)


#include "spdlog/fmt/fmt.h"

namespace GT {

	class HAZEL_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;  }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }


	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}


template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}


template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

template <glm::length_t L, typename T, glm::qualifier Q>
struct fmt::formatter<glm::vec<L, T, Q>> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const glm::vec<L, T, Q>& v, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "{}", glm::to_string(v));
    }
};

template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
struct fmt::formatter<glm::mat<C, R, T, Q>> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const glm::mat<C, R, T, Q>& m, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "{}", glm::to_string(m));
    }
};

template <typename T, glm::qualifier Q>
struct fmt::formatter<glm::qua<T, Q>> {
    constexpr auto parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const glm::qua<T, Q>& q, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "{}", glm::to_string(q));
    }
};

// Core log macros		
#define GT_CORE_TRACE(...)       ::GT::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GT_CORE_INFO(...)        ::GT::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GT_CORE_WARN(...)        ::GT::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GT_CORE_ERROR(...)       ::GT::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GT_CORE_CRITICAL(...)       ::GT::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define GT_TRACE(...)            ::GT::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GT_INFO(...)             ::GT::Log::GetClientLogger()->info(__VA_ARGS__)
#define GT_WARN(...)             ::GT::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GT_ERROR(...)            ::GT::Log::GetClientLogger()->error(__VA_ARGS__)
#define GT_CRITICAL(...)            ::GT::Log::GetClientLogger()->critical(__VA_ARGS__)
