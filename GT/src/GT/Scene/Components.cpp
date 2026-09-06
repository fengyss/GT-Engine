#include "gtpch.h"
#include "Components.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

namespace GT
{
    glm::mat4 LightRendererComponent::GetLightSpaceMatrix()
    {
        glm::mat4 lightView, lightProjection;
        switch (light.type)
        {
        case LightType::Directional:
            lightView = glm::lookAt(light.pos, -light.pos, glm::vec3(0.0f, 1.0f, 0.0f));
            lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, nearPlane, farPlane);
            break;
        case LightType::Point:
        case LightType::Spot:
            lightView = glm::lookAt(light.pos, light.pos + glm::normalize(light.direction), glm::vec3(0.0f, 1.0f, 0.0f));
            lightProjection = glm::perspective(perspectiveFov, aspectRatio, perspectiveNear, perspectiveFar);
            break;
        }
        return lightProjection * lightView;
    }
}