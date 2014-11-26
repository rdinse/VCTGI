#ifndef __SHADERDIRECTILLUMINATION_H
#define __SHADERDIRECTILLUMINATION_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_Node.h>
#include <CVK_Framework/CVK_Camera.h>
#include <CVK_Framework/CVK_ShaderPostProcessing.h>
#include <CVK_Framework/CVK_Texture.h>
#include "SimpleCamera.h"


class ShaderDirectIllumination : public CVK::ShaderPostProcessing
{
public:
	ShaderDirectIllumination(GLuint shader_mask, const char** shaderPaths);
	void update() override;
	void getUniformLocations();

private:
	// GLuint m_shininessID;

	GLuint m_lightPosID;
	GLuint m_lightColorID;
	GLuint m_lightMVPID;
	GLuint m_shadowMapBiasID;
	GLuint m_shadowMapID;

	GLuint m_cameraPosID;

	GLuint m_positionMapID;
	GLuint m_normalMapID;

	GLuint m_worldToVoxelsMatrixID;
	GLuint m_voxelResID;
	GLuint m_voxelDepthID;

};
#endif /*__SHADERDIRECTILLUMINATION_H */
