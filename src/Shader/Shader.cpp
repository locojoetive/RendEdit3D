#include "Shader.h"


// Reads a text file and returns it content as a std::string
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return (contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// Read Vertex and Fragment File and store it in a string
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source file into a character array
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	// Output compile errors
	compileErrors(vertexShader, "VERTEX");

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Atach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);
	// Output compile errors
	compileErrors(fragmentShader, "FRAGMENT");

	// Create Shader Program
	ID = glCreateProgram();
	// Attach Vertex and Fragment Shader to Shader Program
	glAttachShader(ID, vertexShader);
	 glAttachShader(ID, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	// Delete the now useless Vertex and Fragment Shader Object
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader() {}


void Shader::Activate()
{
	// Tell OpenGL which Shader Program we want to use
	glUseProgram(ID);
}

void Shader::Deactivate()
{
	glDeleteProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}


void Shader::SetUniform1f(std::string key, float value)
{
	glUniform1f(glGetUniformLocation(ID, key.c_str()), value);
}
void Shader::SetUniform2f(std::string key, glm::vec2 value)
{
	glUniform2f(glGetUniformLocation(ID, key.c_str()), value.x, value.y);
}
void Shader::SetUniform3f(std::string key, glm::vec3 value)
{
	glUniform3f(glGetUniformLocation(ID, key.c_str()), value.x, value.y, value.z);
}
void Shader::SetUniform4f(std::string key, glm::vec4 value)
{
	glUniform4f(glGetUniformLocation(ID, key.c_str()), value.x, value.y, value.z, value.w);
}


void Shader::compileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for: " << type << "\n" << std::endl;
		}
	}
}