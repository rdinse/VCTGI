#ifndef __SHADER_SIMPLE_TEXTURE_H
#define __SHADER_SIMPLE_TEXTURE_H

#include "CVK_Defs.h"
#include "CVK_ShaderPostProcessing.h"
#include "CVK_State.h"

namespace CVK
{

class ShaderSimpleTexture : public CVK::ShaderPostProcessing
{
public:
	ShaderSimpleTexture(GLuint shader_mask, const char** shaderPaths);
	virtual void update();
	void getUniformLocations();

private:
	GLuint m_colorTextureID;

};

};

#endif /*__SHADER_SIMPLE_TEXTURE_H*/
