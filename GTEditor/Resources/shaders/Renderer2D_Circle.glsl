//--------------------------
// - Hazel 2D -
// Renderer2D Circle Shader
// --------------------------

#type vertex
#version 450 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;
layout(location = 5) in int a_EntityID;
layout(location = 6) in int a_TexIndex;


uniform mat4 u_ViewProjection;


layout (location = 0) out vec3 LocalPosition;
layout (location = 1) out vec4 Color;
layout (location = 2) out float Thickness;
layout (location = 3) out float Fade;
layout (location = 4) out flat int v_EntityID;
layout (location = 5) out flat int v_TexIndex;

void main()
{
	LocalPosition = a_LocalPosition;
	Color = a_Color;
	Thickness = a_Thickness;
	Fade = a_Fade;

	v_EntityID = a_EntityID;
	v_TexIndex = a_TexIndex;

	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;


layout (location = 0) in vec3 LocalPosition;
layout (location = 1) in vec4 Color;
layout (location = 2) in float Thickness;
layout (location = 3) in float Fade;
layout (location = 4) in flat int v_EntityID;
layout (location = 5) in flat int v_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(LocalPosition);
    float circle = smoothstep(0.0, Fade, distance);
    circle *= smoothstep(Thickness + Fade, Thickness, distance);

	if (circle == 0.0)
		discard;
		
    // Set output color
	vec2 uv = LocalPosition.xy / 2.0f + 0.5f;
	 
    o_Color = Color;

	o_Color.a *= circle;


	o_Color = o_Color * texture(u_Textures[v_TexIndex], uv);

	o_EntityID = v_EntityID;
}