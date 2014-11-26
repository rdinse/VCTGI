#include "ShaderGBuffer.h"

ShaderGBuffer::ShaderGBuffer( GLuint shader_mask, const char** ShaderNames) : CVK::ShaderMinimal(shader_mask, ShaderNames)
{	
	m_useColorTextureID = glGetUniformLocation( m_ProgramID, "useColorTexture");
	m_colorTexID = glGetUniformLocation(m_ProgramID, "colortexture");
	m_diffuseColorID = glGetUniformLocation(m_ProgramID, "diffuseColor");
	m_alphaID = glGetUniformLocation(m_ProgramID, "alpha");
}

void ShaderGBuffer::update()
{
	ShaderMinimal::update();
}

void ShaderGBuffer::update(CVK::Node* node)
{
	// COLOR_TEXTURE_UNIT
	glUniform1i( m_colorTexID, 0);
	glActiveTexture(COLOR_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, 0);

	if( node->hasMaterial())
	{
		CVK::Material* mat = node->getMaterial();

		glm::vec3* diffuseColor = mat->getDiffuse();
		glUniform3f(m_diffuseColorID, diffuseColor->r, diffuseColor->g, diffuseColor->b);

		bool colorTexture = mat->hasTexture(CVK::COLOR_TEXTURE);
		glUniform1i( m_useColorTextureID, colorTexture);

		if (mat->hasTexture(CVK::COLOR_TEXTURE))
		{	
			mat->getTexture(CVK::COLOR_TEXTURE)->bind();
		}
	}
}
