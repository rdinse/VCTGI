#ifndef __SHADERGLOBALILLUMINATION_H
#define __SHADERGLOBALILLUMINATION_H

#include "Common.h"
#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_Camera.h>
#include <CVK_Framework/CVK_ShaderPostProcessing.h>
#include <CVK_Framework/CVK_ShaderSet.h>
#include <CVK_Framework/CVK_Texture.h>

#include "SimpleCamera.h"


class ShaderGlobalIllumination : public CVK::ShaderPostProcessing
{
public:
	ShaderGlobalIllumination(GLuint shader_mask, const char** shaderPaths);
	void update() override;
	void getUniformLocations();

private:
	GLuint m_positionMapID;
	GLuint m_normalMapID;
	// GLuint m_colorMapID;
	GLuint m_tangentMapID;

	GLuint m_maxTracingDistanceID;
	
	GLuint m_ambientOcclusionAlphaID;
	GLuint m_ambientOcclusionFalloffID;
	
	GLuint m_specularAlphaID;
	GLuint m_specularApertureID;
		
	GLuint m_voxelResID;
	GLuint m_voxelDepthID;
	
	GLuint m_eyePositionID;
	GLuint m_MVPMatrixID;
	
	GLuint m_worldToVoxelsMatrixID;
	GLuint m_worldToVoxelsNormalMatrixID;
};
#endif /*__SHADERGLOBALILLUMINATION_H */
