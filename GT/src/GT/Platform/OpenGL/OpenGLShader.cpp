#include "gtpch.h"
#include "OpenGLShader.h"
#include "glad/glad.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace GT
{
	GLenum ShaderTypeFromString(const std::string& type)
	{
		if(type== "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		if (type == "geometry")
			return GL_GEOMETRY_SHADER;
		GT_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Name(name)
	{
		GT_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}


	OpenGLShader::~OpenGLShader()
	{
		GT_PROFILE_FUNCTION();
		GLCall(glDeleteProgram(m_RendererID));
	}

	void OpenGLShader::Bind() const
	{
		GT_PROFILE_FUNCTION();
		GLCall(glUseProgram(m_RendererID));
	}

	void OpenGLShader::Unbind() const
	{
		GT_PROFILE_FUNCTION();
		GLCall(glUseProgram(0));
	}


	// read file as binary
	
	OpenGLShader::OpenGLShader(const std::filesystem::path& filepath, ShaderType type)
		:m_FilePath(filepath)
	{
		GT_PROFILE_FUNCTION();
		std::string source = ReadFile(filepath);
		if (source.empty())
		{
			GT_CORE_ASSERT(false,"Creat opengl shader failed {0}!", filepath.string());
		}
		switch (type)
		{
		case ShaderType::Normal:
		{
			auto shaderSources = PreProcessShader(source);
			Compile(shaderSources);
		}
			break;
		case ShaderType::Compute:
			CompileCompute(source);
			break;
		case ShaderType::Geometry:
		{
			auto shaderSources = PreProcessShader(source);
			Compile(shaderSources);
		}
			break;
		default:
			GT_CORE_ASSERT(false, "Unknow ShaderType!");
		};
		

		m_Name = filepath.stem().string();
	}
	
	std::string OpenGLShader::ReadFile(const std::filesystem::path& filepath)
	{
		GT_PROFILE_FUNCTION();
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			GT_CORE_ERROR("Could not open file '{0}'", filepath.string());
		}
		return result;
	}
	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcessShader(const std::string& source)
	{
		GT_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while(pos!= std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			GT_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			GT_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] =
				source.substr(nextLinePos,
					pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}
	void  OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GT_PROFILE_FUNCTION();
		GT_CORE_ASSERT(shaderSources.size() <= 3, "We only support 2 shaders for now");
		std::vector<GLuint> shaderIDs;
		shaderIDs.reserve(shaderSources.size());

		int glShaderIDindex = 0;

		GLuint program = glCreateProgram();
		for (auto kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			const GLchar* sourceCStr = source.c_str();
			GLuint shader = glCreateShader(type);
			glShaderSource(shader, 1, &sourceCStr, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				// We don't need the shader anymore.
				glDeleteShader(shader);
				GT_CORE_ERROR("{0}", infoLog.data());
				GT_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}
			glAttachShader(program, shader);
			shaderIDs.emplace_back(shader);
			//shaderIDs[glShaderIDindex] = shader;
		}


		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for(auto id:shaderIDs)
				glDeleteShader(id);
			GT_CORE_ERROR("{0}", infoLog.data());
			GT_CORE_ASSERT(false, "Shader link failure!");
			return;
		}
		// Always detach shaders after a successful link.
		for (auto id : shaderIDs)
			glDetachShader(program, id);


		m_RendererID = program;
	}


	// read file as string
	//OpenGLShader::OpenGLShader(const std::string& name, const std::string& filepath)
	//	:m_FilePath(filepath), m_Name(name)
	//{
	//	ShaderProgramSource source = ParseShader(filepath);
	//	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	//}

	void OpenGLShader::CompileCompute(const std::string& source)
	{
		m_RendererID = glCreateProgram();
		uint32_t compute = glCreateShader(GL_COMPUTE_SHADER);

		const char* src = source.c_str();
		glShaderSource(compute, 1, &src, nullptr);
		glCompileShader(compute);

		CheckCompileErrors(compute, "COMPUTE");

		glAttachShader(m_RendererID, compute);
		glLinkProgram(m_RendererID);
		CheckCompileErrors(m_RendererID, "PROGRAM");

		glDeleteShader(compute);
	}

	void OpenGLShader::CheckCompileErrors(uint32_t shader, const std::string& type)
	{
		int success;
		char infoLog[1024];

		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
				GT_CORE_ERROR("Shader compilation error ({0}): {1}", type, infoLog);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
				GT_CORE_ERROR("Shader linking error ({0}): {1}", type, infoLog);
			}
		}
	}

	ShaderProgramSource OpenGLShader::ParseShader(std::filesystem::path& filepath)
	{
		GT_PROFILE_FUNCTION();
		std::ifstream stream(filepath);

		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;

		while (getline(stream, line))
		{
			if (line.find("#type") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					type = ShaderType::FRAGMENT;
				}
			}
			else
			{
				if (line.find("#end") != std::string::npos) type = ShaderType::NONE;
				if (type == ShaderType::NONE) continue;
				ss[(int)type] << line << '\n';
			}
		}

		return { ss[0].str(),ss[1].str() };
	}

	unsigned int OpenGLShader::CompileShader(unsigned int type, const std::string& source)
	{
		GT_PROFILE_FUNCTION();
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		// Error handling
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)_alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Failed to compile " <<
				(type == GL_VERTEX_SHADER ? "vertex" : "fragment")
				<< "shader!!" << std::endl;
			std::cout << message << std::endl;
			return 0;
		}

		return id;

	}

	int OpenGLShader::CreateShader(const std::string& vertexshader, const std::string& fragmentshader)
	{
		GT_PROFILE_FUNCTION();
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexshader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentshader);

		glAttachShader(program, vs);
		glAttachShader(program, fs);

		glLinkProgram(program);
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)_alloca(length * sizeof(char));
			glGetProgramInfoLog(program, length, &length, message);
			glDeleteProgram(program);
			glDeleteShader(vs);
			glDeleteShader(fs);
			GT_CORE_ERROR("{0}", message);
			GT_CORE_ASSERT(false, "Shader link failure!");
			return -1;
		}

		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}



	unsigned int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		GT_PROFILE_FUNCTION();
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
		if (location == -1)
			GT_CORE_WARN("Warning: uniform {0} doesn't exist!", name);

		m_UniformLocationCache[name] = location;
		return location;
	}

	//int
	void OpenGLShader::SetUniform1i(const std::string& name, int v0) 
	{
		GT_PROFILE_FUNCTION();
		GLCall(glUniform1i(GetUniformLocation(name), v0));
	}

	void OpenGLShader::SetUniform1ui(const std::string& name, unsigned int v0)
	{
		GT_PROFILE_FUNCTION();
		GLCall(glUniform1ui(GetUniformLocation(name), v0));
	}

	void OpenGLShader::SetUniformiv(const std::string& name, int* values, uint32_t count)
	{
		GT_PROFILE_FUNCTION();
		GLCall(glUniform1iv(GetUniformLocation(name), count, values));
	}

	//vec float
	void OpenGLShader::SetUniform1f(const std::string& name, float val)
	{
		GT_PROFILE_FUNCTION();
		GLCall(glUniform1f(GetUniformLocation(name), val));
	}

	void OpenGLShader::SetUniform2f(const std::string& name, const glm::vec2& val)
	{
		GT_PROFILE_FUNCTION();
		GLCall(glUniform2f(GetUniformLocation(name), val.x, val.y));
	}

	void OpenGLShader::SetUniform3f(const std::string& name, const glm::vec3& val)
	{
		GT_PROFILE_FUNCTION();
		GLCall(glUniform3f(GetUniformLocation(name), val.x, val.y, val.z));
	}

	void OpenGLShader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{
		GT_PROFILE_FUNCTION();
		GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
	}
	void OpenGLShader::SetUniform4f(const std::string& name, const glm::vec4& val)
	{
		GT_PROFILE_FUNCTION();
		GLCall(glUniform4f(GetUniformLocation(name), val.x, val.y, val.z, val.w));
	}

	// matrix
	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GT_PROFILE_FUNCTION();
		GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GT_PROFILE_FUNCTION();
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
	}

	void OpenGLShader::SetUniformPointLight(const std::string& name, const PointLight& light)
	{
		SetUniform3f(name + ".position", light.position);
		SetUniform3f(name + ".ambient", light.ambient);
		SetUniform3f(name + ".diffuse", light.diffuse);
		SetUniform3f(name + ".specular", light.specular);

		SetUniform1f(name + ".constant", light.constant);
		SetUniform1f(name + ".linear", light.linear);
		SetUniform1f(name + ".quadratic", light.quadratic);
	}

	void OpenGLShader::SetUniformDirectionalLight(const std::string& name, const DirectionalLight& light)
	{
		SetUniform3f(name + ".direction", light.direction);
		SetUniform3f(name + ".ambient", light.ambient);
		SetUniform3f(name + ".diffuse", light.diffuse);
		SetUniform3f(name + ".specular", light.specular);
	}

	void OpenGLShader::SetUniformSpotLight(const std::string& name, const SpotLight& light)
	{
		SetUniform3f(name + ".position", light.position);
		SetUniform3f(name + ".direction", light.direction);
		SetUniform3f(name + ".ambient", light.ambient);
		SetUniform3f(name + ".diffuse", light.diffuse);
		SetUniform3f(name + ".specular", light.specular);

		SetUniform1f(name + ".cutOff", light.cutOff);
		SetUniform1f(name + ".outerCutOff", light.outerCutOff);
	}

}