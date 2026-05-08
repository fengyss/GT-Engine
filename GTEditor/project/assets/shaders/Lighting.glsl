
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

out vec2 v_TexCoord;
out vec3 v_FragPos;
out vec3 v_Normal;
out vec3 v_Tangent;
out vec3 v_Bitangent;
out ivec4 v_m_BoneIDs;
out vec4 v_Weights;



void main()
{
    v_FragPos = (u_Transform * vec4(a_Position,1.0)).rgb;
	gl_Position = u_ViewProjection * vec4(v_FragPos,1.0);

    v_TexCoord = a_TexCoord;
    v_m_BoneIDs = a_m_BoneIDs;
    v_Weights = a_Weights;

    v_Normal = (u_Transform * vec4(a_Normal,0.0)).rgb;
    v_Tangent = (u_Transform * vec4(a_Tangent,0.0)).rgb;
    v_Bitangent = (u_Transform * vec4(a_Bitangent,0.0)).rgb;
}



//#type fragment
#version 460 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

in vec2 v_TexCoord;
in vec3 v_FragPos;
in vec3 v_Normal;
in vec3 v_Tangent;
in vec3 v_Bitangent;
in ivec4 v_m_BoneIDs;
in vec4 v_Weights;

uniform vec3 u_LightPos = vec3(10.0, 10.0, 10.0);
uniform vec3 u_LightColor = vec3(1.0, 1.0, 1.0);


int MAX_TEXTURES = 1; // 根据实际使用的纹理数量调整
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;
uniform sampler2D texture_emission;

uniform uint num_textures;

uniform int u_EntityID;
uniform vec3 u_ViewPos;

// 视差映射函数
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    // 简单视差映射（也可以替换为陡峭视差映射或视差遮挡映射）
    float height = texture(texture_height, texCoords).r;
    vec2 p = viewDir.xy * (height * 0.1); // 0.1 是高度缩放因子，可以调整
    return texCoords - p;
}

void main()
{
    o_EntityID = u_EntityID;

     // 1. 采样基础纹理
     //if(num_textures >> 31 & 1)
    vec4 diffuseColor = texture(texture_diffuse1, v_TexCoord);

    vec4 specularColor = texture(texture_specular1, v_TexCoord);
    vec4 emissionColor = texture(texture_emission1, v_TexCoord);
   

    // 3. 简单光照计算（Blinn-Phong）
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec2 parallaxTexCoords = ParallaxMapping(v_TexCoord, viewDir);
    // 边界检查：防止纹理坐标超出范围
    //if(parallaxTexCoords.x > 1.0 || parallaxTexCoords.y > 1.0 || 
      // parallaxTexCoords.x < -1.0 || parallaxTexCoords.y < -1.0)
        //discard;

     // 2. 处理法线贴图（关键部分）
    vec3 normalFromTexture = texture(texture_normal1, v_TexCoord).rgb;
    
    // 将法线从 [0,1] 转换到 [-1,1] 范围
    normalFromTexture = normalFromTexture*2.0-1.0;
    if(normalFromTexture.r>1.0 || normalFromTexture.g>1.0 || normalFromTexture.b>1.0) discard;

     mat3 TBN = mat3(
        normalize(v_Tangent),
        normalize(v_Bitangent),
        normalize(v_Normal)
    );
    vec3 worldNormal = normalize(TBN * normalFromTexture);
    //worldNormal = v_Normal;
    //worldNormal = normalFromTexture;

    // 环境光
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * u_LightColor;
    
    // 漫反射
    float diff = max(dot(worldNormal, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;
    
    // 镜面反射
    float spec = pow(max(dot(worldNormal, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * specularColor.rgb * u_LightColor;
    
    // 4. 组合结果
    vec3 result = (ambient + diffuse + specular + emissionColor.rgb) * diffuseColor.rgb;
    // 确保 Alpha 不低于某个阈值
    float alpha = diffuseColor.a;
    if (alpha < 0.1) alpha = 1.0;  // 低于 0.1 的设为不透明
    o_Color = vec4(result, alpha);
}