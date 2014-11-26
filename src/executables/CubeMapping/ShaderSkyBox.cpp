#include "ShaderSkyBox.h"

ShaderSkyBox::ShaderSkyBox( GLuint shader_mask, const char** ShaderNames) : CVK::ShaderMinimal(shader_mask, ShaderNames)
{	
	m_cubeMapID = glGetUniformLocation(m_ProgramID, "cubeMap");
}

void ShaderSkyBox::update()
{
	ShaderMinimal::update();

	glUniform1i( m_cubeMapID, 0);
	glActiveTexture(COLOR_TEXTURE_UNIT);

	CVK::CubeMapTexture* cubeMap = CVK::State::getInstance()->getCubeMapTexture();

	if(cubeMap != 0)
	{
		cubeMap->bind();
	}
}

