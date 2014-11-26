#include "ShaderLighting.h"

ShaderLighting::ShaderLighting( GLuint shader_mask, const char** shaderPaths) : CVK::ShaderMinimal(shader_mask, shaderPaths)
{	
	m_positionMapID = glGetUniformLocation( m_ProgramID, "positionMap"); 
	m_normalMapID = glGetUniformLocation( m_ProgramID, "normalMap"); 
	m_colorMapID = glGetUniformLocation( m_ProgramID, "colorMap"); 
}

void ShaderLighting::update( )
{
	ShaderMinimal::update();

	if (m_textures.size() > 0)
	{
		glUniform1i( m_positionMapID, 0);
		glActiveTexture( GL_TEXTURE0);
		glBindTexture( GL_TEXTURE_2D, m_textures[0]);
	}
	if (m_textures.size() > 1)
	{
		glUniform1i( m_normalMapID, 1);
		glActiveTexture( GL_TEXTURE1);
		glBindTexture( GL_TEXTURE_2D, m_textures[1]);
	}
	if (m_textures.size() > 2)
	{
		glUniform1i( m_colorMapID, 2);
		glActiveTexture( GL_TEXTURE2);
		glBindTexture( GL_TEXTURE_2D, m_textures[2]);
	}
}

void ShaderLighting::update( CVK::Node* node)
{
	
}
