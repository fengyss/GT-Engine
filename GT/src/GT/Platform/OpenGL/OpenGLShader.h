#pragma once

#include "GT/Renderer/Shader.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};
// TODO: remove!!!
typedef unsigned int GLenum;
namespace GT
{
	class OpenGLShader : public Shader
	{
	private:
		unsigned int m_RendererID = 0;
		std::filesystem::path m_FilePath;
		std::string m_Name;
		std::unordered_map<std::string, int> m_UniformLocationCache;
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::filesystem::path& filepath,ShaderType type = ShaderType::Normal);
		virtual ~OpenGLShader();

		virtual void Bind() const;
		virtual void Unbind() const;


		virtual uint32_t GetRendererID() const override { return m_RendererID; };
		virtual const std::string& GetName() const override { return m_Name; }
		virtual const std::filesystem::path& GetPath() const override { return m_FilePath; }


		virtual void SetUniform1i(const std::string& name, int v0) override;
		virtual void SetUniform1ui(const std::string& name, unsigned int v0) override;
		virtual void SetUniformiv(const std::string& name, int* values, uint32_t count) override;

		// set uniforms
		virtual void SetUniform1f(const std::string& name, float val) override;
		virtual void SetUniform2f(const std::string& name, const glm::vec2& val) override;
		virtual void SetUniform3f(const std::string& name, const glm::vec3& val) override;
		virtual void SetUniform4f(const std::string& name, const glm::vec4& val) override;
		virtual void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) override;


		virtual void SetUniformMat3(const std::string& name, const glm::mat3& matrix) override;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) override;


		virtual void SetUniformPointLight(const std::string& name, const PointLight& light) override;
		virtual void SetUniformDirectionalLight(const std::string& name, const DirectionalLight& light) override;
		virtual void SetUniformSpotLight(const std::string& name, const SpotLight& light) override;

	private:
		unsigned int GetUniformLocation(const std::string& name);

		// read file as binary
		std::string ReadFile(const std::filesystem::path& filepath);
		std::unordered_map<GLenum,std::string> PreProcessShader(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);

		void CompileCompute(const std::string& source);
		void CheckCompileErrors(uint32_t shader, const std::string& type);

		// read file as string
		ShaderProgramSource ParseShader(std::filesystem::path& filepath);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		int CreateShader(const std::string& vertexshader, const std::string& fragmentshader);
	};

}