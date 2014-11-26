#ifndef __CVK_SHADERSET_H
#define __CVK_SHADERSET_H

#include "CVK_Defs.h"
#include "CVK_State.h"

namespace CVK
{

class ShaderSet
{
public:
	ShaderSet();
	ShaderSet( GLuint shader_mask, const char** ShaderNames);
	virtual ~ShaderSet();
	
	virtual void render() {};
	virtual void getUniformLocations() = 0;
	
	void generateShaderProgram(GLuint shader_mask, const char** shaderNames);
	GLuint getProgramID();
	void useProgram();
	
	bool reloadShaders();

    void setTextureInput(int num, GLuint texture);
	void setValue(const char* variableName, float value);

private:
	bool checkShader(GLuint shaderID);
	bool checkProgram(GLuint programID);
	void loadShaderSource(GLint shaderID, const char* fileName);
	
	GLuint m_shader_mask;
	
	GLuint m_vertexShaderID, m_tessControlShaderID, m_tessEvalShaderID;
	GLuint m_geometryShaderID, m_fragmentShaderID, m_computeShaderID;
	
	std::string m_vertexShaderPath, m_tessControlShaderPath, m_tessEvalShaderPath;
	std::string m_geometryShaderPath, m_fragmentShaderPath, m_computeShaderPath;
	
protected:
	GLuint m_ProgramID;
	std::vector<GLuint> m_textures;
};

};

#endif /*__CVK_SHADERSET_H*/
