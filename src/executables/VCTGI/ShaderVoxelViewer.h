#ifndef __SHADER_VOXELVIEWER_H
#define __SHADER_VOXELVIEWER_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_ShaderSet.h>


class ShaderVoxelViewer : public CVK::ShaderSet
{
public:
	ShaderVoxelViewer(GLuint shader_mask, const char** shaderPaths);
	void update();
	void render();
	void getUniformLocations();
	void prepareVoxelGrid();

	GLuint m_voxelGridPointsVAO;
private:
	GLuint m_viewProjectionMatrixID;
	GLuint m_mipLevelID;
	GLuint m_halfVoxelSizeID;
};
#endif /*__SHADER_VOXELVIEWER_H*/
