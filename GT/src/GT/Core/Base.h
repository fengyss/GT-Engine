#pragma once

#include <memory>


#ifdef GT_PLATFORM_WINDOWS
	#if GT_DYNAMIC_LINK
		#ifdef GT_BUILD_DLL
			#define HAZEL_API __declspec(dllexport)
		#else
			#define HAZEL_API __declspec(dllimport)
		#endif // GT_BUILD_DLL
	#else 
		#define HAZEL_API 
	#endif // GT_DYNAMIC_LINK
#else
	#error GT Only support Windows!
#endif // GT_PLATFORM_WINDOWS

#ifdef GT_DEBUG
	#define GT_ENABLE_ASSERTS
#endif // GT_DEBUG


#ifdef GT_ENABLE_ASSERTS
	#define GT_ASSERT(x, ...) { if(!(x)) { GT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define GT_CORE_ASSERT(x, ...) { if(!(x)) { GT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define GT_ASSERT(x, ...)
	#define GT_CORE_ASSERT(x, ...)
#endif // GT_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#define GT_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
 
namespace GT
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

//----------------------------------------------------------------

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	
	
}