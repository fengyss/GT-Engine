
//#type vertex
#version 460 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_Texcoord;
out flat float v_TexIndex;
out float v_TilingFactor;
out flat int v_EntityID;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position,1.0);

	v_Texcoord = a_TexCoord;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	v_EntityID = a_EntityID;
}



//#type fragment
#version 460 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID; 
			
uniform sampler2D u_Textures[32];

in vec4 v_Color;
in vec2 v_Texcoord;
in flat float v_TexIndex;
in float v_TilingFactor;
in flat int v_EntityID;
			
void main()
{
	o_Color = texture(u_Textures[int(v_TexIndex)], v_Texcoord * v_TilingFactor) * v_Color;

	if (o_Color.a == 0.0)
		discard;

	o_EntityID = v_EntityID;
}