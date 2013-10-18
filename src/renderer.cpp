#include <iostream>
#include "color.h"
#include "scene.h"
#include "renderer.h"
#include "ray.h"
#include "random.h"

using namespace std;

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

  double nearScreenHeight = m_nearScreenHeight;
  double nearScreenWidth = m_nearScreenHeight * m_width / m_height;

  // スクリーンx方向を張るベクトル
  Vector3 screen_x(m_camDir.cross(m_camUp));
  screen_x.normalize(); screen_x *= nearScreenWidth;
  // スクリーンy方向を張るベクトル
  Vector3 screen_y(screen_x.cross(m_camDir));
  screen_y.normalize(); screen_y *= nearScreenHeight;
  // スクリーン中心
  const Vector3 screen_center = m_camPos + m_camDir * m_distToScreen;

  const double averaging_factor = m_samples * m_supersamples * m_supersamples;

  // trace all pixels
  int processed_y_counts = 0;
#pragma omp parallel for
  for (int y=0; y<m_height; y++) {
    Random rnd(y+1);
    cerr << "y = " << y << endl; 
    for (int x=0; x<m_width; x++) {
      const int index = x + (m_height - y - 1)*m_width;
       
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
          total_radiance += Radiance(scene, Ray(m_camPos, target_dir), rnd, 0);
        }
        m_result[index] += total_radiance / averaging_factor;
      }
    }
    processed_y_counts++;
    cerr << static_cast<double>(processed_y_counts)/m_height*100 << "% finished" << endl;

  }
}

const static int MinDepth = 5;
const static int MaxDepth = 64;

Color PathTracer::Radiance(const Scene &scene, const Ray &ray, Random &rnd, const int depth) {
  Scene::IntersectionInformation intersect;

  if (!scene.CheckIntersection(ray, intersect)) {
    //std::cerr << "Hit to " << intersect.object << " distance = " << intersect.hit.distance << std::endl;
    return Vector3(0,0,0);
  }

  Vector3 normal = intersect.hit.normal.dot(ray.dir) < 0.0 ? intersect.hit.normal : intersect.hit.normal * -1.0;
  Color income;

  double russian_roulette_probability = std::max(intersect.object->color.x, std::max(intersect.object->color.y, intersect.object->color.z)); // 適当
  if (depth > MaxDepth) {
    russian_roulette_probability *= pow(0.5, depth-MaxDepth);
  }
  if (depth > MinDepth) {
    if (rnd.nextDouble() >= russian_roulette_probability) {
      return intersect.object->emission;
    }
  } else {
    russian_roulette_probability = 1.0; // no roulette
  }

   switch (intersect.object->reflection_type) {
   case SceneObject::REFLECTION_TYPE_LAMBERT:
     income = Radiance_Lambert(scene, ray, rnd, depth, intersect, normal, russian_roulette_probability);
  // lambertian



   }
  
  
//  }

  return income;
}

Color PathTracer::Radiance_Lambert(const Scene &scene, const Ray &ray, Random &rnd, const int depth, Scene::IntersectionInformation &intersect, const Vector3 &normal, double russian_roulette_prob) {
  Vector3 w,u,v;

  w = normal;
  if (fabs(normal.x) > EPS) {
   u = Vector3(0,1,0).cross(w);
  } else {
   u = Vector3(1,0,0).cross(w);
  }
  v = w.cross(u);
  double u1 = rnd.nextDouble(); double u2 = rnd.nextDouble();
  // pdf is 1/PI
  //double r1 = PI*u1; // Φ
  //double r2 = 1-u2; // cosθ
  //double r3 = sqrt(1-r2*r2); // sinθ
  //double pdf = 1/PI;

  // pdf is cosθ/PI
  double r1 = 2*PI*u1;
  double r2 = sqrt(u2); // cosθ
  double r3 = sqrt(1-u2); // sinθ
  double pdf = r2/PI;

  //const double r1 = 2 * PI * rnd.nextDouble();
  //const double r2 = rnd.nextDouble(), r2s = sqrt(r2);
  //Vector3 dir = u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1.0-r2);
  //dir.normalize();
  //weight = intersect.object->color / russian_roulette_probability;

  Vector3 dir = u*r3*cos(r1) + v*r3*sin(r1) + w*r2;
  dir.normalize();

  Color weight = intersect.object->color / PI * r2 / pdf / russian_roulette_prob;
  Color income = Radiance(scene, Ray(intersect.hit.position, dir), rnd, depth+1);
  return intersect.object->emission + Vector3(weight.x*income.x, weight.y*income.y, weight.z*income.z);
}
