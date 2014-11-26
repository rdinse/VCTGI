#ifndef __SHADER_COMPOSITING_H
#define __SHADER_COMPOSITING_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_ShaderPostProcessing.h>
#include <CVK_Framework/CVK_State.h>


class ShaderCompositing : public CVK::ShaderPostProcessing
{
public:
	ShaderCompositing(GLuint shader_mask, const char** shaderPaths);
	virtual void update();

private:
	GLuint m_lightMapID, m_colorMapID, m_ssaoMapID; 

};
#endif /*__SHADER_COMPOSITING_H*/