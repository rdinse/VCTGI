#ifndef __CVK_WIRE_CUBE_H
#define __CVK_WIRE_CUBE_H

#include "CVK_Defs.h"
#include "CVK_Cube.h"

namespace CVK
{

class WireCube : public CVK::Cube
{
public:
	WireCube();
	WireCube(float size);
	~WireCube();

	void render();
};

};

#endif /* __CVK_WIRE_CUBE_H */

