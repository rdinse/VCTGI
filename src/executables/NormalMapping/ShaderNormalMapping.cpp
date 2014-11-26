#include "ShaderNormalMapping.h"

ShaderNormalMapping::ShaderNormalMapping( GLuint shader_mask, const char** ShaderNames) : CVK::ShaderMinimal(shader_mask, ShaderNames)
{	
	// get uniform location for our new normal texture
	m_normalTexID = glGetUniformLocation(m_ProgramID, "normaltexture");
	m_colorTexID = glGetUniformLocation(m_ProgramID, "colortexture");
	m_useColorTexture = glGetUniformLocation( m_ProgramID, "useColorTexture");

	// light
	m_lightposID = glGetUniformLocation( m_ProgramID, "light.pos");
	m_lightcolID = glGetUniformLocation( m_ProgramID, "light.col"); 

	// material
	m_diffuseID = glGetUniformLocation( m_ProgramID, "mat.diffuse"); 
	m_specularID = glGetUniformLocation( m_ProgramID, "mat.specular"); 
	m_shininessID = glGetUniformLocation( m_ProgramID, "mat.shininess");

	m_lightambID = glGetUniformLocation( m_ProgramID, "lightAmbient");
	
}

void ShaderNormalMapping::update()
{
	ShaderMinimal::update();

	CVK::Light *light = CVK::State::getInstance()->getLights()->at(0);
	glUniform4fv(m_lightposID, 1, glm::value_ptr( *light->getPosition()));
	glUniform3fv(m_lightcolID, 1, glm::value_ptr( *light->getColor()));

	glUniform3fv( m_lightambID, 1, glm::value_ptr( CVK::State::getInstance()->getLightAmbient()));
}

void ShaderNormalMapping::update(CVK::Node* node)
{
	if( node->hasMaterial())
	{
		CVK::Material* mat = node->getMaterial();

		glUniform3fv( m_diffuseID, 1, glm::value_ptr( *mat->getDiffuse()));
		glUniform3fv( m_specularID, 1, glm::value_ptr( *mat->getSpecular()));
		glUniform1f( m_shininessID, mat->getShininess());

		bool colorTexture = mat->hasTexture(CVK::COLOR_TEXTURE);
		glUniform1i( m_useColorTexture, colorTexture);
		
		if (colorTexture)
		{	
			//TODO: COLOR_TEXTURE_UNIT
			glUniform1i( m_colorTexID, 0);

			glActiveTexture(COLOR_TEXTURE_UNIT);
			CVK::Texture* colorTexture = mat->getTexture(CVK::COLOR_TEXTURE);
			colorTexture->bind();
		}

		if (mat->hasTexture(CVK::NORMAL_TEXTURE))
		{	
			//TODO: COLOR_TEXTURE_UNIT
			glUniform1i(m_normalTexID, 1);

			glActiveTexture(NORMAL_TEXTURE_UNIT);
			CVK::Texture* normalTexture = mat->getTexture(CVK::NORMAL_TEXTURE);
			normalTexture->bind();
		}
	}
}
