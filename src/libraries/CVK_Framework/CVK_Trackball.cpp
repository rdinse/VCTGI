#include "CVK_Trackball.h"

CVK::Trackball::Trackball( int width, int height)
{
	m_cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
	m_center = glm::vec3( 0.5f, 0.5f, 0.5f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	m_width = width;
	m_height = height;

	m_fov = 60.0f;
	m_znear = 0.001f;
	m_zfar = 10.0f;

	m_sensitivity = 0.010f;
	m_theta = glm::pi<float>() / 2.0f;
	m_phi = -glm::pi<float>() / 2.0f;
	m_radius = 1.5;

	m_viewmatrix = glm::lookAt( m_center + m_cameraPos, m_center, m_up);   
	m_perspective = glm::perspective( m_fov, m_width / (float) m_height, m_znear, m_zfar);    
 
	m_oldX = width/2;
	m_oldY = height/2;
}

CVK::Trackball::~Trackball()
{
}

void CVK::Trackball::update( GLFWwindow* window)
{
	double x, y;

	glfwGetCursorPos( window, &x, &y);  
	if (glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{	
		double changeX = (x - m_oldX) * m_sensitivity;
		double changeY = (y - m_oldY) * m_sensitivity;

		m_theta -= changeY;
		if (m_theta < 0.01f) m_theta = 0.01f;
		else if (m_theta > glm::pi<float>() - 0.01f) m_theta = glm::pi<float>() - 0.01f;

		m_phi -= changeX;	
		if (m_phi < 0) m_phi += 2*glm::pi<float>();
		else if (m_phi > 2*glm::pi<float>()) m_phi -= 2*glm::pi<float>();
	}

	m_oldX = x;
	m_oldY = y;

	if (glfwGetKey( window, GLFW_KEY_UP) == GLFW_PRESS)            
		m_radius -= 0.1f;
	if (glfwGetKey( window, GLFW_KEY_DOWN) == GLFW_PRESS) 
		m_radius += 0.1f;
	if (m_radius < 0.1) m_radius = 0.1;

	m_cameraPos.x = m_radius * sin(m_theta) * cos(m_phi);
	m_cameraPos.y = m_radius * cos(m_theta);
	m_cameraPos.z = -m_radius * sin(m_theta) * sin(m_phi);

	m_viewmatrix = glm::lookAt( m_center+m_cameraPos, m_center, m_up);  
}

void CVK::Trackball::setCenter( glm::vec3 center)
{
	m_center = center;
}

void CVK::Trackball::setRadius( float radius)
{
	m_radius = radius;
}