#ifndef __SHADER_CUBE_MAP_REFLECTION_H
#define __SHADER_CUBE_MAP_REFLECTION_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_Camera.h>
#include <CVK_Framework/CVK_ShaderMinimal.h>
#include <CVK_Framework/CVK_Texture.h>
#include <CVK_Framework/CVK_Material.h>


class ShaderCubeMapReflection : public CVK::ShaderMinimal
{
public:
	ShaderCubeMapReflection(GLuint shader_mask, const char** ShaderNames);
	void update();
	void update( CVK::Node* node);

private:
	GLuint m_cubeMapID;
	GLuint m_diffuseColorID, m_reflectionStrengthID;
	GLuint m_specularID, m_shininessID;
	GLuint m_colorTextureID, m_useColorTexture;
	GLuint m_lightambID;
	GLuint m_lightposID, m_lightcolID, m_lightsdirID, m_lightsexpID, m_lightscutID;

};
#endif /*__SHADER_CUBE_MAP_REFLECTION_H*/