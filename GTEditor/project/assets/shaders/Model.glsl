
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
uniform sampler2D texture_diffuse;       //32
uniform sampler2D texture_specular;      //31
uniform sampler2D texture_normal;        //30
uniform sampler2D texture_height; 	     //29
uniform sampler2D texture_emission;      //28

uniform uint u_TexSlot;

uniform int u_EntityID;
uniform vec3 u_ViewPos;

struct Material {
    vec4  ambient;
    vec4  diffuse;
    vec4  specular;
    float shininess;
};

/* ---------- 光源 ---------- */
struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;       // 内锥角
    float outerCutOff;  // 外锥角
};

/* ---------- Uniforms ---------- */
uniform Material u_material;
uniform DirectionalLight u_dirLight;
uniform PointLight u_pointLight;
uniform SpotLight u_spotLight;
uniform uint u_LightSlots = 0;

Material material;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    o_EntityID = u_EntityID;

    vec4 diffuseColor = vec4(1.0f);
    vec4 specularColor = vec4(0.0f);
    vec4 emissionColor = vec4(0.0f);


     // 1. 采样基础纹理
    if(((u_TexSlot >> 31u) & 1u) > 0)
        diffuseColor = texture(texture_diffuse, v_TexCoord);
    if(((u_TexSlot >> 30u) & 1u) > 0)
        specularColor = texture(texture_specular, v_TexCoord);
    if(((u_TexSlot >> 27u) & 1u) > 0)
        emissionColor = texture(texture_emission, v_TexCoord);
   
    material.ambient = vec4(0.2f);
    material.diffuse = diffuseColor;
    material.specular = specularColor;
    material.shininess = 0.2f;


    // 3. 简单光照计算（Blinn-Phong）
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);



     // 2. 处理法线贴图（关键部分）
    vec3 worldNormal = normalize(v_Normal);
    //worldNormal = v_Normal;
    
    if(((u_TexSlot >> 29u) & 1u) > 0)
    {
        worldNormal = texture(texture_normal, v_TexCoord).rgb;
        worldNormal = worldNormal*2.0-1.0;

        mat3 TBN = mat3(
        normalize(v_Tangent),
        normalize(v_Bitangent),
        normalize(v_Normal));

        worldNormal = normalize(TBN * worldNormal);
    }
   

    vec4 result = emissionColor;
    
    if((u_LightSlots & 1u)>0)
        result += vec4(CalcPointLight(u_pointLight, worldNormal, v_FragPos, viewDir),0.0f);
    if((u_LightSlots & 2u)>0)
        result += vec4(CalcDirectionalLight(u_dirLight, worldNormal, viewDir),0.0f);
    if((u_LightSlots & 4u)>0)
        result += vec4(CalcSpotLight(u_spotLight, worldNormal, v_FragPos, viewDir),0.0f);

    result.a = diffuseColor.a;
    o_Color = result;
}

/* ---------- 方向光 ---------- */
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = light.ambient  * material.ambient.rgb;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse.rgb;
    vec3 specular = light.specular * spec * material.specular.rgb;

    return (ambient + diffuse + specular);
}

/* ---------- 点光源 ---------- */
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance    = length(light.position - fragPos);
    float attenuation  = 1.0 /
        (light.constant + light.linear * distance +
         light.quadratic * (distance * distance));

    vec3 ambient  = light.ambient  * material.ambient.rgb;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse.rgb;
    vec3 specular = light.specular * spec * material.specular.rgb;

    return (ambient + diffuse + specular) * attenuation;
}

/* ---------- 聚光灯 ---------- */
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    float theta = dot(lightDir, normalize(-light.direction));
    
    float intensity = smoothstep(light.outerCutOff, light.cutOff, theta);


    vec3 ambient  = light.ambient  * material.ambient.rgb;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse.rgb;
    vec3 specular = light.specular * spec * material.specular.rgb;


    return (ambient + diffuse + specular) * intensity;
}
// point light 
void save()
{
    o_EntityID = u_EntityID;

    vec4 diffuseColor = vec4(1.0f);
    vec4 specularColor = vec4(0.0f);
    vec4 emissionColor = vec4(0.0f);

         // 1. 采样基础纹理
    if(((u_TexSlot >> 31u) & 1u) > 0)
        diffuseColor = texture(texture_diffuse, v_TexCoord);
    if(((u_TexSlot >> 30u) & 1u) > 0)
        specularColor = texture(texture_specular, v_TexCoord);
    if(((u_TexSlot >> 27u) & 1u) > 0)
        emissionColor = texture(texture_emission, v_TexCoord);


     // 3. 简单光照计算（Blinn-Phong）
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);



     // 2. 处理法线贴图（关键部分）
    vec3 worldNormal = normalize(v_Normal);
    //worldNormal = v_Normal;
    
    if(((u_TexSlot >> 29u) & 1u) > 0)
    {
        worldNormal = texture(texture_normal, v_TexCoord).rgb;
        worldNormal = worldNormal*2.0-1.0;

        mat3 TBN = mat3(
        normalize(v_Tangent),
        normalize(v_Bitangent),
        normalize(v_Normal));

        worldNormal = normalize(TBN * worldNormal);
    }

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