#ifndef __CVK_LIGHT_H
#define __CVK_LIGHT_H

#include "CVK_Defs.h"

namespace CVK
{

class Light 
{
public:
	Light();
	Light( glm::vec4 pos, glm::vec3 col, glm::vec3 s_dir, float s_exp, float s_cut);
	~Light();

	void setPosition( glm::vec4 pos); 
	glm::vec4 *getPosition(); 

	void setColor( glm::vec3 col); 
	glm::vec3 *getColor(); 

	void setSpotDirection( glm::vec3 direction); 
	glm::vec3 *getSpotDirection(); 

	void setSpotExponent( float spotExponent); 
	float getSpotExponent(); 

	void setSpotCutoff( float spotCutoff); 
	float getSpotCutoff(); 

private:
	glm::vec4 m_position;
    glm::vec3 m_color;
	glm::vec3 m_spotDirection;
	float m_spotExponent;
	float m_spotCutoff;
};

};

#endif /* __CVK_LIGHT_H */

