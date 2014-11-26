#ifndef __SHADERMINIMAL_H
#define __SHADERMINIMAL_H

#include <CVK_Framework/CVK_Defs.h>
#include <CVK_Framework/CVK_ShaderSet.h>
#include <CVK_Framework/CVK_State.h>
#include <CVK_Framework/CVK_Node.h>

namespace CVK
{

class ShaderMinimal : public CVK::ShaderSet
{
public:
	ShaderMinimal(GLuint shader_mask, const char** shaderPaths);
	void updateModelMatrix(glm::mat4 modelmatrix);
	virtual void update();
	virtual void update(CVK::Node* node);
	void getUniformLocations();

protected:
	GLuint m_modelMatrixID;
	GLuint m_viewMatrixID, m_projectionMatrixID;

};

};

#endif /*__SHADERMINIMAL_H*/
