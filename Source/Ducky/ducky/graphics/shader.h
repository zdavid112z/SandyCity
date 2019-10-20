#pragma once

#include "ducky_pch.h"
#include "opengl.h"
#include "glm\glm.hpp"
#include "buffer.h"
#include "utils\debug.h"

namespace ducky { namespace graphics {

	enum ShaderVariableType
	{
		SHADER_INT,
		SHADER_FLOAT,
		SHADER_VEC2,
		SHADER_VEC3,
		SHADER_VEC4,
		SHADER_MAT3,
		SHADER_MAT4
	};

	struct ShaderCodeData
	{
		GLuint type;
		string code;
		vector<string> libraries;
	};
	
	struct ShaderLibrary
	{
		ShaderLibrary(const string& n, const string& c) :
			name(n),
			code(c) {}
		string name;
		string code;
	};

	struct ShaderAttribute
	{
		string name;
		int location;
		ShaderVariableType type;
	};

	struct ShaderVariable
	{
		string name;
		ShaderVariableType type;
	};

	struct ShaderStruct
	{
		vector<ShaderVariable> uniforms;
		string name;
	};

	struct MVPUniformBuffer
	{
		glm::mat4 mvp;
		glm::mat4 model;
		glm::vec4 camPosition;
	};

	struct UniformDesc
	{
		string name;
		GLuint location;
	};

	class Shader
	{
	private:
		static vector<ShaderLibrary> g_Libraries;
		static bool g_ParseToGLES;
	public:
		static string GetLibrary(const string& name)
		{
			for (ShaderLibrary s : g_Libraries)
			{
				if (s.name == name)
					return s.code;
			}
			Debug::Printer << WARN << "Could not find shader library " << name << ENDL;
			return "";
		}
		static void AddLibrary(const string& name, const string& code)
		{
			g_Libraries.push_back(ShaderLibrary(name, code));
		}
		static Shader* s_CurrentlyBound;
	public:
		Shader(string vertPath, string fragPath, string geomPath = "");
		~Shader();
		void Bind();
		static void Unbind();

		void SetUniformBuffer(const string& name, Buffer*);

		void SetUniform1i(const string& name, int val);
		void SetUniform1iv(const string& name, int* vals, int size);
		void SetUniform1f(const string& name, float val);
		void SetUniform1fv(const string& name, float* vals, int size);
		void SetUniform2f(const string& name, const glm::vec2& val);
		void SetUniform3f(const string& name, const glm::vec3& val);
		void SetUniform4f(const string& name, const glm::vec4& val);
		void SetUniformMat3(const string& name, const glm::mat3& val);
		void SetUniformMat4(const string& name, const glm::mat4& val);
	protected:
		GLuint GetUniformLocation(const string& name);
		GLuint GetUniformBufferLocation(const string& name);
		GLuint CreateShader(const ShaderCodeData&);
		ShaderCodeData ProcessShader(GLenum type, const string& code);
	protected:
		GLuint m_Program;
		vector<UniformDesc> m_UniformBuffersCache;
		vector<UniformDesc> m_UniformsCache;

		vector<ShaderStruct> m_UniformBuffers;
		vector<ShaderStruct> m_VaryingBlocks;
		vector<ShaderAttribute> m_Attributes;
	};

} }