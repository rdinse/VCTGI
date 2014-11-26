#ifndef __SHADER_COMPOSITING_H
#define __SHADER_COMPOSITING_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_ShaderPostProcessing.h>
#include <CVK_Framework/CVK_State.h>


class ShaderCompositing : public CVK::ShaderPostProcessing
{
public:
	ShaderCompositing(GLuint shader_mask, const char** shaderPaths);
	void update() override;
	void getUniformLocations();
	void setOutputMode(int mode);

private:
	int m_outputMode;

	GLuint m_outputModeID;
	GLuint m_colorMapID;
	GLuint m_normalMapID;
	GLuint m_depthMapID;
	GLuint m_directIlluminationMapID;
	GLuint m_globalIlluminationMapID;
	GLuint m_globalIlluminationAlphaID;
	GLuint m_ambientOcclusionMapID;
	GLuint m_ambientOcclusionAlphaID;

};
#endif /*__SHADER_COMPOSITING_H*/
