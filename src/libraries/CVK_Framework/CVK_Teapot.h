#ifndef __CVK_TEAPOT_H
#define __CVK_TEAPOT_H

#include "CVK_Defs.h"
#include "CVK_Geometry.h"

namespace CVK
{

class Teapot : public CVK::Geometry
{
public:
	Teapot();
	~Teapot();

private:
	void create();
};

};

#endif /* __CVK_TEAPOT_H */

