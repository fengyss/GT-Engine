#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in float aSize;

out vec4 vColor;
out float vSize;

void main()
{
    gl_Position = vec4(aPos, 1.0); // 世界坐标
    vColor = aColor;
    vSize = aSize;
}

#type geometry
#version 330 core
layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

in vec4 vColor[];
in float vSize[];

out vec4 fColor;
out vec2 fUV;

uniform mat4 u_ViewProjection;
uniform mat4 u_View;

void main()
{
    vec3 center = gl_in[0].gl_Position.xyz;
    float s = vSize[0];
    vec4 color = vColor[0];

    // 从 View 矩阵取 right / up
    vec3 right = vec3(u_ViewProjection[0][0], u_ViewProjection[1][0], u_ViewProjection[2][0]);
    vec3 up    = vec3(u_ViewProjection[0][1], u_ViewProjection[1][1], u_ViewProjection[2][1]);

    right *= s;
    up *= s;

    vec3 p0 = center - right - up;
    vec3 p1 = center + right - up;
    vec3 p2 = center - right + up;
    vec3 p3 = center + right + up;
     

    fColor = color;

    gl_Position = u_ViewProjection * vec4(p0,1.0); fUV = vec2(0,0); EmitVertex();
    gl_Position = u_ViewProjection * vec4(p1,1.0); fUV = vec2(1,0); EmitVertex();
    gl_Position = u_ViewProjection * vec4(p2,1.0); fUV = vec2(0,1); EmitVertex();
    gl_Position = u_ViewProjection * vec4(p3,1.0); fUV = vec2(1,1); EmitVertex();

    EndPrimitive();
}


#type fragment
#version 330 core
in vec4 fColor;
in vec2 fUV;

uniform sampler2D u_Texture;

out vec4 outColor;
layout(location = 1) out int o_EntityID;

void main()
{
    vec4 tex = texture(u_Texture, fUV);
    tex=vec4(1.0f);
    outColor = tex * fColor;
    o_EntityID=-1; 
}