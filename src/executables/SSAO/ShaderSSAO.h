#ifndef __SHADER_SSAO_H
#define __SHADER_SSAO_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_ShaderPostProcessing.h>
#include <CVK_Framework/CVK_State.h>


class ShaderSSAO : public CVK::ShaderPostProcessing
{
public:
	ShaderSSAO(GLuint shader_mask, const char** shaderPaths);
	virtual void update();

private:
	GLuint m_positionMapID, m_normalMapID; 
	GLuint m_viewMatrixID, m_projectionMatrixID;

};
#endif /*__SHADER_SSAO_H*/