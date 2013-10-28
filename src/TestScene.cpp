
#include <iostream>
#include "test_scene.h"
#include "sphere.h"
#include "polygon.h"
#include "model.h"
#include "bvh.h"


TestScene::TestScene()
{
  addObject(new Sphere(1e5, Vector3( 1e5+1, 40.8, 81.6),  Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.75, 0.25, 0.25))), true, false);  // 左
  addObject(new Sphere(1e5, Vector3(-1e5+99, 40.8, 81.6), Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.25, 0.25, 0.75))), true, false);  // 右
  addObject(new Sphere(1e5, Vector3(50, 40.8,  1e5),      Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.25, 0.75, 0.25))), true, false);  // 奥
  addObject(new Sphere(1e5, Vector3(50, 40.8, -1e250),    Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color())), true, false);                  // 手前

  //addObject(new Sphere(1e5, Vector3( 1e5+1, 40.8, 81.6),  Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_SPECULAR));  // 左
  //addObject(new Sphere(1e5, Vector3(-1e5+99, 40.8, 81.6), Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_SPECULAR));  // 右
  //addObject(new Sphere(1e5, Vector3(50, 40.8,  1e5),      Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_SPECULAR));  // 奥
  //addObject(new Sphere(1e5, Vector3(50, 40.8, -1e250),    Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_SPECULAR));  // 手前
  
  addObject(new Sphere(1e5, Vector3(50,  1e5,      81.6), Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.75, 0.75, 0.75))), true, false);  // 下
  addObject(new Sphere(1e5, Vector3(50, -1e5+81.6, 81.6), Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.75, 0.75, 0.75))), true, false);  // 上

  Model *cube = new Model;
  if (!cube->readFromObj("torii.obj")) {
    std::cerr << "failed to load cube.obj!!!" << std::endl;
    getchar();
    exit(-1);
  }
  cube->setTransform(Vector3(50,10,80), Vector3(1.5,1.5,1.5), Matrix::rotateAroundVector(Vector3(0,1,0), 30.0/180*PI));
  addModel(cube);
  
  //addObject(new Sphere(10,Vector3(),           Material(Material::REFLECTION_TYPE_LAMBERT,    Color(), Color(0.25, 0.75, 0.25))));    // 緑球
  //addObject(new Sphere(16.5,Vector3(27, 16.5, 47),       Material(Material::REFLECTION_TYPE_SPECULAR,   Color(), Color(0.99, 0.99, 0.99))));   // 鏡
  //addObject(new Sphere(16.5,Vector3(77, 16.5, 78),       Material(Material::REFLECTION_TYPE_REFRACTION, Color(), Color(0.99, 0.99, 0.99), REFRACTIVE_INDEX_OBJECT))); // ガラス


	addObject(new Sphere(15.0,Vector3(50.0, 90.0, 81.6),   Material(Material::REFLECTION_TYPE_LAMBERT, Color(36,36,36), Color())), true, false);    // 照明

  ConstructBVH();
}
