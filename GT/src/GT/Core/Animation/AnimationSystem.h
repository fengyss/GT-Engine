#pragma once

#include "GT/Scene/Scene.h"

namespace GT {

    class Animation2DSystem
    {
    public:
        static void OnUpdate(Scene* scene, Timestep ts);
    };

   class Animation2DSystem
   {
   public:
       static void OnUpdate(Scene* scene, Timestep ts);
       void UpdateSkeletal2D(Scene* scene, Timestep ts);
   };

}