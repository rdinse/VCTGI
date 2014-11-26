#ifndef __SHADER_PREINTEGRATION_H
#define __SHADER_PREINTEGRATION_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_ShaderSet.h>


class ShaderPreIntegration : public CVK::ShaderSet
{
public:
	ShaderPreIntegration(GLuint shader_mask, const char** shaderPaths);
	void getUniformLocations();
	void run();

private:
	GLuint m_sourceMipLevelID;
	GLuint m_destinationMipResID;
};

#endif /*__SHADER_PREINTEGRATION_H*/
