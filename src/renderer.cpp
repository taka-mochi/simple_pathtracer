#include "color.h"
#include "scene.h"
#include "renderer.h"

PathTracer::PathTracer(int screen_width, int screen_height, int samples, int supersamples)
	: m_width(screen_width)
	, m_height(screen_height)
	, m_samples(samples)
	, m_supersamples(supersamples)
	, m_camPos(50.0, 52.0, 220.0)
	, m_camDir(0.0, -0.04, -1.0)
	, m_camUp(0.0, 1.0, 0.0)
	, m_nearScreenHeight(30.0)
	, m_distToScreen(40.0)
{
	m_camDir.normalize();
	m_result = new Color[m_width*m_height];
}

PathTracer::~PathTracer()
{
	delete [] m_result;
}

void PathTracer::RenderScene(const Scene &scene) {
}
