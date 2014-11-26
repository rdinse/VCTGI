#ifndef __SHADER_LIGHTING_H
#define __SHADER_LIGHTING_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_ShaderMinimal.h>
#include <CVK_Framework/CVK_State.h>


class ShaderLighting : public CVK::ShaderMinimal
{
public:
	ShaderLighting(GLuint shader_mask, const char** shaderPaths);
	void update( );
	void update( CVK::Node* node);

private:
	GLuint m_positionMapID,	m_normalMapID, m_colorMapID; 
};
#endif /*__SHADER_LIGHTING_H*/