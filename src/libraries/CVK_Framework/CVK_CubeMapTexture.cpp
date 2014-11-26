#include <string>
#include "CVK_CubeMapTexture.h"
#include "stb_image.h"

CVK::CubeMapTexture::CubeMapTexture( const char **fileNames)
{
	m_textureID = INVALID_OGL_VALUE;

	createCubeMapTexture();
	load( fileNames);
}

CVK::CubeMapTexture::CubeMapTexture( GLuint texture)
{
	setCubeMapTexture(texture);
}

CVK::CubeMapTexture::~CubeMapTexture()
{
	if (m_textureID != INVALID_OGL_VALUE) glDeleteTextures(1, &m_textureID);
}

void CVK::CubeMapTexture::createCubeMapTexture()
{
	glGenTextures( 1, &m_textureID);
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_textureID);
	glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
}

bool CVK::CubeMapTexture::load( const char **fileNames)
{
	GLint types[6] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for(unsigned int i=0; i<6; i++)
	{
		const char* fileName = fileNames[i];

		int bytesPerPixel = 0;

		unsigned char *data = stbi_load(fileName, &m_width, &m_height, &bytesPerPixel, 0);

		//send image data to the new texture
		GLint internalFormat;
		if (bytesPerPixel < 3)
		{
			printf("ERROR: Unable to load texture image %s\n", fileName);
			return false;
		}
		else if (bytesPerPixel == 3)
		{
			internalFormat = GL_RGB;
		} 
		else if (bytesPerPixel == 4) 
		{
			internalFormat = GL_RGBA;
		} 
		else 
		{
			printf("RESOLVED: Unknown format for bytes per pixel in texture image %s, changed to 4\n", fileName);
			internalFormat = GL_RGBA;
		}
	
		glTexImage2D(types[i], 0, internalFormat, m_width, m_height, 0, internalFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);       

		printf("SUCCESS: Texture image %s loaded\n", fileName);
	}
	return true; 
}

void CVK::CubeMapTexture::bind()
{
	if (m_textureID != INVALID_OGL_VALUE)  glBindTexture( GL_TEXTURE_CUBE_MAP, m_textureID);
}

void CVK::CubeMapTexture::setCubeMapTexture( GLuint texture)
{
	m_textureID = texture;
}

unsigned int CVK::CubeMapTexture::getCubeMapTexture()
{
	return m_textureID;
}

