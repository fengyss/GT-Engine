#include "gtpch.h"
#include "AnimationTypes.h"
//#include "GT/Core/Log.h"
#include "GT/Utils/JsonUtils.h"

namespace GT
{
	bool AnimationClip::ImportSpriteSheet(const std::filesystem::path& texPath)
	{
        auto jsonPath = texPath;
        jsonPath.replace_extension(".json");

        if (!std::filesystem::exists(jsonPath))
        {
            GT_CORE_ERROR("Json file path : {0} not exited!",jsonPath.string());
            return false;
        }

        auto json = Utils::LoadJSON(jsonPath);

        for (auto& s : json["Sprites"])
        {
            SpriteRegion r;
            r.PixelOffset = { s["X"], s["Y"] };
            r.PixelSize = { s["W"], s["H"] };
            r.UVOffset = { s["U"], s["V"] };
            r.UVSize = { s["USize"], s["VSize"] };
            AddFrame(r,0.2f);
        }
        FrameDuration = m_Duration / count;
        return true;
	}

    void AnimationClip::AddFrame(const SpriteRegion& frame, float duration)
    {
        count++;
        m_Frames.push_back(frame);
        m_FrameDurations.push_back(duration);
    }
}

