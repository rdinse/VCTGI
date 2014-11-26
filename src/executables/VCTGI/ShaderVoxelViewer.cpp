#include "ShaderVoxelViewer.h"
#include "Renderer.h"

ShaderVoxelViewer::ShaderVoxelViewer(GLuint shader_mask, const char** shaderPaths) :
	CVK::ShaderSet(shader_mask, shaderPaths),
	m_voxelGridPointsVAO(INVALID_OGL_VALUE)
{
	prepareVoxelGrid();
}

void ShaderVoxelViewer::getUniformLocations() {
	m_viewProjectionMatrixID = glGetUniformLocation(m_ProgramID, "uniformViewProjectionMatrix");
	m_mipLevelID = glGetUniformLocation(m_ProgramID, "uniformMipLevel");
	m_halfVoxelSizeID = glGetUniformLocation(m_ProgramID, "uniformHalfVoxelSize");
}

void ShaderVoxelViewer::prepareVoxelGrid() {
	if (m_voxelGridPointsVAO != INVALID_OGL_VALUE) {
		glDeleteVertexArrays(1, &m_voxelGridPointsVAO);
	}

	int voxelCount = Renderer::getInstance()->m_voxelCount;
	int voxelRes = Renderer::getInstance()->m_voxelRes;
	float voxelSize = Renderer::getInstance()->m_voxelSize;
	GLfloat* voxelGridPointData = new GLfloat[voxelCount * 6];

	int voxelIndex = 0;
	for (GLfloat x = 0; x < voxelRes; x++) {
		for (GLfloat y = 0; y < voxelRes; y++) {
			for (GLfloat z = 0; z < voxelRes; z++) {
				glm::vec3 pos = glm::vec3(x * voxelSize, y * voxelSize, z * voxelSize)
					+ glm::vec3(voxelSize/2) + Renderer::getInstance()->m_BBMin;
				glm::vec3 texCoords = glm::vec3(x/((float)voxelRes) + 1/(2*(float)voxelRes),
												y/((float)voxelRes) + 1/(2*(float)voxelRes),
												z/((float)voxelRes) + 1/(2*(float)voxelRes));

				voxelGridPointData[voxelIndex++] = pos.x;
				voxelGridPointData[voxelIndex++] = pos.y;
				voxelGridPointData[voxelIndex++] = pos.z;
				voxelGridPointData[voxelIndex++] = texCoords.x;
				voxelGridPointData[voxelIndex++] = texCoords.y;
				voxelGridPointData[voxelIndex++] = texCoords.z;
			}
		}
	}
	
	int vertexByteSize = 6 * sizeof(GLfloat);
	GLuint voxelGridPointsVBO;
	
	glGenVertexArrays(1, &m_voxelGridPointsVAO);
	glBindVertexArray(m_voxelGridPointsVAO);
	
	glGenBuffers(1, &voxelGridPointsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, voxelGridPointsVBO);
	
	glBufferData(GL_ARRAY_BUFFER, voxelCount * vertexByteSize, voxelGridPointData, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexByteSize, 0);  // Position attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexByteSize, (void *)(3 * sizeof(GLfloat)));  // Texture coordinates attribute

	glBindVertexArray(0);
	glDeleteBuffers(1, &voxelGridPointsVBO);
	delete [] voxelGridPointData;

}

void ShaderVoxelViewer::update() {
	for (int i = 0; i < Renderer::getInstance()->m_voxelAttributes; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_3D, Renderer::getInstance()->m_voxelTextures[i]);
	}
	glActiveTexture(GL_TEXTURE0);

	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	Renderer::getInstance()->m_camera.getPerspective(&projectionMatrix);
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	Renderer::getInstance()->m_camera.getView(&viewMatrix);
	glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
	glUniformMatrix4fv(m_viewProjectionMatrixID, 1, false, glm::value_ptr(viewProjectionMatrix));

	glUniform1i(m_mipLevelID, Renderer::getInstance()->tw_mipLevel);
	glUniform1f(m_halfVoxelSizeID, Renderer::getInstance()->m_voxelSize / 2.0f);
}

void ShaderVoxelViewer::render() {
	if(Renderer::getInstance()->tw_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glBindVertexArray(m_voxelGridPointsVAO);
	glDrawArrays(GL_POINTS, 0, Renderer::getInstance()->m_voxelCount);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
