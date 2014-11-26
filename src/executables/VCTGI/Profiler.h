#ifndef __PROFILER_H
#define __PROFILER_H

#include "Singleton.h"
#include "Common.h"

#include "Renderer.h"
#include "Context.h"

enum tw_EProfilerPoint {
	PASS_INIT = 0,
	PASS_G_BUFFER,
	PASS_SHADOW_MAP,
	PASS_VOXELIZATION_CLEAR,
	PASS_VOXELIZATION,
	PASS_PRE_INTEGRATION,
	PASS_DIRECT_ILLUMINATION,
	PASS_GLOBAL_ILLUMINATION,
	PASS_COMPOSITING,
	PASS_DRAW,
	E_NUM_PROFILER_POINTS
};

class Profiler : public Singleton <Profiler>
{
friend class Singleton <Profiler>;
public:
	~Profiler();
	void init();
	void updateProfiler(tw_EProfilerPoint point, bool isFirstFrame);
	void updateProfilerFirst(tw_EProfilerPoint point);
	void afterDrawUpdate();
	void printProfilerData();
	void updateFPS(double deltaT);
	
	unsigned int tw_profilerMeanSamples;
	double tw_profilerTimings[E_NUM_PROFILER_POINTS];
	GLuint  m_queryIDs[E_NUM_PROFILER_POINTS];
	GLuint  m_queryResults[E_NUM_PROFILER_POINTS];
	double tw_fps;
protected:
	Profiler();
private:
};

#endif /* __PROFILER_H */
