#ifndef __CVK_SHADERPHONG_H
#define __CVK_SHADERPHONG_H

#include "CVK_Defs.h"
#include "CVK_Camera.h"
#include "CVK_Light.h"
#include "CVK_Material.h"
#include "CVK_Geometry.h"
#include "CVK_ShaderMinimal.h"

namespace CVK
{

class ShaderPhong : public CVK::ShaderMinimal
{
public:
	ShaderPhong( GLuint shader_mask, const char** shaderPaths);
	void update( );
	void update( CVK::Node* node);
	void getUniformLocations();

private:
	GLuint m_diffuseID, m_specularID, m_shininessID, m_alphaID;
	GLuint m_lightambID;
	GLuint m_lightposID, m_lightcolID, m_lightsdirID, m_lightsexpID, m_lightscutID;

	GLuint m_useColorTexture, m_colorTextureID;

	GLuint m_fogcolID, m_fogstartID, m_fogendID, m_fogdensID, m_fogmodeID;
};

};

#endif /*__CVK_SHADERPHONG_H*/
