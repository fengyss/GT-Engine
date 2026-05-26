
#type vertex
#version 460 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 1.0, 1.0);
    TexCoords = vertex.zw;
}
 


#type fragment
#version 460 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D u_Text;
uniform vec4 u_TextColor;

void main()
{    
    color = texture(u_Text, TexCoords).r * u_TextColor;
}