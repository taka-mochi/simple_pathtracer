#include "color.h"
#include "scene.h"
#include "renderer.h"

PathTracer::PathTracer(int screen_width, int screen_height, int samples, int supersamples)
	: m_width(screen_width)
	, m_height(screen_height)
	, m_samples(samples)
	, m_supersamples(supersamples)
{
	m_result = new Color[m_width*m_height];
}

PathTracer::~PathTracer()
{
	delete [] m_result;
}

void PathTracer::RenderScene(const Scene &scene) {
}
