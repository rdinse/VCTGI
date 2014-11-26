#include "ShaderVoxelization.h"
#include "Common.h"
#include "Renderer.h"

ShaderVoxelization::ShaderVoxelization(GLuint shader_mask, const char** shaderNames) : CVK::ShaderMinimal(shader_mask, shaderNames) {
	prepareTextures();
}

void ShaderVoxelization::getUniformLocations() {
	m_modelMatrixID = glGetUniformLocation(m_ProgramID, "uniformModelMatrix");
	m_voxelResID = glGetUniformLocation(m_ProgramID, "uniformVoxelRes");

	//m_modelNormalMatrixID = glGetUniformLocation(m_ProgramID, "uniformModelNormalMatrix");
	m_viewMatrixXID = glGetUniformLocation(m_ProgramID, "uniformViewMatrixX");
	m_viewMatrixYID = glGetUniformLocation(m_ProgramID, "uniformViewMatrixY");
	m_viewMatrixZID = glGetUniformLocation(m_ProgramID, "uniformViewMatrixZ");
	m_projectionMatrixID = glGetUniformLocation(m_ProgramID, "uniformProjectionMatrix");

	m_worldToVoxelsMatrixID = glGetUniformLocation(m_ProgramID, "uniformWorldToVoxelsMatrix");

	m_lightPosID = glGetUniformLocation(m_ProgramID, "uniformLightPos");
	m_lightColorID = glGetUniformLocation(m_ProgramID, "uniformLightColor");
	m_lightMVPID = glGetUniformLocation(m_ProgramID, "uniformLightMVP");
	m_shadowMapID = glGetUniformLocation(m_ProgramID, "shadowMap");
	m_shadowMapBiasID = glGetUniformLocation(m_ProgramID, "uniformShadowMapBias");

	m_diffuseTextureID = glGetUniformLocation(m_ProgramID, "diffuseTexture");
	m_diffuseColorID = glGetUniformLocation(m_ProgramID, "diffuseColor");
	m_useColorTextureID = glGetUniformLocation(m_ProgramID, "useColorTexture");
}

void ShaderVoxelization::prepareTextures() {
	// Delete previously declared textures.
	if (Renderer::getInstance()->m_voxelTextures[0] != INVALID_OGL_VALUE) {
		glDeleteTextures(Renderer::getInstance()->m_voxelAttributes, &Renderer::getInstance()->m_voxelTextures[0]);
	}

	int voxelRes = Renderer::getInstance()->m_voxelRes;
	int voxelDepth = Renderer::getInstance()->m_voxelDepth;
	
	// Create a mipmapped RGBA8 texture for each voxel attribute.
	for (int i = 0; i < Renderer::getInstance()->m_voxelAttributes; i++) {
		glGenTextures(1, &Renderer::getInstance()->m_voxelTextures[i]);
		glBindTexture(GL_TEXTURE_3D, Renderer::getInstance()->m_voxelTextures[i]);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTexStorage3D(GL_TEXTURE_3D, voxelDepth, GL_RGBA8, voxelRes, voxelRes, voxelRes);
	}
}

void ShaderVoxelization::update() {
	float fvoxelRes = Renderer::getInstance()->m_voxelRes;

	glUniform1i(m_voxelResID, Renderer::getInstance()->m_voxelRes);
	
	float BBSize = Renderer::getInstance()->m_BBSize;
	float halfBBSize = Renderer::getInstance()->m_BBSize2;
	glm::vec3 BBCenter = Renderer::getInstance()->m_BBCenter;

	glm::mat4 projectionMatrix = glm::ortho(-halfBBSize, halfBBSize, -halfBBSize, halfBBSize, 0.0f, BBSize);
	glUniformMatrix4fv(m_projectionMatrixID, 1, false, glm::value_ptr(projectionMatrix));
	glm::mat4 viewMatrixX = glm::lookAt(BBCenter + glm::vec3(halfBBSize, 0.0f, 0.0f), BBCenter, glm::vec3(0.0f, 1.0f,  0.0f));
	glm::mat4 viewMatrixY = glm::lookAt(BBCenter + glm::vec3(0.0f, halfBBSize, 0.0f), BBCenter, glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 viewMatrixZ = glm::lookAt(BBCenter + glm::vec3(0.0f, 0.0f, halfBBSize), BBCenter, glm::vec3(0.0f, 1.0f,  0.0f));
	glUniformMatrix4fv(m_viewMatrixXID, 1, false, glm::value_ptr(viewMatrixX));
	glUniformMatrix4fv(m_viewMatrixYID, 1, false, glm::value_ptr(viewMatrixY));
	glUniformMatrix4fv(m_viewMatrixZID, 1, false, glm::value_ptr(viewMatrixZ));

	glm::mat4 tmpScale = glm::scale(glm::vec3(Renderer::getInstance()->m_BBScale));
	glm::mat4 tmpTranslate = glm::translate(-Renderer::getInstance()->m_BBMin);
	glm::mat4 worldToVoxelsMatrix = tmpScale * tmpTranslate;
	glUniformMatrix4fv(m_worldToVoxelsMatrixID, 1, false, glm::value_ptr(worldToVoxelsMatrix));

	glm::mat4 projectionLight = glm::mat4(1.0f);
	Renderer::getInstance()->m_cameraLight.getPerspective(&projectionLight);
	glm::mat4 viewLight = glm::mat4(1.0f);
	Renderer::getInstance()->m_cameraLight.getView(&viewLight);
	glm::mat4 shadowMVP = projectionLight * viewLight;
	glUniform3fv(m_lightPosID, 1, glm::value_ptr(Renderer::getInstance()->m_cameraLight.getPosition()));
	glUniform3fv(m_lightColorID, 1, glm::value_ptr(Renderer::getInstance()->tw_lightColor));
	glUniformMatrix4fv(m_lightMVPID, 1, false, glm::value_ptr(shadowMVP));
	glUniform1f(m_shadowMapBiasID, Renderer::getInstance()->tw_shadowMapBias);

	// Bind voxel attribute as image units.
	for (int i = 0; i < Renderer::getInstance()->m_voxelAttributes; i++) {
		glBindImageTexture(i, Renderer::getInstance()->m_voxelTextures[i], 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);
	}

	// Bind shadow map to texture unit.
	glActiveTexture(SHADOW_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, m_textures[2]);
	glUniform1i(m_shadowMapID, 2);
	glActiveTexture(GL_TEXTURE0);
}

/*void ShaderVoxelization::updateModelMatrix(glm::mat4 modelmatrix)
{
	CVK::ShaderMinimal::updateModelMatrix(modelmatrix);

	glm::mat4 modelnormalmatrix = glm::inverse(glm::transpose(modelmatrix));
	glUniformMatrix4fv(m_modelNormalMatrixID, 1, GL_FALSE, glm::value_ptr(modelnormalmatrix));
}*/

void ShaderVoxelization::update(CVK::Node* node) {
	glUniform1i(m_diffuseTextureID, 0);
	glActiveTexture(COLOR_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, 0);

	if( node->hasMaterial()) {
		CVK::Material* mat = node->getMaterial();

		glm::vec3* diffuseColor = mat->getDiffuse();
		glUniform3f(m_diffuseColorID, diffuseColor->r, diffuseColor->g, diffuseColor->b);

		bool hasDiffuseTexture = mat->hasTexture(CVK::COLOR_TEXTURE);
		glUniform1i(m_useColorTextureID, hasDiffuseTexture);

		if (mat->hasTexture(CVK::COLOR_TEXTURE)) {
			mat->getTexture(CVK::COLOR_TEXTURE)->bind();
		}
	}
}
