#ifndef __SHADER_GBUFFER_H
#define __SHADER_GBUFFER_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_Camera.h>
#include <CVK_Framework/CVK_ShaderMinimal.h>
#include <CVK_Framework/CVK_Texture.h>


class ShaderGBuffer : public CVK::ShaderMinimal
{
public:
	ShaderGBuffer(GLuint shader_mask, const char** ShaderNames);
	void update() override;
	void update(CVK::Node* node) override;
	void getUniformLocations() override;
	void resetModelMatrix();

	GLuint m_useColorTextureID, m_colorTexID;
	GLuint m_diffuseColorID, m_alphaID;
};
#endif /*__SHADER_GBUFFER_H*/
