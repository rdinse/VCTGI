#ifndef __CVK_MATERIAL_H
#define __CVK_MATERIAL_H

#include "CVK_Defs.h"
#include "CVK_Texture.h"

namespace CVK
{
	enum TextureType
	{
		COLOR_TEXTURE,
		NORMAL_TEXTURE
	};

class Material 
{
public:
	Material( glm::vec3 diffuse = glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3 specular = glm::vec3( 0.3f, 0.3f, 0.3f), float shininess = 15.0f); 
	Material( char *colorTexturePath, glm::vec3 diffuse = glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3 specular = glm::vec3( 0.3f, 0.3f, 0.3f), float shininess = 15.0f);
	Material( GLuint colorTextureID, glm::vec3 diffuse = glm::vec3( 0.5f, 0.5f, 0.5f), glm::vec3 specular = glm::vec3( 0.3f, 0.3f, 0.3f), float shininess = 15.0f);
	~Material( );

	void init(glm::vec3 diffuse, glm::vec3 specular, float shininess);

	void setDiffuse( glm::vec3 col); 
	glm::vec3* getDiffuse();

	void setSpecular( glm::vec3 col); 
	glm::vec3* getSpecular();

	void setShininess( float shininess); 
	float getShininess();

	void setAlpha ( float alpha);
	float getAlpha();

	void setReflectionStrength ( float reflectionStrength);
	float getReflectionStrength();

	void setTexture( TextureType type, char *fileName);
	void setTexture( TextureType type, GLuint textureID);
	bool hasTexture( TextureType type);
	CVK::Texture* getTexture( TextureType type);


private:

	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	float m_shininess;
	float m_alpha;
	float m_reflectionStrength;

	CVK::Texture* m_colorTexture;
	CVK::Texture* m_normalTexture;
};

};

#endif /* __CVK_MATERIAL_H */

