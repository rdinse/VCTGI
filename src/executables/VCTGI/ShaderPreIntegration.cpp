#include "ShaderPreIntegration.h"
#include "Common.h"
#include "Renderer.h"

ShaderPreIntegration::ShaderPreIntegration(GLuint shader_mask, const char** shaderPaths) : CVK::ShaderSet(shader_mask, shaderPaths) { }

void ShaderPreIntegration::getUniformLocations() {
	m_sourceMipLevelID = glGetUniformLocation(m_ProgramID, "srcMipLevel");
	m_destinationMipResID = glGetUniformLocation(m_ProgramID, "dstMipRes");
}

void ShaderPreIntegration::run() {
	int dstMipRes = Renderer::getInstance()->m_voxelRes / 2;

	for (int i = 0; i < Renderer::getInstance()->m_voxelAttributes; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_3D, Renderer::getInstance()->m_voxelTextures[i]);
	}
	glActiveTexture(GL_TEXTURE0);

	for (int mipLevel = 0; mipLevel < (int)Renderer::getInstance()->m_voxelDepth - 1; mipLevel++) {
		glUniform1i(m_sourceMipLevelID, mipLevel);
		glUniform1i(m_destinationMipResID, dstMipRes);

		for (int i = 0; i < Renderer::getInstance()->m_voxelAttributes; i++) {
			glBindImageTexture(i, Renderer::getInstance()->m_voxelTextures[i], mipLevel + 1, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);
		}

		int numWorkGroups = glm::ceil(dstMipRes / (float) Renderer::getInstance()->tw_computeWorkgroupSize);
		glDispatchCompute(numWorkGroups, numWorkGroups, numWorkGroups);

		glMemoryBarrier(GL_TEXTURE_FETCH_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		dstMipRes /= 2;
	}
}
