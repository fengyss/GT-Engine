#pragma once
#include <string>
#include "glm/glm.hpp"
#include "GT/Core/Asset/Asset.h"

namespace GT
{
	enum class ShaderType
	{
		Normal,
		Compute,
		Geometry,
		Count
	};
	enum class LightType
	{
		Ambient,
		Point,
		Directional,
		Spot,
		Count
	
	};
	struct DirectionalLight {
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	struct PointLight {
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float constant;
		float linear;
		float quadratic;
	};

	struct SpotLight {
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float cutOff;       // 内锥角
		float outerCutOff;  // 外锥角
	};
	struct Light
	{
		/* ===== 几何 ===== */
		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f); // 向下

		/* ===== 颜色（强度） ===== */
		glm::vec3 ambient = glm::vec3(0.05f);   // 很暗
		glm::vec3 diffuse = glm::vec3(0.8f);    // 主光
		glm::vec3 specular = glm::vec3(1.0f);    // 高光偏白

		/* ===== 点光源衰减 ===== */
		float constant = 1.0f;
		float linear = 0.09f;
		float quadratic = 0.032f;

		/* ===== 聚光灯 ===== */
		float cutOff = glm::cos(glm::radians(12.5f));
		float outerCutOff = glm::cos(glm::radians(17.5f));

		/* ===== 类型 ===== */
		LightType type = LightType::Point;


		PointLight GetPointLight() const {
			PointLight light;
			light.position = pos;
			light.ambient = ambient;
			light.diffuse = diffuse;
			light.specular = specular;
			light.constant = constant;
			light.linear = linear;
			light.quadratic = quadratic;
			return light;
		}

		DirectionalLight GetDirectionalLight() const {
			DirectionalLight light;
			light.direction = direction;
			light.ambient = ambient;
			light.diffuse = diffuse;
			light.specular = specular;
			return light;
		}

		SpotLight GetSpotLight() const {
			SpotLight light;
			light.position = pos;
			light.direction = direction;
			light.ambient = ambient;
			light.diffuse = diffuse;
			light.specular = specular;
			light.cutOff = cutOff;
			light.outerCutOff = outerCutOff;
			return light;
		}
	};
	class Shader : public Asset
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;


		virtual void SetUniform1i(const std::string& name, int v0)  = 0;
		virtual void SetUniform1ui(const std::string& name, unsigned int v0) = 0;
		virtual void SetUniformiv(const std::string& name, int* values, uint32_t count) = 0;

		virtual void SetUniform1f(const std::string& name, float val) = 0;
		virtual void SetUniform2f(const std::string& name, const glm::vec2& val) = 0;
		virtual void SetUniform3f(const std::string& name, const glm::vec3& val) = 0;
		virtual void SetUniform4f(const std::string& name, const glm::vec4& val) = 0;
		virtual void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) = 0;

		virtual void SetUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;


		virtual void SetUniformPointLight(const std::string& name, const PointLight& light) = 0;
		virtual void SetUniformDirectionalLight(const std::string& name, const DirectionalLight& light) = 0;
		virtual void SetUniformSpotLight(const std::string& name, const SpotLight& light) = 0;

		virtual uint32_t GetRendererID() const = 0;
		virtual const std::string& GetName() const = 0;
		virtual const std::filesystem::path& GetPath() const = 0;

		static Ref<Shader> Create(const std::filesystem::path& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> CreateCompute(const std::filesystem::path& filepath);
		static Ref<Shader> CreateGeometry(const std::filesystem::path& filepath);


		static AssetType GetStaticType() { return AssetType::Shader; }

		virtual AssetType _GetType() const { return GetStaticType(); }

	private:
		uint32_t m_RendererID;

	};
	class ShaderLibrary
	{
	public:
		Ref<Shader> Load(uint32_t ID,const std::filesystem::path& filepath);

		Ref<Shader> Get(uint32_t ID);
		Ref<Shader> ShaderLibrary::Reload(uint32_t ID, const std::filesystem::path& filepath);
		void Clear();
		bool Exists(uint32_t ID) const
		{
			return m_Shaders.find(ID) != m_Shaders.end();
		}
	private:
		void Add(uint32_t ID, const Ref<Shader>& shader);
		std::unordered_map<uint32_t, Ref<Shader>> m_Shaders;
	};
}