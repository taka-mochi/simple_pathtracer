
#include <iostream>
#include "cornell_box_scene.h"
#include "sphere.h"
#include "polygon.h"
#include "quad.h"
#include "box.h"

CornellBoxScene::CornellBoxScene()
{
  addObject(new Sphere(1e5, Vector3( 1e5+1, 40.8, 81.6),  Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.75, 0.25, 0.25))));  // ç∂
  addObject(new Sphere(1e5, Vector3(-1e5+99, 40.8, 81.6), Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.25, 0.25, 0.75))));  // âE
  addObject(new Sphere(1e5, Vector3(50, 40.8,  1e5),      Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.75, 0.75, 0.75))));  // âú
  addObject(new Sphere(1e5, Vector3(50, 40.8, -1e250),    Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color())));                  // éËëO
  addObject(new Sphere(1e5, Vector3(50,  1e5,      81.6), Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.75, 0.75, 0.75))));  // â∫
  addObject(new Sphere(1e5, Vector3(50, -1e5+81.6, 81.6), Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.75, 0.75, 0.75))));  // è„

  addObject(new Sphere(20,Vector3(65, 20, 20),           Material(Material::REFLECTION_TYPE_LAMBERT,    Color(), Color(0.25, 0.75, 0.25))));    // óŒãÖ
  addObject(new Sphere(16.5,Vector3(27, 16.5, 47),       Material(Material::REFLECTION_TYPE_SPECULAR,   Color(), Color(0.99, 0.99, 0.99))));   // ãæ
  addObject(new Sphere(16.5,Vector3(77, 16.5, 78),       Material(Material::REFLECTION_TYPE_REFRACTION, Color(), Color(0.99, 0.99, 0.99), REFRACTIVE_INDEX_OBJECT))); // ÉKÉâÉX
	addObject(new Sphere(15.0,Vector3(50.0, 90.0, 81.6),   Material(Material::REFLECTION_TYPE_LAMBERT, Color(36,36,36), Color())));    // è∆ñæ
}
