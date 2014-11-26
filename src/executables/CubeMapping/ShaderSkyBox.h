#ifndef __SHADER_SKY_BOX_H
#define __SHADER_SKY_BOX_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_Camera.h>
#include <CVK_Framework/CVK_ShaderMinimal.h>
#include <CVK_Framework/CVK_Texture.h>


class ShaderSkyBox : public CVK::ShaderMinimal
{
public:
	ShaderSkyBox(GLuint shader_mask, const char** ShaderNames);
	void update();

private:
	GLuint m_cubeMapID;

};
#endif /*__SHADER_SKY_BOX_H*/