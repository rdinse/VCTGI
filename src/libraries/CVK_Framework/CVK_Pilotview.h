#ifndef __CVK_PILOTVIEW_H
#define __CVK_PILOTVIEW_H

#include "CVK_Camera.h"

namespace CVK
{

class Pilotview : public CVK::Camera
{
public:
	Pilotview( int width, int height);
	~Pilotview();

	void update( GLFWwindow* window);
	void setCamPos( glm::vec3 campos);

private:
	glm::vec3 m_cameraPos, m_direction, m_up;

	double m_oldX, m_oldY;
	double m_step, m_sensitivity;
	float m_speed;
	double m_theta, m_phi;
};

};

#endif /* __CVK_PILOTVIEW_H */
