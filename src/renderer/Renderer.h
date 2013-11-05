#pragma once

#include "Color.h"
#include "scenes/Scene.h"

namespace SimpleRenderer {

class Scene;
class Ray;
class Random;

class PathTracer {
public:
  class RenderingFinishCallback {
  public:
    virtual void operator()(int samples, const Color *result) = 0;
  };
public:
	PathTracer(int screen_width, int screen_height, int samples, int supersamples);
  PathTracer(int screen_width, int screen_height, int min_samples, int max_samples, int step, int supersamples, RenderingFinishCallback *callback);
	~PathTracer();

	void SetCamera(const Vector3 &pos, const Vector3 &dir, const Vector3 &up) {
		m_camPos = pos;
		m_camDir = dir; m_camDir.normalize();
		m_camUp = up;
	}

	void RenderScene(const Scene &scene);

	const Color *GetResult() const {return m_result;}

private:
  void init(int screen_width, int screen_height, int min_samples, int max_samples, int step, int supersamples, RenderingFinishCallback *callback);

  void ScanPixelsAndCastRays(const Scene &scene, const Vector3 &screen_x, const Vector3 &screen_y, const Vector3 &screen_center, int previous_samples, int next_samples);
  Color Radiance(const Scene &scene, const Ray &ray, Random &rnd, const int depth);

  Color Radiance_Lambert(const Scene &scene, const Ray &ray, Random &rnd, const int depth, Scene::IntersectionInformation &intersect, const Vector3 &normal, double russian_roulette_prob);
  Color Radiance_Specular(const Scene &scene, const Ray &ray, Random &rnd, const int depth, Scene::IntersectionInformation &intersect, const Vector3 &normal, double russian_roulette_prob);
  Color Radiance_Refraction(const Scene &scene, const Ray &ray, Random &rnd, const int depth, Scene::IntersectionInformation &intersect, const Vector3 &normal, double russian_roulette_prob);

private:
	int m_width;
	int m_height;
	int m_min_samples,m_max_samples,m_step_samples;
	int m_supersamples;
  RenderingFinishCallback *m_renderFinishCallback;

	Vector3 m_camPos;
	Vector3 m_camDir;
	Vector3 m_camUp;
	double m_nearScreenHeight;
	double m_distToScreen;

  int m_checkIntersectionCount;
	
	Color *m_result;
};

}
