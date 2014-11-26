#ifndef __SHADER_POST_PROCESSING_H
#define __SHADER_POST_PROCESSING_H

#include "CVK_Defs.h"
#include "CVK_ShaderSet.h"
#include "CVK_Plane.h"

namespace CVK
{

class ShaderPostProcessing : public CVK::ShaderSet
{
public:
	ShaderPostProcessing(GLuint shader_mask, const char** shaderPaths);
	virtual void update();
	void getUniformLocations();
	void render();

private:
	CVK::Plane m_screenFillingQuad;

};

};

#endif /*__SHADER_POST_PROCESSING_H*/
