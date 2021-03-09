#include "AdvancedShader.h"

GLint AdvancedShader::loadShader(const char* path, GLenum shaderType)
{
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		shaderFile.open(path);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		code = shaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ --> " << path << std::endl;
	}

	const char* shaderCode = code.c_str();

	GLuint shaderHandle;
	shaderHandle = glCreateShader(shaderType);
	glShaderSource(shaderHandle, 1, &shaderCode, NULL);
	glCompileShader(shaderHandle);

	GLint IsCompiled = false;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &IsCompiled);
	if (IsCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shaderHandle, maxLength, &maxLength, &errorLog[0]);
		printf("%s\n Vertex", &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(shaderHandle); // Don't leak the shader.
		exit(-1);
	}

	return shaderHandle;
}

GLint AdvancedShader::getUniformLocation(std::string uniform)
{
	GLint location;
	if (_locations.find(uniform) == _locations.end()) {
		location = glGetUniformLocation(programId, uniform.c_str());
		_locations.insert({ uniform, location });
		return location;
	}
	return _locations.at(uniform);
}

AdvancedShader::AdvancedShader(const char* vertexFile, const char* fragmentFile, const char* geomFile)
{
	_locations = std::unordered_map<string, GLint>();

	GLuint vertexID = loadShader((DIRECTORY + vertexFile).c_str(), GL_VERTEX_SHADER);
	GLuint fragmentID = loadShader((DIRECTORY + fragmentFile).c_str(), GL_FRAGMENT_SHADER);
	GLuint geomID;
	if (geomFile != nullptr) {
		geomID = loadShader((DIRECTORY + geomFile).c_str(), GL_GEOMETRY_SHADER);
	}

	programId = glCreateProgram();
	glAttachShader(programId, vertexID);
	glAttachShader(programId, fragmentID);
	if (geomFile != nullptr) {
		glAttachShader(programId, geomID);
	}

	glLinkProgram(programId);

	GLint isLinked = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);
		printf("%s\n", &infoLog[0]);
		// We don't need the program anymore.
		glDeleteProgram(programId);
		// Don't leak shaders either.
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);

		exit(-1);
	}

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	if (geomFile != nullptr) {
		glDeleteShader(geomID);
	}
}

AdvancedShader::AdvancedShader()
{
}

void AdvancedShader::use() const
{
	glUseProgram(programId);
}

void AdvancedShader::unuse() const
{
	glUseProgram(0);
}

void AdvancedShader::setUniform(std::string uniform, const int i)
{
	setUniform(getUniformLocation(uniform), i);
}

void AdvancedShader::setUniform(GLint location, const int i)
{
	glUniform1i(location, i);
}

void AdvancedShader::setUniform(std::string uniform, const unsigned int i)
{
	setUniform(getUniformLocation(uniform), i);
}

void AdvancedShader::setUniform(GLint location, const unsigned int i)
{
	glUniform1ui(location, i);
}

void AdvancedShader::setUniform(std::string uniform, const float f)
{
	setUniform(getUniformLocation(uniform), f);
}

void AdvancedShader::setUniform(GLint location, const float f)
{
	glUniform1f(location, f);
}

void AdvancedShader::setUniform(std::string uniform, const glm::mat4& mat)
{
	setUniform(getUniformLocation(uniform), mat);
}

void AdvancedShader::setUniform(GLint location, const glm::mat4& mat)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void AdvancedShader::setUniform(std::string uniform, const glm::mat3& mat)
{
	setUniform(getUniformLocation(uniform), mat);
}

void AdvancedShader::setUniform(GLint location, const glm::mat3& mat)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void AdvancedShader::setUniform(std::string uniform, const glm::vec2& vec)
{
	setUniform(getUniformLocation(uniform), vec);
}

void AdvancedShader::setUniform(GLint location, const glm::vec2& vec)
{
	glUniform2fv(location, 1, glm::value_ptr(vec));
}

void AdvancedShader::setUniform(std::string uniform, const glm::vec3& vec)
{
	setUniform(getUniformLocation(uniform), vec);
}

void AdvancedShader::setUniform(GLint location, const glm::vec3& vec)
{
	glUniform3fv(location, 1, glm::value_ptr(vec));
}

void AdvancedShader::setUniform(std::string uniform, const glm::vec4& vec)
{
	setUniform(getUniformLocation(uniform), vec);
}

void AdvancedShader::setUniform(GLint location, const glm::vec4& vec)
{
	glUniform4fv(location, 1, glm::value_ptr(vec));
}

GLuint AdvancedShader::getProgramId()
{
	return programId;
}