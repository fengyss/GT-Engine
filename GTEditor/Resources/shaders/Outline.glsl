
//#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position; // World space position
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;
layout(location = 5) in ivec4 a_m_BoneIDs;
layout(location = 6) in vec4 a_Weights;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat3 u_NormalMatrix;

void main()
{
    vec3 v_FragPos = (u_Transform * vec4(a_Position, 1.0)).rgb;
    vec3 v_Normal = normalize(u_NormalMatrix * a_Normal);
    v_FragPos += v_Normal * 0.15;
    gl_Position = u_ViewProjection * vec4(v_FragPos, 1.0);
}



//#type fragment
#version 460 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

uniform vec4 u_color;
uniform int u_EntityID;


void main()
{

    o_Color = u_color;
    o_EntityID = u_EntityID;

}