#pragma once

#include "GT/Core/Base.h"

#include "AssetMetadata.h"


namespace GT {

    // index in AssetSlots, and AssetMetadata
    // Refcount IsLoaded NeedReload
    struct AssetInfo
    {
        uint32_t Index,Refcount;
        bool IsLoaded = false;   // Indentify asset is loaded or not
        bool NeedReload = false; // OpenGL not safe at multithread, need delay reload
        AssetMetadata metadata;
    };

    class Asset
    {
    public:
        virtual ~Asset() = default;

        virtual AssetType GetType() const = 0;
		virtual uint32_t GetMemorySize() const = 0;

        virtual const std::string& GetName() const = 0;

        std::string Name = "None";
        UUID ID;

        Ref<AssetInfo> Info;
    };
}



