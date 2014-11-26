#include "ShaderCompositing.h"

ShaderCompositing::ShaderCompositing( GLuint shader_mask, const char** shaderPaths) : CVK::ShaderPostProcessing(shader_mask, shaderPaths)
{	
	m_lightMapID = glGetUniformLocation( m_ProgramID, "lightMap"); 
	m_colorMapID = glGetUniformLocation( m_ProgramID, "colorMap"); 
}

void ShaderCompositing::update()
{
	if (m_textures.size() > 0)
	{
		glUniform1i( m_lightMapID, 0);
		glActiveTexture( GL_TEXTURE0);
		glBindTexture( GL_TEXTURE_2D, m_textures[0]);
	}
	if (m_textures.size() > 1)
	{
		glUniform1i( m_colorMapID, 1);
		glActiveTexture( GL_TEXTURE1);
		glBindTexture( GL_TEXTURE_2D, m_textures[1]);
	}
}
