#include "ducky_pch.h"
#include "shader.h"
#include "io\fileio.h"
#include "utils\debug.h"
#include "utils\stringUtils.h"

namespace ducky { namespace graphics {

	Shader* Shader::s_CurrentlyBound = nullptr;
	vector<ShaderLibrary> Shader::g_Libraries;
#ifdef DUCKY_GLES;
	bool Shader::g_ParseToGLES = true;
#else
	bool Shader::g_ParseToGLES = false;
#endif

	Shader::Shader(string vertPath, string fragPath, string geomPath)
	{
		m_Program = glCreateProgram();

		ShaderCodeData vertData = ProcessShader(GL_VERTEX_SHADER, io::FileUtils::ReadTextFile(vertPath.c_str()));
		GLuint vert = CreateShader(vertData);
		ShaderCodeData fragData = ProcessShader(GL_FRAGMENT_SHADER, io::FileUtils::ReadTextFile(fragPath.c_str()));
		GLuint frag = CreateShader(fragData);
		GLuint geom = 0;

		glAttachShader(m_Program, vert);
		glAttachShader(m_Program, frag);

#ifndef DUCKY_GLES
		if (geomPath != "")
		{
			ShaderCodeData geomData = ProcessShader(GL_GEOMETRY_SHADER, io::FileUtils::ReadTextFile(geomPath.c_str()));
			geom = CreateShader(geomData);
			glAttachShader(m_Program, geom);
		}
#endif

		glLinkProgram(m_Program);

		GLint res;
		glGetProgramiv(m_Program, GL_LINK_STATUS, &res);
		if (res == GL_NO_ERROR)
		{
			char message[1024];
			glGetProgramInfoLog(m_Program, 1024, NULL, message);
			utils::Debug::Printer << FATAL << "Program linking failed!" << ENDL << message << ENDL;
		}

		glValidateProgram(m_Program);

		glGetProgramiv(m_Program, GL_VALIDATE_STATUS, &res);
		if (res == GL_NO_ERROR)
		{
			char message[1024];
			glGetProgramInfoLog(m_Program, 1024, NULL, message);
			utils::Debug::Printer << FATAL << "Program validation failed!" << ENDL << message << ENDL;
		}

		glDeleteShader(vert);
		glDeleteShader(frag);
		if(geomPath != "")
			glDeleteShader(geom);
	}

	ShaderCodeData Shader::ProcessShader(GLenum type, const string& code)
	{
		ShaderCodeData data;
		data.type = type;
		data.code = code;
#ifndef DUCKY_GLES
		data.libraries.push_back("#version 330 core\n");
#else
		string lib = "#version 120\n";
		if (type == GL_FRAGMENT_SHADER)
		{
			lib += "precision highp float;\n";
			lib += "precision highp vec2;\n";
			lib += "precision highp vec3;\n";
			lib += "precision highp vec4;\n";
			lib += "precision highp mat3;\n";
			lib += "precision highp mat4;\n";
		}
		data.libraries.push_back(lib);
#endif
		for (int i = 0; i < data.code.size(); i++)
		{
			if (memcmp(data.code.c_str() + i, "#include", 8) == 0)
			{
				int ii = i;
				i += 8;
				while (*(data.code.c_str() + i) == ' ')
				{
					i++;
				}
				if (*(data.code.c_str() + i) != '\"' && *(data.code.c_str() + i) != '<')
				{
					Debug::Printer << ERR << "Shader preprocessing failed!" << ENDL;
					Debug::Printer << ERR << "Unknown token after #include" << ENDL;
					continue;
				}
				i++;
				int ni = i;
				while (*(data.code.c_str() + i) != '\"' && *(data.code.c_str() + i) != '>')
				{
					i++;
				}
				string libname = data.code.substr(ni, i - ni);
				string libcode = GetLibrary(libname);
				if (libcode == "")
				{
					Debug::Printer << ERR << "Shader preprocessing failed!" << ENDL;
					Debug::Printer << ERR << "Could not find library " << libname << ENDL;
					data.code.erase(data.code.begin() + ii, data.code.begin() + i + 1);
					i = ii;
					continue;
				}
				data.libraries.push_back(libcode);
				data.code.erase(data.code.begin() + ii, data.code.begin() + i + 1);
				i = ii;
			}
		}
		/*vector<string> s = StringUtils::Tokenize(data.code);
		for(uint i = 0; i < s.size(); i++)
		{
			if (s[i] == "layout")
			{
				if (s[i + 2] == "location")
				{
					ShaderAttribute a;
					a.location = StringUtils::ParseInt(s[i + 4]);

				}
			}
		}*/
		return data;
	}

	GLuint Shader::CreateShader(const ShaderCodeData& data)
	{
		GLuint shader = glCreateShader(data.type);
		GLint l[12];
		const GLchar* c[12];
		uint i = 0;
		for (i = 0; i < data.libraries.size(); i++)
		{
			l[i] = data.libraries[i].size();
			c[i] = data.libraries[i].c_str();
		}
		l[i] = data.code.length();
		c[i] = data.code.c_str();
		glShaderSource(shader, i + 1, c, l);
		glCompileShader(shader);

		GLint res;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
		if (res == GL_NO_ERROR)
		{
			char message[1024];
			glGetShaderInfoLog(shader, 1024, NULL, message);
			utils::Debug::Printer << FATAL << (data.type == GL_VERTEX_SHADER ? "Vertex" : (data.type == GL_FRAGMENT_SHADER ? "Fragment" : "Geometry")) << " shader compilation failed: " << message << ENDL;
			utils::Debug::Assert(false, (data.type == GL_VERTEX_SHADER ? "Vertex shader failed to compile" : (data.type == GL_FRAGMENT_SHADER ? "Fragment shader failed to compile" : "Geometry shader failed to compile")));
			return 0;
		}
		return shader;
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_Program);
	}

	void Shader::Bind()
	{
		if (s_CurrentlyBound == this)
			return;
		s_CurrentlyBound = this;
		glUseProgram(m_Program);
	}

	void Shader::Unbind()
	{
		if (s_CurrentlyBound == nullptr)
			return;
		s_CurrentlyBound = nullptr;
		glUseProgram(0);
	}


	void Shader::SetUniformBuffer(const string& name, Buffer* b)
	{
		Bind();
		GLuint loc = GetUniformBufferLocation(name);
		glUniformBlockBinding(m_Program, loc, loc);
		glBindBufferBase(GL_UNIFORM_BUFFER, loc, b->GetBuffer());
	}

	void Shader::SetUniform1i(const string& name, int val)
	{
		Bind();
		glUniform1i(GetUniformLocation(name), val);
	}

	void Shader::SetUniform1f(const string& name, float val)
	{
		Bind();
		glUniform1f(GetUniformLocation(name), val);
	}

	void Shader::SetUniform2f(const string& name, const glm::vec2& val)
	{
		Bind();
		glUniform2f(GetUniformLocation(name), val.x, val.y);
	}

	void Shader::SetUniform3f(const string& name, const glm::vec3& val)
	{
		Bind();
		glUniform3f(GetUniformLocation(name), val.x, val.y, val.z);
	}

	void Shader::SetUniform4f(const string& name, const glm::vec4& val)
	{
		Bind();
		glUniform4f(GetUniformLocation(name), val.x, val.y, val.z, val.w);
	}

	void Shader::SetUniformMat3(const string& name, const glm::mat3& val)
	{
		Bind();
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &val[0][0]);
	}

	void Shader::SetUniformMat4(const string& name, const glm::mat4& val)
	{
		Bind();
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &val[0][0]);
	}

	void Shader::SetUniform1iv(const string& name, int* vals, int size)
	{
		Bind();
		glUniform1iv(GetUniformLocation(name), size, vals);
	}

	void Shader::SetUniform1fv(const string& name, float* vals, int size)
	{
		Bind();
		glUniform1fv(GetUniformLocation(name), size, vals);
	}

	GLuint Shader::GetUniformLocation(const string& name)
	{
		for (uint i = 0; i < m_UniformsCache.size(); i++)
		{
			if (name == m_UniformsCache[i].name)
			{
				return m_UniformsCache[i].location;
			}
		}
		UniformDesc ud;
		ud.name = name;
		ud.location = glGetUniformLocation(m_Program, name.c_str());
		m_UniformsCache.push_back(ud);
		return ud.location;
	}

	GLuint Shader::GetUniformBufferLocation(const string& name)
	{
		for (uint i = 0; i < m_UniformBuffersCache.size(); i++)
		{
			if (name == m_UniformBuffersCache[i].name)
			{
				return m_UniformBuffersCache[i].location;
			}
		}
		UniformDesc ud;
		ud.name = name;
		ud.location = glGetUniformBlockIndex(m_Program, name.c_str());
		m_UniformBuffersCache.push_back(ud);
		return ud.location;
	}

} }