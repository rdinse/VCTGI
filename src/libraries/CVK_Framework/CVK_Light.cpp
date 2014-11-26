#include "CVK_Light.h"

// parameters like in OpenGL 1.1 (s. spec)
CVK::Light::Light()
{
	m_position = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f);
    m_color = glm::vec3( 1.0f, 1.0f, 1.0f);
	m_spotDirection = glm::vec3( 0.0f, 1.0f, 0.0f);;
	m_spotExponent = 1.0f;
	m_spotCutoff = 0.0f;
}

CVK::Light::Light( glm::vec4 pos, glm::vec3 col, glm::vec3 s_dir, float s_exp, float s_cut)
{
	m_position = pos;
	m_color = col;
	m_spotDirection = s_dir;
	m_spotExponent = s_exp;
	m_spotCutoff = s_cut;
}

CVK::Light::~Light()
{
}

void CVK::Light::setPosition( glm::vec4 pos)
{
	m_position = pos;
} 

glm::vec4 *CVK::Light::getPosition()
{
	return &m_position;
} 

void CVK::Light::setColor( glm::vec3 col)
{
	m_color = col;
} 

glm::vec3 *CVK::Light::getColor()
{
	return &m_color;
} 
	
void CVK::Light::setSpotDirection( glm::vec3 direction)
{
	m_spotDirection = direction;
} 

glm::vec3 *CVK::Light::getSpotDirection() 
{
	return &m_spotDirection;
}

void CVK::Light::setSpotExponent( float spotExponent) 
{
	m_spotExponent = spotExponent;
}

float CVK::Light::getSpotExponent()
{
	return m_spotExponent;
}

void CVK::Light::setSpotCutoff( float spotCutoff) 
{
	m_spotCutoff = spotCutoff;
}

float CVK::Light::getSpotCutoff() 
{
	return m_spotCutoff;
}