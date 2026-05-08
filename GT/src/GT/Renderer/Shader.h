#pragma once
#include <string>
#include "glm/glm.hpp"

namespace GT
{
	class Shader
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

		virtual uint32_t GetRendererID() const = 0;
		virtual const std::string& GetName() const = 0;
		virtual const std::filesystem::path& GetPath() const = 0;

		static Ref<Shader> Create(std::filesystem::path filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

	private:
		uint32_t m_RendererID;

	};
	class ShaderLibrary
	{
	public:
		Ref<Shader> Load(uint32_t ID,const std::filesystem::path& filepath);

		Ref<Shader> Get(uint32_t ID);
		Ref<Shader> ShaderLibrary::Reload(uint32_t ID, const std::filesystem::path& filepath);
		void Clear() { m_Shaders.clear(); }
		bool Exists(uint32_t ID) const
		{
			return m_Shaders.find(ID) != m_Shaders.end();
		}
	private:
		void Add(uint32_t ID, const Ref<Shader>& shader);
		std::unordered_map<uint32_t, Ref<Shader>> m_Shaders;
	};
}