#include "Shader.h"

Shader::Shader()
{
}

void Shader::init(const std::string& vertFile, const std::string& fragFile)
{
	shaderID = glCreateProgram(); // create shader program (openGL saves as ref number)

	shaders[0] = CreateShader(LoadShader(vertFile), GL_VERTEX_SHADER); // create vertex shader
	shaders[1] = CreateShader(LoadShader(fragFile), GL_FRAGMENT_SHADER); // create fragment shader

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(shaderID, shaders[i]); //add all our shaders to the shader program "shaders" 
	}

	glBindAttribLocation(shaderID, 0, "VertexPosition"); // associate attribute variable with our shader program attribute (in this case attribute vec3 position;)
	glBindAttribLocation(shaderID, 1, "VertexTexCoord");
	glBindAttribLocation(shaderID, 2, "VertexNormal");

	glLinkProgram(shaderID); //create executables that will run on the GPU shaders
	CheckShaderError(shaderID, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // cheack for error

	glValidateProgram(shaderID); //check the entire program is valid
	CheckShaderError(shaderID, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	uniforms[TRANSFORM_U] = glGetUniformLocation(shaderID, "transform"); // associate with the location of uniform variable within a program
}

void Shader::initGeo()
{
	shaderID = glCreateProgram(); // create shader program (openGL saves as ref number)

	shaders1[0] = CreateShader(LoadShader("..\\res\\shaderGeoText.vert"), GL_VERTEX_SHADER); // create vertex shader
	shaders1[1] = CreateShader(LoadShader("..\\res\\shaderGeoText.geom"), GL_GEOMETRY_SHADER); // create fragment shader
	shaders1[2] = CreateShader(LoadShader("..\\res\\shaderGeoText.frag"), GL_FRAGMENT_SHADER); // create fragment shader


	for (unsigned int i = 0; i < 3; i++)
	{
		glAttachShader(shaderID, shaders1[i]); //add all our shaders to the shader program "shaders" 
	}

	glBindAttribLocation(shaderID, 0, "VertexPosition"); // associate attribute variable with our shader program attribute (in this case attribute vec3 position;)
	glBindAttribLocation(shaderID, 1, "VertexTexCoord");
	glBindAttribLocation(shaderID, 2, "VertexNormal");

	glLinkProgram(shaderID); //create executables that will run on the GPU shaders
	CheckShaderError(shaderID, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // cheack for error

	glValidateProgram(shaderID); //check the entire program is valid
	CheckShaderError(shaderID, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	uniforms[TRANSFORM_U] = glGetUniformLocation(shaderID, "transform"); // associate with the location of uniform variable within a program
}




Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(shaderID, shaders[i]); //detach shader from program
		glDeleteShader(shaders[i]); //delete the sahders
	}
	glDeleteProgram(shaderID); // delete the program
}

void Shader::Bind()
{
	glUseProgram(shaderID); //installs the program object specified by program as part of rendering state
}

void Shader::Update(const Transform& transform, const Camera& camera)
{
	glm::mat4 mvp = camera.getViewProjection() * transform.GetModel();
	glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GLU_FALSE, &mvp[0][0]);
}


GLuint Shader::CreateShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type); //create shader based on specified type

	if (shader == 0) //if == 0 shader no created
		std::cerr << "Error type creation failed " << type << std::endl;

	const GLchar* stringSource[1]; //convert strings into list of c-strings
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, stringSource, lengths); //send source code to opengl
	glCompileShader(shader); //get open gl to compile shader code

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //check for compile error

	return shader;
}

std::string Shader::LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

