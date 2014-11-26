#include "CVK_Camera.h"

CVK::Camera::Camera()
{
	setOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 1.0f);
	m_useOrtho = false;
}

CVK::Camera::~Camera()
{
}

void CVK::Camera::getView( glm::mat4 *view)
{
	*view = m_viewmatrix;
}

void CVK::Camera::getPerspective( glm::mat4 *perspective)
{
	*perspective = m_perspective;
}

void CVK::Camera::getOrtho( glm::mat4 *ortho)
{
	*ortho = m_ortho;
}

void CVK::Camera::useOrtho( float useOrtho)
{
	m_useOrtho = useOrtho;
}

void CVK::Camera::setOrtho( float left, float right, float bottom, float top, float near, float far)
{
	m_orthoValues[0] = left;
	m_orthoValues[1] = right;
	m_orthoValues[2] = bottom;
	m_orthoValues[3] = top;
	m_orthoValues[4] = near;
	m_orthoValues[5] = far;
	m_ortho = glm::ortho( left, right, bottom, top, near, far);  
	m_useOrtho = true;
}

void CVK::Camera::getProjection( glm::mat4 *projection)
{
	if (m_useOrtho) 
		*projection = m_ortho;
	else 
		*projection = m_perspective;
}

void CVK::Camera::setNearFar( float near, float far)
{
	m_znear = near;
	m_zfar = far;
	m_perspective = glm::perspective( m_fov, m_width / (float) m_height, m_znear, m_zfar);    
	m_useOrtho = false;
}

void CVK::Camera::getNearFar( float *near, float *far)
{
	*near = m_znear;
	*far = m_zfar;
}

void CVK::Camera::setWidthHeight( int width, int height)
{
	m_width = width;
	m_height = height;
}
