#pragma once

#include "color.h"
#include "scene.h"

class Scene;
class Ray;
class Random;

class PathTracer {
public:
	PathTracer(int screen_width, int screen_height, int samples, int supersamples);
	~PathTracer();

	void SetCamera(const Vector3 &pos, const Vector3 &dir, const Vector3 &up) {
		m_camPos = pos;
		m_camDir = dir; m_camDir.normalize();
		m_camUp = up;
	}

	void RenderScene(const Scene &scene);

	const Color *GetResult() const {return m_result;}

private:
  Color Radiance(const Scene &scene, const Ray &ray, Random &rnd, const int depth);

  Color Radiance_Lambert(const Scene &scene, const Ray &ray, Random &rnd, const int depth, Scene::IntersectionInformation &intersect, const Vector3 &normal, double russian_roulette_prob);

private:
	int m_width;
	int m_height;
	int m_samples;
	int m_supersamples;

	Vector3 m_camPos;
	Vector3 m_camDir;
	Vector3 m_camUp;
	double m_nearScreenHeight;
	double m_distToScreen;
	
	Color *m_result;
};
