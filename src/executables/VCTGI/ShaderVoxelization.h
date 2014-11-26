#ifndef __SHADER_VOXELIZATION_H
#define __SHADER_VOXELIZATION_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_Camera.h>
#include <CVK_Framework/CVK_ShaderMinimal.h>
#include <CVK_Framework/CVK_Texture.h>
#include <CVK_Framework/CVK_FBO.h>


class ShaderVoxelization : public CVK::ShaderMinimal {
public:
	ShaderVoxelization(GLuint shader_mask, const char** ShaderNames);
	void updateModelMatrix(glm::mat4 modelmatrix);
	void update() override;
	void update(CVK::Node* node) override;
	void getUniformLocations();
	void prepareTextures();

	GLuint m_diffuseColorID;
	GLuint m_alphaID;  // Unused
	GLuint m_shininessID;  // Unused
	GLuint m_specularID;  // Unused
	GLuint m_useColorTextureID;
	GLuint m_diffuseTextureID;

	GLuint m_voxelResID;

	GLuint m_modelNormalMatrixID;  // Model and projection matrices are provided by ShaderMinimal.
	GLuint m_viewMatrixXID;
	GLuint m_viewMatrixYID;
	GLuint m_viewMatrixZID;

	GLuint m_worldToVoxelsMatrixID;

	GLuint m_lightPosID;
	GLuint m_lightColorID;
	GLuint m_lightMVPID;
	GLuint m_shadowMapID;
	GLuint m_shadowMapBiasID;
};
#endif /*__SHADER_VOXELIZATION_H*/
