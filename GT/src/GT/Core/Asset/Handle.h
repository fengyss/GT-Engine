#pragma once
#include "GT/Core/UUID.h"
#include "GT/Core/Log.h"
namespace GT
{
    // used to retrive asset from assetmanager
    // UUID, index, generation
    struct Handle
    {
        UUID ID = 0;  // asset's unique identifier
        uint32_t index = 0; //asset's index in AssetManager's assets
        uint32_t generation = 0;


        bool operator==(const Handle& other) const {
            return ID == other.ID;
        }

    };

}


template <>
struct fmt::formatter<GT::Handle> {
    constexpr auto parse(fmt::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const GT::Handle& handle, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "Handle(ID:{},Index:{})", handle.ID, handle.index);
    }
};
