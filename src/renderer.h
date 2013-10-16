#pragma once

#include "color.h"

class Scene;

class PathTracer {
public:
	PathTracer(int screen_width, int screen_height, int samples, int supersamples);
	~PathTracer();

	void RenderScene(const Scene &scene);

	const Color *GetResult() const {return m_result;}

private:
	int m_width;
	int m_height;
	int m_samples;
	int m_supersamples;

	Color *m_result;
};
