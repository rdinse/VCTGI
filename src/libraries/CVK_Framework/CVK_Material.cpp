#include "CVK_Material.h"

CVK::Material::Material( glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	init(diffuse, specular, shininess);
}

CVK::Material::Material(char *colorTexturePath, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	init(diffuse, specular, shininess);
	setTexture(COLOR_TEXTURE, colorTexturePath);
}

CVK::Material::Material(GLuint colorTextureID, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	init(diffuse, specular, shininess);
	setTexture(COLOR_TEXTURE, colorTextureID);
}
void CVK::Material::init(glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	m_diffuse = diffuse;
	m_specular = specular;
	m_shininess = shininess;
	m_alpha = 1.0f; //opaque
	m_reflectionStrength = 0.0f;

	m_colorTexture = NULL;
	m_normalTexture = NULL;
}

CVK::Material::~Material()
{	
}

void CVK::Material::setDiffuse( glm::vec3 col)
{
	m_diffuse = col;
}

glm::vec3* CVK::Material::getDiffuse()
{
	return &m_diffuse;
}

void CVK::Material::setSpecular( glm::vec3 col)
{
	m_specular = col;
}

glm::vec3* CVK::Material::getSpecular()
{
	return &m_specular;
}

void CVK::Material::setShininess( float shininess)
{
	m_shininess = shininess;
}

float CVK::Material::getShininess()
{
	return m_shininess;
}

void CVK::Material::setAlpha( float alpha)
{
	m_alpha = alpha;
}

float CVK::Material::getAlpha()
{
	return m_alpha;
}

void CVK::Material::setReflectionStrength( float reflectionStrength)
{
	m_reflectionStrength = reflectionStrength;
}

float CVK::Material::getReflectionStrength()
{
	return m_reflectionStrength;
}

void CVK::Material::setTexture( TextureType type, char *fileName)
{
	switch(type)
	{
		case COLOR_TEXTURE:
			if(m_colorTexture)
				m_colorTexture->load( fileName);
			else
				m_colorTexture = new Texture( fileName); 
			break;
		case NORMAL_TEXTURE:
			if(m_normalTexture)
				m_normalTexture->load( fileName);
			else
				m_normalTexture = new Texture( fileName);
			break;

	}
}

void CVK::Material::setTexture( TextureType type, GLuint textureID)
{
	switch(type)
	{
		case COLOR_TEXTURE:
			if(m_colorTexture)
				m_colorTexture->setTexture(textureID);
			else
				m_colorTexture = new Texture(textureID); 
			break;
		case NORMAL_TEXTURE:
			if(m_normalTexture)
				m_normalTexture->setTexture(textureID);
			else
				m_normalTexture = new Texture(textureID);
			break;

	}
}

bool CVK::Material::hasTexture( TextureType type)
{
	switch(type)
	{
		case COLOR_TEXTURE:
			return m_colorTexture != NULL;
		case NORMAL_TEXTURE:
			return m_normalTexture != NULL;
	}
}

CVK::Texture* CVK::Material::getTexture( TextureType type)
{
	switch(type)
	{
		case COLOR_TEXTURE:
			return m_colorTexture;
		case NORMAL_TEXTURE:
			return m_normalTexture;
	}
}


