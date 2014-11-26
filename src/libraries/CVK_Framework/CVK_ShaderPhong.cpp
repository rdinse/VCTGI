#include "CVK_ShaderPhong.h"


CVK::ShaderPhong::ShaderPhong(GLuint shader_mask, const char** shaderPaths) : CVK::ShaderMinimal(shader_mask,shaderPaths) { }

void CVK::ShaderPhong::getUniformLocations() {
	CVK::ShaderMinimal::getUniformLocations();
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

	//Textures
	m_useColorTexture = glGetUniformLocation( m_ProgramID, "useColorTexture");
	m_colorTextureID = glGetUniformLocation( m_ProgramID, "colorTexture");

	//Fog
	m_lightambID = glGetUniformLocation( m_ProgramID, "lightAmbient");
	m_fogcolID = glGetUniformLocation( m_ProgramID, "fog.col");
	m_fogstartID = glGetUniformLocation( m_ProgramID, "fog.start");
	m_fogendID = glGetUniformLocation( m_ProgramID, "fog.end");
	m_fogdensID = glGetUniformLocation( m_ProgramID, "fog.density");
	m_fogmodeID = glGetUniformLocation( m_ProgramID, "fog.mode");
}

void CVK::ShaderPhong::update()
{
	CVK::ShaderMinimal::update();

	CVK::Light *light = CVK::State::getInstance()->getLights()->at(0);
	glUniform4fv( m_lightposID, 1, glm::value_ptr( *light->getPosition()));
	glUniform3fv( m_lightcolID, 1, glm::value_ptr( *light->getColor()));

	glUniform3fv( m_lightsdirID, 1, glm::value_ptr( *light->getSpotDirection()));
	glUniform1f( m_lightsexpID, light->getSpotExponent());
	glUniform1f( m_lightscutID, light->getSpotCutoff());

	glUniform3fv( m_lightambID, 1, glm::value_ptr( CVK::State::getInstance()->getLightAmbient()));

	glUniform3fv( m_fogcolID, 1, glm::value_ptr( CVK::State::getInstance()->getFogCol()));
	glUniform1i( m_fogmodeID, CVK::State::getInstance()->getFogMode());
	glUniform1f( m_fogstartID, CVK::State::getInstance()->getFogStart());
	glUniform1f( m_fogendID, CVK::State::getInstance()->getFogEnd());
	glUniform1f( m_fogdensID, CVK::State::getInstance()->getFogDens());
}

void CVK::ShaderPhong::update( CVK::Node* node)
{

	if( node->hasMaterial())
	{
		CVK::Material* mat = node->getMaterial();
		CVK::Texture *color_texture;

		glUniform3fv( m_diffuseID, 1, glm::value_ptr( *mat->getDiffuse()));
		glUniform3fv( m_specularID, 1, glm::value_ptr( *mat->getSpecular()));
		glUniform1f( m_shininessID, mat->getShininess());
		glUniform1f( m_alphaID, mat->getAlpha());

		bool colorTexture = mat->hasTexture(COLOR_TEXTURE);
		glUniform1i( m_useColorTexture, colorTexture);

		if (colorTexture)
		{	
			//TODO: COLOR_TEXTURE_UNIT
			glUniform1i( m_colorTextureID, 0);

			glActiveTexture(COLOR_TEXTURE_UNIT);
			color_texture = mat->getTexture(COLOR_TEXTURE);
			color_texture->bind();
		}
	}
}
