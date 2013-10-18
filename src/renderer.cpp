#include "color.h"
#include "scene.h"
#include "renderer.h"
#include "ray.h"
#include "random.h"

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

  // スクリーンx方向を張るベクトル
  Vector3 screen_x(m_camDir.cross(m_camUp));
  screen_x.normalize(); screen_x *= m_width;
  // スクリーンy方向を張るベクトル
  Vector3 screen_y(screen_x.cross(m_camDir));
  screen_y.normalize(); screen_y *= m_height;
  // スクリーン中心
  const Vector3 screen_center = m_camPos + m_camDir * m_distToScreen;

  // trace all pixels
  for (int y=0; y<m_height; y++) {
    Random rnd(y+1);
     for (int x=0; x<m_width; x++) {
       const int index = x + y*m_width;
       const double averaging_factor = m_samples * m_supersamples * m_supersamples;
       // super-sampling
       for (int sy=0; sy<m_supersamples; sy++) for (int sx=0; sx < m_supersamples; sx++) {
         // (x,y)ピクセル内での位置: [0,1]
         const double rx = (2.0*sx + 1.0)/(2*m_supersamples);
         const double ry = (2.0*sy + 1.0)/(2*m_supersamples);

         Vector3 target_position = screen_center +
           screen_x * ((x+rx)/m_width - 0.5) +
           screen_y * ((y+ry)/m_height - 0.5);
         Vector3 target_dir = target_position - m_camPos;
         target_dir.normalize();

         Color total_radiance;
         // (m_samples)回サンプリングする
         for (int s=0; s<m_samples; s++) {
           total_radiance += Radiance(Ray(m_camPos, target_dir), rnd, 0);
         }
         m_result[index] += total_radiance / averaging_factor;
       }
     }
  }
}

 Vector3 PathTracer::Radiance(const Ray &ray, Random &rnd, const int depth) {
   return Vector3(0.5,0.5,0.1);
 }
