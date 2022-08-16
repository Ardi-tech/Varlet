#include "OpenGLShader.h"
#include <regex>
#include <fstream>
#include <sstream>

#include "Core/ComponentType.h"

#include "glad/glad.h"

namespace Varlet
{
	std::unordered_map<std::string, Type> OpenGLShader::_types =
	{
		{ "bool",			Type::Bool },
		{ "int",			Type::Int32 },
		{ "uint",			Type::UInt32 },
		{ "float",			Type::Float },
		{ "double",			Type::Double },
		{ "bvec2",			Type::BoolVector2 },
		{ "bvec3",			Type::BoolVector3 },
		{ "bvec4",			Type::BoolVector4 },
		{ "ivec2",			Type::Int32Vector2 },
		{ "ivec3",			Type::Int32Vector3 },
		{ "ivec4",			Type::Int32Vector4 },
		{ "uvec2",			Type::UInt32Vector2 },
		{ "uvec3",			Type::UInt32Vector3 },
		{ "uvec4",			Type::UInt32Vector4 },
		{ "vec2",			Type::Vector2 },
		{ "vec3",			Type::Color3 },
		{ "vec4",			Type::Color4 },
		{ "dvec2",			Type::DoubleVector2 },
		{ "dvec3",			Type::DoubleVector3 },
		{ "dvec4",			Type::DoubleVector4 },
		{ "mat2",			Type::Matrix2 },
		{ "mat3",			Type::Matrix3 },
		{ "mat4",			Type::Matrix4 },
		{ "sampler2D",		Type::Sampler2D },
		{ "samplerCube",	Type::SamplerCube }
	};

	OpenGLShader::OpenGLShader(const ShaderInitializer&
		initializer)
	{
		std::string vertexShaderSource = Load(initializer.vertexPath.c_str());
		std::string fragmentShaderSource = Load(initializer.fragmentPath.c_str());
		std::string geomtryShaderSource = Load(initializer.geomtryPath.c_str());

#if META
		AddUniforms(vertexShaderSource);
		AddUniforms(fragmentShaderSource);
		AddUniforms(geomtryShaderSource);
#endif // META

		uint32_t vertexShaderId = GenerateShader(ShaderType::Vertex, vertexShaderSource.c_str());
		uint32_t fragmentShaderId = GenerateShader(ShaderType::Fragment, fragmentShaderSource.c_str());
		uint32_t geomtryShaderId = GenerateShader(ShaderType::Geometry, geomtryShaderSource.c_str());

		_id = glCreateProgram();

		TryAttach(vertexShaderId);
		TryAttach(fragmentShaderId);
		TryAttach(geomtryShaderId);

		glLinkProgram(_id);
		Compile(_id, ObjectType::Program);

		if (vertexShaderId != 0)
			glDeleteProgram(vertexShaderId);

		if (fragmentShaderId != 0)
			glDeleteProgram(fragmentShaderId);

		if (geomtryShaderId != 0)
			glDeleteProgram(geomtryShaderId);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(_id);
	}

	void OpenGLShader::Use() const
	{
		glUseProgram(_id);
	}

	void OpenGLShader::Compile(const uint32_t& objId, const ObjectType& objType) const
	{
		int32_t isSuccess;
		char errorLog[512];

		if (objType == ObjectType::Program)
		{
			glGetProgramiv(objId, GL_LINK_STATUS, &isSuccess);

			if (isSuccess == 0)
			{
				glGetProgramInfoLog(objId, 512, NULL, errorLog);
				VARLET_LOG(LevelType::Warning, "Shader program link error [program id" + std::to_string(objId) + "] " + errorLog);
			}
		}
		else
		{
			glGetShaderiv(objId, GL_COMPILE_STATUS, &isSuccess);

			if (isSuccess == 0)
			{
				glGetShaderInfoLog(objId, sizeof(errorLog), NULL, errorLog);
				VARLET_LOG(LevelType::Warning, "Shader compile error [shader " + std::to_string(objId) + "] " + errorLog);
			}
		}
	}

	std::string OpenGLShader::Load(const char* path) const
	{
		std::ifstream stream;
		stream.open(path);

		if (stream.is_open())
		{
			std::stringstream buffer;
			buffer << stream.rdbuf();
			return buffer.str();
		}
		else
		{
			VARLET_LOG(LevelType::Warning, "Failed load shader: " + *path);
			return std::string();
		}
	}

	const uint32_t OpenGLShader::GenerateShader(const ShaderType&& type, const char* source) const
	{
		if (source[0] == '\0')
			return 0;

		const uint32_t vertexShaderId = glCreateShader(type);
		glShaderSource(vertexShaderId, 1, &source, NULL);
		glCompileShader(vertexShaderId);

		ObjectType objectType;

		switch (type)
		{
		case ShaderType::Vertex:
			objectType = ObjectType::VertexShader;
			break;

		case ShaderType::Fragment:
			objectType = ObjectType::FragmentShader;
			break;

		case ShaderType::Geometry:
			objectType = ObjectType::GeomertyShader;
			break;

		default:
			return 0;
		}

		Compile(vertexShaderId, objectType);

		return vertexShaderId;
	}

	void OpenGLShader::TryAttach(const uint32_t& id) const
	{
		if (id != 0)
			glAttachShader(_id, id);
	}

	void OpenGLShader::SetBool(const char* name, const bool& value)
	{
		glUniform1i(glGetUniformLocation(_id, name), static_cast<int32_t>(value));
	}

	void OpenGLShader::SetUInt32(const char* name, const uint32_t& value)
	{
		glUniform1ui(glGetUniformLocation(_id, name), value);
	}

	void OpenGLShader::SetInt32(const char* name, const int32_t& value)
	{
		glUniform1i(glGetUniformLocation(_id, name), value);
	}

	void OpenGLShader::SetFloat(const char* name, const float& value)
	{
		glUniform1f(glGetUniformLocation(_id, name), value);
	}

	void OpenGLShader::SetVec2(const char* name, const glm::vec2& value)
	{
		glUniform2f(glGetUniformLocation(_id, name), value.x, value.y);
	}

	void OpenGLShader::SetVec3(const char* name, const glm::vec3& value)
	{
		glUniform3f(glGetUniformLocation(_id, name), value.x, value.y, value.z);
	}

	void OpenGLShader::SetVec4(const char* name, const glm::vec4& value)
	{
		glUniform4f(glGetUniformLocation(_id, name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat3(const char* name, const glm::mat3& value)
	{
		glUniformMatrix3fv(glGetUniformLocation(_id, name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat4(const char* name, const glm::mat4& value)
	{
		glUniformMatrix4fv(glGetUniformLocation(_id, name), 1, GL_FALSE, glm::value_ptr(value));
	}

#ifdef META
	static std::shared_ptr<void> GetValue(const Type& type)
	{
#define MAKE_SHARED(T, Value) std::make_shared<T>(Value);

		switch (type)
		{
			case Type::Bool: return MAKE_SHARED(bool, false);
			case Type::Int32: return MAKE_SHARED(int32_t, 0);
			case Type::UInt32: return MAKE_SHARED(uint32_t, 0);
			case Type::Float: return MAKE_SHARED(float, 0);
			case Type::Double: return MAKE_SHARED(double, 0);
			case Type::BoolVector2: return MAKE_SHARED(glm::bvec2, 1);
			case Type::BoolVector3: return MAKE_SHARED(glm::bvec3, 1);
			case Type::BoolVector4: return MAKE_SHARED(glm::bvec4, 1);
			case Type::Int32Vector2: return MAKE_SHARED(glm::ivec2, 1);
			case Type::Int32Vector3: return MAKE_SHARED(glm::ivec3, 1);
			case Type::Int32Vector4: return MAKE_SHARED(glm::ivec4, 1);
			case Type::UInt32Vector2: return MAKE_SHARED(glm::uvec2, 1);
			case Type::UInt32Vector3: return MAKE_SHARED(glm::uvec3, 1);
			case Type::UInt32Vector4: return MAKE_SHARED(glm::uvec4, 1);
			case Type::Vector2: return MAKE_SHARED(glm::vec2, 1);
			case Type::Vector3:
			case Type::Color3: return MAKE_SHARED(glm::vec3, 1);
			case Type::Vector4:
			case Type::Color4: return MAKE_SHARED(glm::vec4, 1);
			case Type::DoubleVector2: return MAKE_SHARED(glm::dvec2, 1);
			case Type::DoubleVector3: return MAKE_SHARED(glm::dvec3, 1);
			case Type::DoubleVector4: return MAKE_SHARED(glm::dvec4, 1);
			case Type::Matrix2: return MAKE_SHARED(glm::mat2, 1);
			case Type::Matrix3: return MAKE_SHARED(glm::mat3, 1);
			case Type::Matrix4: return MAKE_SHARED(glm::mat4, 1);
			case Type::Sampler2D:
			case Type::SamplerCube:
			default: return std::make_shared<bool>(nullptr);
		}
	}

	void OpenGLShader::AddUniforms(const std::string& source)
	{
		const std::regex reg("uniform\\s+(bool|int|uint|float|double|bvec2|bvec3|bvec4|ivec2|ivec3|ivec4|uvec2|uvec3|uvec4|vec2|vec3|vec4|dvec2|dvec3|dvec4|mat2|mat3|mat4|samplerCube|sampler2D)\\s+(\\w*)", std::regex_constants::ECMAScript);
		std::smatch matches;
		std::string suffix = source;

		while (std::regex_search(suffix, matches, reg))
		{
			const std::string typeName = matches[1].str();

			if (_types.contains(typeName))
			{
				const auto type = _types[typeName];
				uniforms.push_back({ strdup(matches[2].str().c_str()), type, GetValue(type)});
			}

			suffix = matches.suffix();
		}
	}
#endif // META
}
