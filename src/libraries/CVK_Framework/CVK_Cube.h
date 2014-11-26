#ifndef __CVK_CUBE_H
#define __CVK_CUBE_H

#include "CVK_Defs.h"
#include "CVK_Geometry.h"

namespace CVK
{

class Cube : public CVK::Geometry
{
public:
	Cube();
	Cube(float size);
	~Cube();
	
	void setSize(float size);

protected:
	void create(float size);
	
	float m_size;
};

};

#endif /* __CVK_CUBE_H */

