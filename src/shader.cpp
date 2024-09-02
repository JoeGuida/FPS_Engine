#include "shader.hpp"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	// Retrieve vertex/fragment shader source code from filePath
	std::string vertex_shader_code, fragment_shader_code;
	std::ifstream vertex_shader_file, fragment_shader_file;
	std::stringstream vertex_shader_stream, fragment_shader_stream;

	// Ensure ifstream objects can throw exceptions
	vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// open files
		vertex_shader_file.open(vertexPath);
		fragment_shader_file.open(fragmentPath);

		// read file's buffer contents into streams
		vertex_shader_stream << vertex_shader_file.rdbuf();
		fragment_shader_stream << fragment_shader_file.rdbuf();

		// close file handlers
		vertex_shader_file.close();
		fragment_shader_file.close();

		// convert stream into string
		vertex_shader_code = vertex_shader_stream.str();
		fragment_shader_code = fragment_shader_stream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* v_shader_code = vertex_shader_code.c_str();
	const char* f_shader_code = fragment_shader_code.c_str();

	// Compile shaders
	unsigned int vertex, fragment;
	int success;
	char info_log[512];

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &v_shader_code, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	// fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &f_shader_code, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	// shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::set_uniform(const char* uniformName, float value) {
	use();
	glUniform1f(glGetUniformLocation(ID, uniformName), value);
}

void Shader::set_uniform(const char* uniformName, double value) {
	use();
	glUniform1f(glGetUniformLocation(ID, uniformName), value);
}

void Shader::set_uniform(const char* uniformName, int value) {
	use();
	glUniform1i(glGetUniformLocation(ID, uniformName), value);
}

void Shader::set_uniform(const char* uniformName, const glm::mat4& value) {
	use();
	glUniformMatrix4fv(glGetUniformLocation(ID, uniformName), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_uniform(const char* uniformName, const glm::vec3& value) {
	use();
	glUniform3f(glGetUniformLocation(ID, uniformName), value.x, value.y, value.z);
}

void Shader::use() {
	glUseProgram(ID);
}