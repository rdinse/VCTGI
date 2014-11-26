#ifndef __CVK_TRACKBALL_H
#define __CVK_TRACKBALL_H

#include "CVK_Camera.h"

namespace CVK
{

class Trackball : public CVK::Camera
{
public:
	Trackball( int width, int height);
	~Trackball();

	void update( GLFWwindow* window);
	void setCenter( glm::vec3 center);
	void setRadius( float radius);

private:
	glm::vec3 m_cameraPos, m_center, m_up;

	double m_oldX, m_oldY;
	double m_sensitivity;
	double m_theta, m_phi, m_radius;
};

};

#endif /* __CVK_TRACKBALL_H */
