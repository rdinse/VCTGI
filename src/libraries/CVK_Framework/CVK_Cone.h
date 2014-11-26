// Cone along y-axis, origin in base midpoint

#ifndef __CVK_CONE_H
#define __CVK_CONE_H

#include "CVK_Defs.h"
#include "CVK_Geometry.h"

namespace CVK
{

class Cone : public CVK::Geometry
{
public:
	Cone();
	Cone( int resolution);
	Cone( float baseRadius, float topRadius, float height);
	Cone( float baseRadius, float topRadius, float height, int resolution);
	~Cone();

private:
	void create( float baseRadius, float topRadius, float height, int resolution);

	float m_baseRadius, m_topRadius, m_height;
	int m_resolution;
};

};

#endif /* __CVK_CONE_H */

