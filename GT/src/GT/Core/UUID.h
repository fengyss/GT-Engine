#pragma once
#include <cstdint>
#include "GT/Core/Log.h"

namespace GT {

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;


	// ¸ñÊ½»¯Âß¼­
	//auto format(const UUID& id, fmt::format_context& ctx) const {

	//	return fmt::format_to(ctx.out(), "{}", id);
	//}
	};


}

template <>
struct fmt::formatter<GT::UUID> {
	constexpr auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }

	auto format(const GT::UUID& id, fmt::format_context& ctx) const {
		return fmt::format_to(ctx.out(), "{}", static_cast<uint64_t>(id));
	}
};

namespace std {

	template<>
	struct std::hash<GT::UUID>
	{
		std::size_t operator()(const GT::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};

}