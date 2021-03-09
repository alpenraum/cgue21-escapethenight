#pragma once
#include "utils/Settings.h"
#include "utils/Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
/*!
* A shader manager that is able to work with geometry shaders
*/
class AdvancedShader
{
private:
	const string DIRECTORY = "assets/shader/";
	std::unordered_map<std::string, GLint> _locations;
	GLuint programId;

	/*!
	* creates a Shader and returns the handle
	* @param path: the name of the shader file
	* @param shaderType: the type of the shader (GL_VERTEX_SHADER,GL_FRAGMENT_SHADER,GL_GEOMETRY_SHADER)
	*/
	GLint loadShader(const char* path, GLenum shaderType);

public:

	AdvancedShader(const char* vertexFile, const char* fragmentFile, const char* geomFile = nullptr);
	AdvancedShader();

	/*!
	* finds the uniform location, saves it in _locations (if not present) and returns the value;
	 * @param uniform: uniform string in shader
	 * @return the location ID of the uniform
	 */
	GLint getUniformLocation(std::string uniform);

	/*!
	 * Uses the shader with glUseProgram
	 */
	void use() const;

	/*!
	 * Un-uses the shader
	 */
	void unuse() const;

	/*!
	 * Sets an integer uniform in the shader
	 * @param uniform: the name of the uniform
	 * @param i: the value to be set
	 */
	void setUniform(std::string uniform, const int i);
	/*!
	 * Sets an integer uniform in the shader
	 * @param location: location ID of the uniform
	 * @param i: the value to be set
	 */
	void setUniform(GLint location, const int i);
	/*!
	 * Sets an unsigned integer uniform in the shader
	 * @param uniform: the name of the uniform
	 * @param i: the value to be set
	 */
	void setUniform(std::string uniform, const unsigned int i);
	/*!
	 * Sets an unsigned integer uniform in the shader
	 * @param location: location ID of the uniform
	 * @param i: the value to be set
	 */
	void setUniform(GLint location, const unsigned int i);
	/*!
	 * Sets a float uniform in the shader
	 * @param uniform: the name of the uniform
	 * @param f: the value to be set
	 */
	void setUniform(std::string uniform, const float f);
	/*!
	 * Sets a float uniform in the shader
	 * @param location: location ID of the uniform
	 * @param f: the value to be set
	 */
	void setUniform(GLint location, const float f);
	/*!
	 * Sets a 4x4 matrix uniform in the shader
	 * @param uniform: the name of the uniform
	 * @param mat: the value to be set
	 */
	void setUniform(std::string uniform, const glm::mat4& mat);
	/*!
	 * Sets a 4x4 matrix uniform in the shader
	 * @param location: location ID of the uniform
	 * @param mat: the value to be set
	 */
	void setUniform(GLint location, const glm::mat4& mat);
	/*!
	 * Sets a 3x3 matrix uniform in the shader
	 * @param uniform: the name of the uniform
	 * @param mat: the value to be set
	 */
	void setUniform(std::string uniform, const glm::mat3& mat);
	/*!
	 * Sets a 3x3 matrix uniform in the shader
	 * @param location: location ID of the uniform
	 * @param mat: the value to be set
	 */
	void setUniform(GLint location, const glm::mat3& mat);
	/*!
	 * Sets a 2D vector uniform in the shader
	 * @param uniform: the name of the uniform
	 * @param vec: the value to be set
	 */
	void setUniform(std::string uniform, const glm::vec2& vec);
	/*!
	 * Sets a 2D vector uniform in the shader
	 * @param location: location ID of the uniform
	 * @param vec: the value to be set
	 */
	void setUniform(GLint location, const glm::vec2& vec);
	/*!
	 * Sets a 3D vector uniform in the shader
	 * @param uniform: the name of the uniform
	 * @param vec: the value to be set
	 */
	void setUniform(std::string uniform, const glm::vec3& vec);
	/*!
	 * Sets a 3D vector uniform in the shader
	 * @param location: location ID of the uniform
	 * @param vec: the value to be set
	 */
	void setUniform(GLint location, const glm::vec3& vec);
	/*!
	 * Sets a 4D vector uniform in the shader
	 * @param uniform: the name of the uniform
	 * @param vec: the value to be set
	 */
	void setUniform(std::string uniform, const glm::vec4& vec);
	/*!
	 * Sets a 4D vector uniform in the shader
	 * @param location: location ID of the uniform
	 * @param vec: the value to be set
	 */
	void setUniform(GLint location, const glm::vec4& vec);

	GLuint getProgramId();
};
