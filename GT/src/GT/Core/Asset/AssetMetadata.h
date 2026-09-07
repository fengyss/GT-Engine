#pragma once
#include "GT/Core/UUID.h"
#include <vector>
#include <string>
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
        Mesh,
        Model
    };

    std::string_view AssetTypeToString(AssetType type);
    AssetType AssetTypeFromString(std::string_view assetType);

    // ==================== �������ã�������ϸ�֣� ====================
    struct TextureImportSettings {
        std::string format = "BC7";       // ѹ����ʽ
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
        std::string shaderUUID;   // ������ Shader UUID
        std::vector<std::string> textureSlots; // ������λ��
    };

    // ==================== Ԫ���ݣ�Meta�� ====================
    struct AssetMetadata {
        UUID ID;                              // Ψһ��ʶ��һ������

        //std::string fileName;                   // ԭʼ�ļ���������չ����
        //std::string relativePath;               // ��Ŀ���߼�·��
        //std::string friendlyName;               // �û����޸ĵ���ʾ��

        //std::vector<std::string> tags;          // ��ǩ�����ڷ������
        std::vector<UUID> directDependencies;   // ֱ�������� UUID �б�
        //int version = 1;                        // ��Դ�汾��
        //std::string sourceHash;                 // Դ�ļ� MD5 ��ϣ
        //std::string lastModifiedTime;           // ����޸�ʱ�䣨ISO ��ʽ��

        // �������ã�������ʹ�ö�Ӧ�ֶΣ�
        //std::optional<TextureImportSettings> textureSettings;
        //std::optional<ModelImportSettings> modelSettings;
        //std::optional<SoundImportSettings> soundSettings;
        //std::optional<ShaderImportSettings> shaderSettings;
        //std::optional<MaterialImportSettings> materialSettings;

        // �༭����ע
        std::string editorNote;


        AssetType Type = AssetType::None;
        std::string Name;
        std::filesystem::path FilePath;
        bool IsWatch = false;


        operator bool() const { return Type != AssetType::None; }
    };



};


