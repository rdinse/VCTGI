#include "CVK_ShaderPostProcessing.h"

CVK::ShaderPostProcessing::ShaderPostProcessing( GLuint shader_mask, const char** shaderPaths) : CVK::ShaderSet(shader_mask, shaderPaths) { }

void CVK::ShaderPostProcessing::getUniformLocations() { };

void CVK::ShaderPostProcessing::update() { }

void CVK::ShaderPostProcessing::render()
{
	m_screenFillingQuad.render();
}

