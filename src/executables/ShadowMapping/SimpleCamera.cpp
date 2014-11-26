#include "SimpleCamera.h"


SimpleCamera::SimpleCamera( int width, int height, float near, float far, float fov) : 
m_up(0.f,1.f,0.f),m_cameraPos(0.f,0.f,-5.f),m_lookAt(0.f,0.f,0.f)
{
	m_width = width;
	m_height = height;
	
	m_fov = fov;
	m_znear = near;
	m_zfar = far;
}

SimpleCamera::~SimpleCamera(void)
{
}

void SimpleCamera::update( GLFWwindow* window)
{
	m_viewmatrix = glm::lookAt(m_cameraPos,m_lookAt,m_up);
}

void SimpleCamera::setPosition(glm::vec3 &position)
{
	m_cameraPos = position;
}

void SimpleCamera::setLookAt(glm::vec3 &lookAt)
{
	m_lookAt = lookAt;
}

void SimpleCamera::setFOV(float fov)
{
	m_fov = fov;
}

float SimpleCamera::getFov()
{
	return m_fov;
}

glm::vec3& SimpleCamera::getLookAt()
{
	return m_lookAt;
}

glm::vec3& SimpleCamera::getPosition()
{
	return m_cameraPos;
}