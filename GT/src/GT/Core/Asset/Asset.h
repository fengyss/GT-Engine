#pragma once

#include "GT/Core/Base.h"

#include "AssetMetadata.h"


namespace GT {

    // used to retrive asset from assetmanager
    struct Handle
    {
        uint32_t slot = 0;  // asset's slot in AssetManager's assets
        uint32_t generation = 0;

        // 可选的比较运算符，用于放入 unordered_set/map
        bool operator==(const Handle& other) const {
            return slot == other.slot && generation == other.generation;
        }
    };


    class Asset
    {
    public:
        virtual ~Asset() = default;

        virtual AssetType GetType() const = 0;
		virtual uint32_t GetMemorySize() const = 0;

        AssetMetadata metadata;
        uint32_t count = 0;
        bool NeedReload = false;
    };
}



