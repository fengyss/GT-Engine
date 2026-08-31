#pragma once
#include "GT/Core/UUID.h"
#include "gtpch.h"
namespace GT
{ 

    enum class AssetType : uint8_t
    {
        None = 0,
        Scene,
        Texture2D,
        Texture3D,
        Shader,
        ComputeShader,
        GeometryShader,
        Model
    };

    std::string_view AssetTypeToString(AssetType type);
    AssetType AssetTypeFromString(std::string_view assetType);

    // ==================== 导入设置（按类型细分） ====================
    struct TextureImportSettings {
        std::string format = "BC7";       // 压缩格式
        bool generateMipmaps = true;
        bool sRGB = true;
        int maxSize = 2048;
    };

    struct ModelImportSettings {
        bool mergeMeshes = false;
        bool generateColliders = false;
        float boneCompressionPrecision = 0.01f;
        int lodLevels = 1;
    };

    struct SoundImportSettings {
        std::string compressionFormat = "OGG";
        int sampleRate = 44100;
        bool streaming = false;
        bool loop = false;
    };

    struct ShaderImportSettings {
        std::vector<std::string> keywords;
        bool enableInstancing = false;
    };

    struct MaterialImportSettings {
        std::string shaderUUID;   // 依赖的 Shader UUID
        std::vector<std::string> textureSlots; // 纹理槽位名
    };

    // ==================== 元数据（Meta） ====================
    struct AssetMetadata {
        UUID ID;                              // 唯一标识，一生不变

        //std::string fileName;                   // 原始文件名（含扩展名）
        //std::string relativePath;               // 项目内逻辑路径
        //std::string friendlyName;               // 用户可修改的显示名

        //std::vector<std::string> tags;          // 标签，用于分类检索
        std::vector<UUID> directDependencies;   // 直接依赖的 UUID 列表
        //int version = 1;                        // 资源版本号
        //std::string sourceHash;                 // 源文件 MD5 哈希
        //std::string lastModifiedTime;           // 最后修改时间（ISO 格式）

        // 导入设置（按类型使用对应字段）
        //std::optional<TextureImportSettings> textureSettings;
        //std::optional<ModelImportSettings> modelSettings;
        //std::optional<SoundImportSettings> soundSettings;
        //std::optional<ShaderImportSettings> shaderSettings;
        //std::optional<MaterialImportSettings> materialSettings;

        // 编辑器备注
        std::string editorNote;


        AssetType Type = AssetType::None;
        std::string Name;
        std::filesystem::path FilePath;
        bool IsWatch = false;


        operator bool() const { return Type != AssetType::None; }
    };



};


