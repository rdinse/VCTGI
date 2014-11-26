#include <string>
#include "CVK_Texture.h"
#include "stb_image.h"

CVK::Texture::Texture( char *fileName)
{
	m_textureID = INVALID_OGL_VALUE;

	createTexture();
	load( fileName);
}

CVK::Texture::Texture( GLuint texture)
{
	setTexture(texture);
}

CVK::Texture::~Texture()
{
	if (m_textureID != INVALID_OGL_VALUE) glDeleteTextures(1, &m_textureID);
}

void CVK::Texture::createTexture()
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // Trilinear filtering.
	glGenerateMipmap(GL_TEXTURE_2D);
}

bool CVK::Texture::load( char *fileName)
{
	int bytesPerPixel = 0;

	unsigned char *data = stbi_load(fileName, &m_width, &m_height, &bytesPerPixel, 0);

	//flip image vertically
	unsigned char* s = data;
	for (int y=0; y<m_height/2; y++)
	{
		unsigned char* e = data+(m_height-y-1)*m_width*bytesPerPixel;
		for(int x=0; x<m_width*bytesPerPixel; x++)
		{
			unsigned char temp = *s;
			*s = *e;
			*e = temp;
			s++;
			e++;
		}
	}

	//send image data to the new texture
	if (bytesPerPixel < 3)
	{
		printf("ERROR: Unable to load texture image %s\n", fileName);
		return false;
	}
	else if (bytesPerPixel == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	} 
	else if (bytesPerPixel == 4) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	} 
	else 
	{
		printf("RESOLVED: Unknown format for bytes per pixel in texture image %s, changed to 4\n", fileName);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	stbi_image_free(data);
	glGenerateMipmap(GL_TEXTURE_2D);          

	printf("SUCCESS: Texture image %s loaded\n", fileName);
	return true; 
}

void CVK::Texture::bind()
{
	if (m_textureID != INVALID_OGL_VALUE)  glBindTexture( GL_TEXTURE_2D, m_textureID);
}

void CVK::Texture::setTexture( GLuint texture)
{
	m_textureID = texture;
}

unsigned int CVK::Texture::getTexture()
{
	return m_textureID;
}

