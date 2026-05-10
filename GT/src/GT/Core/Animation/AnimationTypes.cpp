#include "gtpch.h"
#include "AnimationTypes.h"

#include "GT/Utils/JsonUtils.h"

namespace GT
{
	void AnimationClip::ImportSpriteSheet(const std::filesystem::path& texPath)
	{
        auto jsonPath = texPath;
        jsonPath.replace_extension(".json");

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
        return ;
	}
}

