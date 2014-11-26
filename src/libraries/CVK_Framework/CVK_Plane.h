#ifndef __CVK_PLANE_H
#define __CVK_PLANE_H

#include "CVK_Defs.h"
#include "CVK_Geometry.h"

namespace CVK
{

class Plane : public CVK::Geometry
{
public:
	Plane();
	Plane( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd);
	~Plane();

	void set_Points( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
	void set_Tcoords( glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd);

private:
	void create( glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd);
};

};

#endif /* __CVK_PLANE_H */

