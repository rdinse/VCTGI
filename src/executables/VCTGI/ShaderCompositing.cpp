#include "ShaderCompositing.h"
#include "Renderer.h"

ShaderCompositing::ShaderCompositing(GLuint shader_mask, const char** shaderPaths) : CVK::ShaderPostProcessing(shader_mask, shaderPaths), m_outputMode(0) { }

void ShaderCompositing::getUniformLocations() {
	CVK::ShaderPostProcessing::getUniformLocations();

	m_outputModeID = glGetUniformLocation(m_ProgramID, "outputMode");
	m_colorMapID = glGetUniformLocation(m_ProgramID, "colorMap");
	m_normalMapID = glGetUniformLocation(m_ProgramID, "normalMap");
	m_depthMapID = glGetUniformLocation(m_ProgramID, "depthMap");
	m_directIlluminationMapID = glGetUniformLocation(m_ProgramID, "directIlluminationMap");
	m_globalIlluminationMapID = glGetUniformLocation(m_ProgramID, "globalIlluminationMap");
	m_ambientOcclusionMapID = glGetUniformLocation(m_ProgramID, "ambientOcclusionMap");
	m_globalIlluminationAlphaID = glGetUniformLocation(m_ProgramID, "uniformGlobalIlluminationAlpha");
	m_ambientOcclusionAlphaID = glGetUniformLocation(m_ProgramID, "uniformAmbientOcclusionAlpha");
}

void ShaderCompositing::update()
{
	glUniform1i(m_colorMapID, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures[0]);
	
	glUniform1i(m_normalMapID, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textures[1]);
	
	glUniform1i(m_depthMapID, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_textures[2]);

	glUniform1i(m_directIlluminationMapID, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_textures[3]);

	glUniform1i(m_globalIlluminationMapID, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_RENDERBUFFER_NV, m_textures[4]);
	glTexRenderbufferNV(GL_TEXTURE_RENDERBUFFER_NV, Renderer::getInstance()->m_globalIlluminationFBO->getTexRenderbuffer(0));

	glUniform1i(m_ambientOcclusionMapID, 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_RENDERBUFFER_NV, m_textures[5]);
	glTexRenderbufferNV(GL_TEXTURE_RENDERBUFFER_NV, Renderer::getInstance()->m_globalIlluminationFBO->getTexRenderbuffer(1));

	glUniform1fv(m_globalIlluminationAlphaID, 1, &Renderer::getInstance()->tw_globalIlluminationAlpha);
	glUniform1fv(m_ambientOcclusionAlphaID, 1, &Renderer::getInstance()->tw_ambientOcclusionAlpha);
	glUniform1iv(m_outputModeID, 1, &m_outputMode);
}

void ShaderCompositing::setOutputMode(int mode) {
	m_outputMode = mode;
}
