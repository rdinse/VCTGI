#include "Profiler.h"

std::map<tw_EProfilerPoint, const char *> tw_profilerPointNames = {
	{PASS_INIT, "Input Handling, GUI, Misc."},
	{PASS_G_BUFFER, "G-Buffer Pass"},
	{PASS_SHADOW_MAP, "Shadow Map Pass"},
	{PASS_VOXELIZATION_CLEAR, "Voxel Map Clearing Pass"},
	{PASS_VOXELIZATION, "Voxelization Pass"},
	{PASS_PRE_INTEGRATION, "Pre-Integration Pass"},
	{PASS_DIRECT_ILLUMINATION, "Direct Illumination Pass"},
	{PASS_GLOBAL_ILLUMINATION, "Global Illumination Pass"},
	{PASS_COMPOSITING, "Compositing Pass"},
	{PASS_DRAW, "Drawing Pass"}
};

Profiler::Profiler() {
	tw_profilerMeanSamples = 10;
	tw_fps = 0.0;
}

Profiler::~Profiler() {
	printProfilerData();
}

void Profiler::updateFPS(double deltaT) {
	tw_fps = 1.0 / deltaT;
}

void Profiler::init() {
	glGenQueries(E_NUM_PROFILER_POINTS, m_queryIDs);

	for (int i = 0; i < E_NUM_PROFILER_POINTS; i++) {
		Profiler::getInstance()->tw_profilerTimings[i] = 0.0;
	}
}

void Profiler::updateProfilerFirst(tw_EProfilerPoint point) {
	glBeginQuery(GL_TIME_ELAPSED, m_queryIDs[point]);
}

void Profiler::updateProfiler(tw_EProfilerPoint point, bool isFirstFrame) {
	glEndQuery(GL_TIME_ELAPSED);
	if (!isFirstFrame) {
		glGetQueryObjectuiv(m_queryIDs[point], GL_QUERY_RESULT, &m_queryResults[point]);
	}

	glBeginQuery(GL_TIME_ELAPSED, m_queryIDs[point]);
}

void Profiler::afterDrawUpdate() {
	for (int i = 0; i < E_NUM_PROFILER_POINTS; i++) {
		double d = m_queryResults[i] / 1000000.0;

		if (tw_profilerTimings[i] == 0) {
			tw_profilerTimings[i] = d;
		} else {
			tw_profilerTimings[i] = (d + (double)(tw_profilerMeanSamples - 1) * (tw_profilerTimings[i]))
										/ (double)(tw_profilerMeanSamples);
		}
	}
}

void Profiler::printProfilerData() {
	printf("Time Measurements:\n");
	for (int i = 0; i < E_NUM_PROFILER_POINTS; i++) {
		tw_EProfilerPoint point = static_cast<tw_EProfilerPoint>(i);
		printf("\t%s:  %.2f ms\n", tw_profilerPointNames[point], tw_profilerTimings[point]);
	}
}
