#include "ShaderDirectIllumination.h"
#include "Renderer.h"

ShaderDirectIllumination::ShaderDirectIllumination(GLuint shader_mask, const char** shaderPaths) : CVK::ShaderPostProcessing(shader_mask, shaderPaths) { }

void ShaderDirectIllumination::getUniformLocations() {
	CVK::ShaderPostProcessing::getUniformLocations();

	// G-buffer
	m_positionMapID = glGetUniformLocation(m_ProgramID, "positionMap");
	m_normalMapID = glGetUniformLocation(m_ProgramID, "normalMap");
	m_shadowMapID = glGetUniformLocation(m_ProgramID, "shadowMap");

	// TODO attenuation, alpha
	m_cameraPosID = glGetUniformLocation(m_ProgramID, "uniformCameraPos");
	
	m_lightPosID = glGetUniformLocation(m_ProgramID, "uniformLightPos");
	m_lightColorID = glGetUniformLocation(m_ProgramID, "uniformLightColor");
	m_lightMVPID = glGetUniformLocation(m_ProgramID, "uniformLightMVP");
	m_shadowMapBiasID = glGetUniformLocation(m_ProgramID, "uniformShadowMapBias");

	m_worldToVoxelsMatrixID = glGetUniformLocation(m_ProgramID, "uniformWorldToVoxelsMatrix");
	m_voxelDepthID = glGetUniformLocation(m_ProgramID, "uniformVoxelDepth");
	m_voxelResID = glGetUniformLocation(m_ProgramID, "uniformVoxelRes");
}

void ShaderDirectIllumination::update() {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures[0]);
	glUniform1i(m_positionMapID, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_textures[1]);
	glUniform1i(m_normalMapID, 1);

	glActiveTexture(SHADOW_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, m_textures[2]);
	glUniform1i(m_shadowMapID, 2);
	glActiveTexture(GL_TEXTURE0);

	const int offset = 3;
	for (int i = 0; i < Renderer::getInstance()->m_voxelAttributes; i++) {
		glActiveTexture(GL_TEXTURE0 + i + offset);
		glBindTexture(GL_TEXTURE_3D, Renderer::getInstance()->m_voxelTextures[i]);
	}
	glActiveTexture(GL_TEXTURE0);

	glUniform3fv(m_cameraPosID, 1, glm::value_ptr(Renderer::getInstance()->m_camera.getPosition()));

	glm::mat4 projectionLight = glm::mat4(1.0f);
	Renderer::getInstance()->m_cameraLight.getPerspective(&projectionLight);
	glm::mat4 viewLight = glm::mat4(1.0f);
	Renderer::getInstance()->m_cameraLight.getView(&viewLight);
	glm::mat4 shadowMVP = projectionLight * viewLight;
	glUniform3fv(m_lightPosID, 1, glm::value_ptr(Renderer::getInstance()->m_cameraLight.getPosition()));
	glUniform3fv(m_lightColorID, 1, glm::value_ptr(Renderer::getInstance()->tw_lightColor));
	glUniformMatrix4fv(m_lightMVPID, 1, false, glm::value_ptr(shadowMVP));
	glUniform1f(m_shadowMapBiasID, Renderer::getInstance()->tw_shadowMapBias);

	glUniform1i(m_voxelResID, Renderer::getInstance()->m_voxelRes);
	glUniform1i(m_voxelDepthID, Renderer::getInstance()->m_voxelDepth);

	glm::mat4 tmpScale = glm::scale(glm::vec3(Renderer::getInstance()->m_BBScale));
	glm::mat4 tmpTranslate = glm::translate(-Renderer::getInstance()->m_BBMin);
	glm::mat4 worldToVoxelsMatrix = tmpScale * tmpTranslate;
	glUniformMatrix4fv(m_worldToVoxelsMatrixID, 1, false, glm::value_ptr(worldToVoxelsMatrix));
}
