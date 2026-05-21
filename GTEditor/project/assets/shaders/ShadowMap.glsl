#type vertex
#version 460 core
			
layout (location = 0) in vec3 a_Position;

uniform mat4 u_LightSpaceMatrix; // 光源的 VP 矩阵
uniform mat4 u_Model;            // 物体的模型矩阵

void main()
{
    gl_Position = u_LightSpaceMatrix * u_Model * vec4(a_Position, 1.0);
}



#type fragment
#version 460 core
			
void main()
{
    // 将非线性深度转为线性
    gl_FragDepth = gl_FragCoord.z;

}