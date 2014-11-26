#include "ShaderCubeMapReflection.h"

ShaderCubeMapReflection::ShaderCubeMapReflection( GLuint shader_mask, const char** ShaderNames) : CVK::ShaderMinimal(shader_mask, ShaderNames)
{	
	//Skybox
	m_cubeMapID = glGetUniformLocation(m_ProgramID, "cubeMap");

	//Color
	m_diffuseColorID = glGetUniformLocation( m_ProgramID, "mat.diffuse"); 
	m_specularID = glGetUniformLocation( m_ProgramID, "mat.specular"); 
	m_shininessID = glGetUniformLocation( m_ProgramID, "mat.shininess");
	m_reflectionStrengthID = glGetUniformLocation( m_ProgramID, "mat.reflectionStrength");
	
	//Texture
	m_useColorTexture = glGetUniformLocation( m_ProgramID, "useColorTexture");
	m_colorTextureID = glGetUniformLocation( m_ProgramID, "colorTexture");

	//Light
	m_lightposID = glGetUniformLocation( m_ProgramID, "light.pos");
	m_lightcolID = glGetUniformLocation( m_ProgramID, "light.col"); 
	m_lightsdirID = glGetUniformLocation( m_ProgramID, "light.spot_direction");
	m_lightsexpID = glGetUniformLocation( m_ProgramID, "light.spot_exponent");
	m_lightscutID = glGetUniformLocation( m_ProgramID, "light.spot_cutoff");
}

void ShaderCubeMapReflection::update()
{
	ShaderMinimal::update();
	
	CVK::Light *light = CVK::State::getInstance()->getLights()->at(0);
	glUniform4fv( m_lightposID, 1, glm::value_ptr( *light->getPosition()));
	glUniform3fv( m_lightcolID, 1, glm::value_ptr( *light->getColor()));

	glUniform3fv( m_lightsdirID, 1, glm::value_ptr( *light->getSpotDirection()));
	glUniform1f( m_lightsexpID, light->getSpotExponent());
	glUniform1f( m_lightscutID, light->getSpotCutoff());

	glUniform3fv( m_lightambID, 1, glm::value_ptr( CVK::State::getInstance()->getLightAmbient()));

	/////////////////////////////////////////////////////
	//////////////// Cube Map Reflection ////////////////
	/////////////////////////////////////////////////////
	glUniform1i( m_cubeMapID, 3);
	glActiveTexture(CUBE_MAP_TEXTURE_UNIT);

	CVK::CubeMapTexture* cubeMap = CVK::State::getInstance()->getCubeMapTexture();

	if(cubeMap != 0)
	{
		cubeMap->bind();
	}
}

void ShaderCubeMapReflection::update( CVK::Node* node)
{
	ShaderMinimal::update(node);

	if( node->hasMaterial())
	{
		CVK::Material* mat = node->getMaterial();
		CVK::Texture *color_texture;
		
		glUniform3fv( m_diffuseColorID, 1, glm::value_ptr( *mat->getDiffuse()));
		glUniform3fv( m_specularID, 1, glm::value_ptr( *mat->getSpecular()));
		glUniform1f( m_shininessID, mat->getShininess());
		glUniform1f( m_reflectionStrengthID, mat->getReflectionStrength());

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

