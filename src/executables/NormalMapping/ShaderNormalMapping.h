#ifndef __SHADERNORMALMAPPING_H
#define __SHADERNORMALMAPPING_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_Camera.h>
#include <CVK_Framework/CVK_ShaderMinimal.h>
#include <CVK_Framework/CVK_Texture.h>


class ShaderNormalMapping : public CVK::ShaderMinimal
{
public:
	ShaderNormalMapping(GLuint shader_mask, const char** ShaderNames);
	void update();
	void update(CVK::Node* node);

private:
	GLuint m_colorTexID, m_normalTexID;
	GLuint m_useColorTexture;
	GLuint m_lightposID, m_lightcolID;
	GLuint m_lightambID;
	GLuint m_diffuseID, m_specularID, m_shininessID;

};
#endif /*__SHADERNORMALMAPPING_H*/