#ifndef __SHADERPHONGSHADOW_H
#define __SHADERPHONGSHADOW_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_Camera.h>
#include <CVK_Framework/CVK_ShaderMinimal.h>
#include <CVK_Framework/CVK_Texture.h>
#include "SimpleCamera.h"


class ShaderPhongShadow : public CVK::ShaderMinimal
{
public:
	ShaderPhongShadow(GLuint shader_mask, const char** ShaderNames);
	void update(SimpleCamera* lightCamera, GLuint shadowMap);
	void update(CVK::Node* node);

private:
	GLuint m_diffuseID, m_specularID, m_shininessID, m_alphaID;
	GLuint m_lightambID;
	GLuint m_lightposID, m_lightcolID, m_lightsdirID, m_lightsexpID, m_lightscutID;

	GLuint m_useColorTexture, m_colorTextureID;
	GLuint m_shadowMapID;

	GLuint m_lightVPBiasID;

	static glm::mat4 sm_lightViewport;

};
#endif /*__SHADERPHONGSHADOW_H*/