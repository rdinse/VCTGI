#ifndef __CVK_TEXTURE_H
#define __CVK_TEXTURE_H

#include "CVK_Defs.h"

namespace CVK
{

class Texture
{
public:
	Texture( char *fileName);
	Texture( GLuint texture);
	~Texture();

	bool load( char *fileName);
	void bind();

	void setTexture( GLuint texture);
	unsigned int getTexture();

private:
	void createTexture();

	unsigned int m_textureID;
	int m_width, m_height;
};

};

#endif /*__CVK_TEXTURE_H*/
