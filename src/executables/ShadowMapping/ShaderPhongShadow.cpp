#include "ShaderPhongShadow.h"

//The shadow map texture viewport transform
glm::mat4 ShaderPhongShadow::sm_lightViewport = glm::mat4(
														0.5, 0.0, 0.0, 0.0,
														0.0, 0.5, 0.0, 0.0,
														0.0, 0.0, 0.5, 0.0,
														0.5, 0.5, 0.5, 1.0
														);

ShaderPhongShadow::ShaderPhongShadow( GLuint shader_mask, const char** ShaderNames) : CVK::ShaderMinimal(shader_mask, ShaderNames)
{	
	//Material
	m_diffuseID = glGetUniformLocation( m_ProgramID, "mat.diffuse"); 
	m_specularID = glGetUniformLocation( m_ProgramID, "mat.specular"); 
	m_shininessID = glGetUniformLocation( m_ProgramID, "mat.shininess");
	m_alphaID = glGetUniformLocation( m_ProgramID, "mat.alpha");
		
	//Light
	m_lightposID = glGetUniformLocation( m_ProgramID, "light.pos");
	m_lightcolID = glGetUniformLocation( m_ProgramID, "light.col"); 
	m_lightsdirID = glGetUniformLocation( m_ProgramID, "light.spot_direction");
	m_lightsexpID = glGetUniformLocation( m_ProgramID, "light.spot_exponent");
	m_lightscutID = glGetUniformLocation( m_ProgramID, "light.spot_cutoff");
	m_lightambID = glGetUniformLocation( m_ProgramID, "lightAmbient");

	//Textures
	m_useColorTexture = glGetUniformLocation( m_ProgramID, "useColorTexture");
	m_colorTextureID = glGetUniformLocation( m_ProgramID, "colorTexture");

	//Shadow Map
	m_shadowMapID = glGetUniformLocation( m_ProgramID, "shadowMap");
	
	//Light MVP matrix and viewport transform
	m_lightVPBiasID = glGetUniformLocation( m_ProgramID, "lightVPBias");
}

void ShaderPhongShadow::update(SimpleCamera* lightCamera, GLuint shadowMap)
{
	ShaderMinimal::update();

	CVK::Light *light = CVK::State::getInstance()->getLights()->at(0);
	glUniform4fv( m_lightposID, 1, glm::value_ptr( *light->getPosition()));
	glUniform3fv( m_lightcolID, 1, glm::value_ptr( *light->getColor()));

	glUniform3fv( m_lightsdirID, 1, glm::value_ptr( *light->getSpotDirection()));
	glUniform1f( m_lightsexpID, light->getSpotExponent());
	glUniform1f( m_lightscutID, light->getSpotCutoff());

	glUniform3fv( m_lightambID, 1, glm::value_ptr( CVK::State::getInstance()->getLightAmbient()));

	//Shadow mapping
	glm::mat4 lightPerspective, lightView, lightModel, lightMVPBias;
	lightCamera->getPerspective(&lightPerspective);
	lightCamera->getView(&lightView);

	//Build "shadow matrix"
	lightMVPBias = sm_lightViewport * lightPerspective * lightView;
	glUniformMatrix4fv(m_lightVPBiasID, 1, GL_FALSE, glm::value_ptr(lightMVPBias));

	//Bind and Pass shadow map. Only use SHADOW_TEXTURE_UNIT when Normal Mapping is applied.
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	
	glUniform1i( m_shadowMapID, 1);
}

void ShaderPhongShadow::update(CVK::Node* node)
{
	CVK::ShaderMinimal::update( node);

	if( node->hasMaterial())
	{
		CVK::Material* mat = node->getMaterial();
		CVK::Texture *color_texture;

		glUniform3fv( m_diffuseID, 1, glm::value_ptr( *mat->getDiffuse()));
		glUniform3fv( m_specularID, 1, glm::value_ptr( *mat->getSpecular()));
		glUniform1f( m_shininessID, mat->getShininess());
		glUniform1f( m_alphaID, mat->getAlpha());

		bool colorTexture = mat->hasTexture(CVK::COLOR_TEXTURE);
		glUniform1i( m_useColorTexture, colorTexture);

		if (colorTexture)
		{	
			glUniform1i( m_colorTextureID, 0);

			glActiveTexture(COLOR_TEXTURE_UNIT);
			color_texture = mat->getTexture(CVK::COLOR_TEXTURE);
			color_texture->bind();
		}
	}
}
