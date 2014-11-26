#include "ShaderGlobalIllumination.h"
#include "Renderer.h"

ShaderGlobalIllumination::ShaderGlobalIllumination(GLuint shader_mask, const char** shaderPaths) : CVK::ShaderPostProcessing(shader_mask, shaderPaths) { }

void ShaderGlobalIllumination::getUniformLocations() {
	CVK::ShaderPostProcessing::getUniformLocations();

	m_positionMapID = glGetUniformLocation(m_ProgramID, "positionMap");
	m_normalMapID = glGetUniformLocation(m_ProgramID, "normalMap");
	m_tangentMapID = glGetUniformLocation(m_ProgramID, "tangentMap");

	m_eyePositionID = glGetUniformLocation(m_ProgramID, "uniformEyePos");

	m_maxTracingDistanceID = glGetUniformLocation(m_ProgramID, "maxTracingDistance");

	m_ambientOcclusionAlphaID = glGetUniformLocation(m_ProgramID, "uniformAOAlpha");
	m_ambientOcclusionFalloffID = glGetUniformLocation(m_ProgramID, "uniformAOFalloff");

	m_specularAlphaID = glGetUniformLocation(m_ProgramID, "uniformSpecularAlpha");
	m_specularApertureID = glGetUniformLocation(m_ProgramID, "uniformSpecularAperture");
	
	// m_MVPMatrixID = glGetUniformLocation(m_ProgramID, "uniformMVPMatrix");
	m_worldToVoxelsMatrixID = glGetUniformLocation(m_ProgramID, "uniformWorldToVoxelsMatrix");
	m_worldToVoxelsNormalMatrixID = glGetUniformLocation(m_ProgramID, "uniformWorldToVoxelsNormalMatrix");
	m_voxelDepthID = glGetUniformLocation(m_ProgramID, "uniformVoxelDepth");
	m_voxelResID = glGetUniformLocation(m_ProgramID, "uniformVoxelRes");
}

void ShaderGlobalIllumination::update() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures[0]);
	glUniform1i(m_positionMapID, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textures[1]);
	glUniform1i(m_normalMapID, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_textures[2]);
	glUniform1i(m_tangentMapID, 2);

	const int offset = 3;
	for (int i = 0; i < Renderer::getInstance()->m_voxelAttributes; i++) {
		glActiveTexture(GL_TEXTURE0 + i + offset);
		glBindTexture(GL_TEXTURE_3D, Renderer::getInstance()->m_voxelTextures[i]);
	}
	glActiveTexture(GL_TEXTURE0);

	glUniform4fv(m_eyePositionID, 1, glm::value_ptr(Renderer::getInstance()->m_camera.getPosition()));

	glUniform1f(m_maxTracingDistanceID, Renderer::getInstance()->tw_maximalTracingDistance);

	glUniform1f(m_ambientOcclusionAlphaID, Renderer::getInstance()->tw_ambientOcclusionAlpha); 
	glUniform1f(m_ambientOcclusionFalloffID, Renderer::getInstance()->tw_ambientOcclusionFalloff); 
	
	glUniform1f(m_specularAlphaID, Renderer::getInstance()->tw_specularAlpha); 
	glUniform1f(m_specularApertureID, Renderer::getInstance()->tw_specularAperture); 
				
	glUniform1i(m_voxelResID, Renderer::getInstance()->m_voxelRes); 
	glUniform1i(m_voxelDepthID, Renderer::getInstance()->m_voxelDepth); 

	glm::mat4 tmpScale = glm::scale(glm::vec3(Renderer::getInstance()->m_BBScale));
	glm::mat4 tmpTranslate = glm::translate(-Renderer::getInstance()->m_BBMin);
	glm::mat4 worldToVoxelsMatrix = tmpScale * tmpTranslate;
	glUniformMatrix4fv(m_worldToVoxelsMatrixID, 1, false, glm::value_ptr(worldToVoxelsMatrix));
	glm::mat4 worldToVoxelsNormalMatrix = glm::inverse(glm::transpose(worldToVoxelsMatrix));
	glUniformMatrix4fv(m_worldToVoxelsNormalMatrixID, 1, false, glm::value_ptr(worldToVoxelsNormalMatrix));
}
