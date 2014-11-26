#include "ShaderSSAO.h"

ShaderSSAO::ShaderSSAO( GLuint shader_mask, const char** shaderPaths) : CVK::ShaderPostProcessing(shader_mask, shaderPaths)
{	
	m_positionMapID = glGetUniformLocation( m_ProgramID, "positionMap"); 
	m_normalMapID = glGetUniformLocation( m_ProgramID, "normalMap"); 
	m_projectionMatrixID = glGetUniformLocation(m_ProgramID, "sceneProjectionMatrix");
	m_viewMatrixID = glGetUniformLocation(m_ProgramID, "sceneViewMatrix");
}

void ShaderSSAO::update()
{
	CVK::Camera* cam = CVK::State::getInstance()->getCamera();
	glm::mat4 perspective = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	if (cam != NULL)
	{
		cam->getPerspective(&perspective);
		cam->getView(&view);
	}
	glUniformMatrix4fv(m_projectionMatrixID, 1, GL_FALSE, glm::value_ptr(perspective));
	glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, glm::value_ptr(view));

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
}
