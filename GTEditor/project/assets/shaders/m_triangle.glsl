#type vertex
#version 460 core
			
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;
			
void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(position,1.0);
	v_Color = color;
}



#type fragment
#version 460 core
			
layout(location = 0) out vec4 color;
			
in vec4 v_Color;
			
void main()
{
	color = v_Color;
}