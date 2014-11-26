#include "ShaderGBuffer.h"
#include "Renderer.h"

ShaderGBuffer::ShaderGBuffer( GLuint shader_mask, const char** ShaderNames) : CVK::ShaderMinimal(shader_mask, ShaderNames) { }

void ShaderGBuffer::getUniformLocations() {
	CVK::ShaderMinimal::getUniformLocations();

	m_useColorTextureID = glGetUniformLocation( m_ProgramID, "useColorTexture");
	m_colorTexID = glGetUniformLocation(m_ProgramID, "colorTexture");
	m_diffuseColorID = glGetUniformLocation(m_ProgramID, "diffuseColor");
	m_alphaID = glGetUniformLocation(m_ProgramID, "alpha");
}

void ShaderGBuffer::update()
{
	ShaderMinimal::update();

	// 207,240,158  168,219,168  121,189,154  59,134,134
	// glClearColor(207.0f/255.0f, 240.0f/255.0f, 158.0f/255.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if(Renderer::getInstance()->tw_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void ShaderGBuffer::resetModelMatrix()
{
	glm::mat4 identityModelMatrix(1.0);
	glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, glm::value_ptr(identityModelMatrix));
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
