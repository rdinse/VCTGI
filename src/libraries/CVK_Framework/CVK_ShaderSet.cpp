#include <string>
#include <fstream>

#include "CVK_ShaderSet.h"

CVK::ShaderSet::ShaderSet() { }

// ShaderNames must be in order Vertx, Tess_Control, Tess_Eval, Geometry, Fragment, Compute
CVK::ShaderSet::ShaderSet(GLuint shader_mask, const char** shaderPaths)
{
	generateShaderProgram(shader_mask, shaderPaths);
}

GLuint CVK::ShaderSet::getProgramID()
{
	return( m_ProgramID);
}

void CVK::ShaderSet::useProgram()
{
	glUseProgram(m_ProgramID);
}

CVK::ShaderSet::~ShaderSet()
{
	if (m_ProgramID != INVALID_OGL_VALUE) glDeleteProgram( m_ProgramID);
}

void CVK::ShaderSet::setTextureInput(int num, GLuint texture)
{
	if(num >= m_textures.size())
		m_textures.resize(num+1);
	m_textures[num] = texture;
}

void CVK::ShaderSet::setValue(const char* variableName, float value)
{
	GLuint variableID = glGetUniformLocation(m_ProgramID, variableName);
	glUniform1f(variableID, value);
}

void CVK::ShaderSet::generateShaderProgram(GLuint shader_mask, const char** shaderPaths)
{
	m_vertexShaderID = m_tessControlShaderID = m_tessEvalShaderID = INVALID_OGL_VALUE;
	m_geometryShaderID = m_fragmentShaderID = m_computeShaderID = INVALID_OGL_VALUE;
	
	m_shader_mask = shader_mask;
	
	m_ProgramID = glCreateProgram();

	int next_name = 0;

	if (m_shader_mask & VERTEX_SHADER_BIT)
	{
		m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		m_vertexShaderPath = shaderPaths[next_name++];
		loadShaderSource(m_vertexShaderID, m_vertexShaderPath.c_str());
		glCompileShader(m_vertexShaderID);
		checkShader(m_vertexShaderID);
	}

	if (m_shader_mask & TESS_CONTROL_BIT)
	{
		m_tessControlShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
		m_tessControlShaderPath = shaderPaths[next_name++];
		loadShaderSource(m_tessControlShaderID, m_tessControlShaderPath.c_str());
		glCompileShader(m_tessControlShaderID);
		checkShader(m_tessControlShaderID);
	}

	if (m_shader_mask & TESS_EVAL_BIT)
	{
		m_tessEvalShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
		m_tessEvalShaderPath = shaderPaths[next_name++];
		loadShaderSource(m_tessEvalShaderID, m_tessEvalShaderPath.c_str());
		glCompileShader(m_tessEvalShaderID);
		checkShader(m_tessEvalShaderID);
	}

	if (m_shader_mask & GEOMETRY_SHADER_BIT)
	{
		m_geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		m_geometryShaderPath = shaderPaths[next_name++];
		loadShaderSource(m_geometryShaderID, m_geometryShaderPath.c_str());
		glCompileShader(m_geometryShaderID);
		checkShader(m_geometryShaderID);
	}

	if (m_shader_mask & FRAGMENT_SHADER_BIT)
	{    
		m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		m_fragmentShaderPath = shaderPaths[next_name++];
		loadShaderSource(m_fragmentShaderID, m_fragmentShaderPath.c_str());
		glCompileShader(m_fragmentShaderID);
		checkShader(m_fragmentShaderID);
	}

	if (m_shader_mask & COMPUTE_SHADER_BIT)
	{
		m_computeShaderID = glCreateShader(GL_COMPUTE_SHADER);
		m_computeShaderPath = shaderPaths[next_name++];
		loadShaderSource(m_computeShaderID, m_computeShaderPath.c_str());
		glCompileShader(m_computeShaderID);
		checkShader(m_computeShaderID);
	}

	if (m_shader_mask & VERTEX_SHADER_BIT) glAttachShader(m_ProgramID, m_vertexShaderID);
	if (m_shader_mask & TESS_CONTROL_BIT) glAttachShader(m_ProgramID, m_tessControlShaderID);
	if (m_shader_mask & TESS_EVAL_BIT) glAttachShader(m_ProgramID, m_tessEvalShaderID);
	if (m_shader_mask & GEOMETRY_SHADER_BIT) glAttachShader(m_ProgramID, m_geometryShaderID);
	if (m_shader_mask & FRAGMENT_SHADER_BIT) glAttachShader(m_ProgramID, m_fragmentShaderID);
	if (m_shader_mask & COMPUTE_SHADER_BIT) glAttachShader(m_ProgramID, m_computeShaderID);

	glLinkProgram(m_ProgramID);

	GLint infoLogLength;
    glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 1) {
		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(m_ProgramID, infoLogLength, NULL, infoLog);

		std::cout << "Info log:\n" << infoLog << std::endl;

		delete[] infoLog;
    }

    checkProgram(m_ProgramID);
}

bool CVK::ShaderSet::reloadShaders() {
	GLuint tmp_vertexShaderID, tmp_tessControlShaderID, tmp_tessEvalShaderID;
	GLuint tmp_geometryShaderID, tmp_fragmentShaderID, tmp_computeShaderID;

	tmp_vertexShaderID = tmp_tessControlShaderID = tmp_tessEvalShaderID = INVALID_OGL_VALUE;
	tmp_geometryShaderID = tmp_fragmentShaderID = tmp_computeShaderID = INVALID_OGL_VALUE;

	int next_name = 0;
	bool noError = true;

	if (m_shader_mask & VERTEX_SHADER_BIT)
	{
		tmp_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		loadShaderSource(tmp_vertexShaderID, m_vertexShaderPath.c_str());
		glCompileShader(tmp_vertexShaderID);
		noError &= checkShader(tmp_vertexShaderID);
	}

	if (m_shader_mask & TESS_CONTROL_BIT)
	{
		tmp_tessControlShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
		loadShaderSource(tmp_tessControlShaderID, m_tessControlShaderPath.c_str());
		glCompileShader(tmp_tessControlShaderID);
		noError &= checkShader(tmp_tessControlShaderID);
	}

	if (m_shader_mask & TESS_EVAL_BIT)
	{
		tmp_tessEvalShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
		loadShaderSource(tmp_tessEvalShaderID, m_tessEvalShaderPath.c_str());
		glCompileShader(tmp_tessEvalShaderID);
		noError &= checkShader(tmp_tessEvalShaderID);
	}

	if (m_shader_mask & GEOMETRY_SHADER_BIT)
	{
		tmp_geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		loadShaderSource(tmp_geometryShaderID, m_geometryShaderPath.c_str());
		glCompileShader(tmp_geometryShaderID);
		noError &= checkShader(tmp_geometryShaderID);
	}

	if (m_shader_mask & FRAGMENT_SHADER_BIT)
	{
		tmp_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		loadShaderSource(tmp_fragmentShaderID, m_fragmentShaderPath.c_str());
		glCompileShader(tmp_fragmentShaderID);
		noError &= checkShader(tmp_fragmentShaderID);
	}

	if (m_shader_mask & COMPUTE_SHADER_BIT)
	{
		tmp_computeShaderID = glCreateShader(GL_COMPUTE_SHADER);
		loadShaderSource(tmp_computeShaderID, m_computeShaderPath.c_str());
		glCompileShader(tmp_computeShaderID);
		noError &= checkShader(tmp_computeShaderID);
	}

	if (noError) {

		if (m_shader_mask & VERTEX_SHADER_BIT) {
			glDetachShader(m_ProgramID, m_vertexShaderID);
			glDeleteShader(m_vertexShaderID);
			glAttachShader(m_ProgramID, tmp_vertexShaderID);
			m_vertexShaderID = tmp_vertexShaderID;
		}
		if (m_shader_mask & TESS_CONTROL_BIT) {
			glDetachShader(m_ProgramID, m_tessControlShaderID);
			glDeleteShader(m_tessControlShaderID);
			glAttachShader(m_ProgramID, tmp_tessControlShaderID);
			m_tessControlShaderID = tmp_tessControlShaderID;
		}
		if (m_shader_mask & TESS_EVAL_BIT) {
			glDetachShader(m_ProgramID, m_tessEvalShaderID);
			glDeleteShader(m_tessEvalShaderID);
			glAttachShader(m_ProgramID, tmp_tessEvalShaderID);
			m_tessEvalShaderID = tmp_tessEvalShaderID;
		}
		if (m_shader_mask & GEOMETRY_SHADER_BIT) {
			glDetachShader(m_ProgramID, m_geometryShaderID);
			glDeleteShader(m_geometryShaderID);
			glAttachShader(m_ProgramID, tmp_geometryShaderID);
			m_geometryShaderID = tmp_geometryShaderID;
		}
		if (m_shader_mask & FRAGMENT_SHADER_BIT) {
			glDetachShader(m_ProgramID, m_fragmentShaderID);
			glDeleteShader(m_fragmentShaderID);
			glAttachShader(m_ProgramID, tmp_fragmentShaderID);
			m_fragmentShaderID = tmp_fragmentShaderID;
		}
		if (m_shader_mask & COMPUTE_SHADER_BIT) {
			glDetachShader(m_ProgramID, m_computeShaderID);
			glDeleteShader(m_computeShaderID);
			glAttachShader(m_ProgramID, tmp_computeShaderID);
			m_computeShaderID = tmp_computeShaderID;
		}

		glLinkProgram(m_ProgramID);

		if (checkProgram(m_ProgramID)) {
			return true;
		} else {
			std::cerr << "Fatal error: Linking not successful." << std::endl;
			return false;
		}
	} else {
		return false;
	}
}

//checks a shader for compiler errors
bool CVK::ShaderSet::checkShader(GLuint shaderID) {
    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar* infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, infoLog);

        std::cerr << "ERROR: Unable to compile shader" << std::endl << infoLog << std::endl;
		delete[] infoLog;
		return false;
    } else {
        std::cout << "SUCCESS: Shader compiled" << std::endl;
        return true;
    }
}

//checks a program
bool CVK::ShaderSet::checkProgram(GLuint programID) {
    GLint status;
    glGetProgramiv(programID, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {

		GLint infoLogLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar* infoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(programID, infoLogLength, NULL, infoLog);

        std::cout << "ERROR: Unable to link shader set. See errors above." << infoLog << std::endl;
        // delete[] infoLog;
        return false;
    } else {
        std::cout << "SUCCESS: ShaderSet linked." << std::endl;
        return true;
    }
}

//reads a file and returns the content as a pointer to chars
void CVK::ShaderSet::loadShaderSource(GLint shaderID, const char* fileName) {
    std::string fileContent;
    std::string line;

    //open file and "parse" input
    std::ifstream file(fileName);
    if (file.is_open()) {
        while (!file.eof()){
            getline (file, line);
            fileContent += line + "\n";
        }
        file.close();
        std::cout << "SUCCESS: Opened file " << fileName << std::endl;
    }
    else
        std::cout << "ERROR: Unable to open file " << fileName << std::endl;

    const char* source = fileContent.c_str();
    const GLint source_size = strlen(source);
    
    glShaderSource(shaderID, 1, &source, &source_size);
}
