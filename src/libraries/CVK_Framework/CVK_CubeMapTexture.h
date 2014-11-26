#ifndef __CVK_CUBE_MAP_TEXTURE_H
#define __CVK_CUBE_MAP_TEXTURE_H

#include "CVK_Defs.h"

namespace CVK
{

class CubeMapTexture
{
public:
	CubeMapTexture( const char **fileNames);
	CubeMapTexture( GLuint texture);
	~CubeMapTexture();

	bool load( const char **fileNames);
	void bind();

	void setCubeMapTexture( GLuint texture);
	unsigned int getCubeMapTexture();

private:
	void createCubeMapTexture();

	unsigned int m_textureID;
	int m_width, m_height;
};

};

#endif /*__CVK_CUBE_MAP_TEXTURE_H*/
