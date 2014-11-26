#ifndef __CVK_SPHERE_H
#define __CVK_SPHERE_H

#include "CVK_Defs.h"
#include "CVK_Geometry.h"

namespace CVK
{

class Sphere : public CVK::Geometry
{
public:
	Sphere();
	Sphere( float radius);
	Sphere( int resolution);
	Sphere( float radius, int resolution);
	~Sphere();

private:
	void create(float radius, int resolution);

	float m_radius;
	int m_resolution;
};

};

#endif /* __CVK_SPHERE_H */

