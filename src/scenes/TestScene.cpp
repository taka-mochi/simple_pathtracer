
#include <iostream>
#include "TestScene.h"
#include "renderer/Sphere.h"
#include "renderer/Polygon.h"
#include "renderer/Model.h"
#include "renderer/BVH.h"

namespace SimpleRenderer {

TestScene::TestScene()
{
  AddObject(new Sphere(1e5, Vector3( 1e5+1, 40.8, 81.6),  Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.75, 0.25, 0.25))), true, false);  // 左
  AddObject(new Sphere(1e5, Vector3(-1e5+99, 40.8, 81.6), Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.25, 0.25, 0.75))), true, false);  // 右
  AddObject(new Sphere(1e5, Vector3(50, 40.8,  1e5),      Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.25, 0.75, 0.25))), true, false);  // 奥
  AddObject(new Sphere(1e5, Vector3(50, 40.8, -1e250),    Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color())), true, false);                  // 手前

  //addObject(new Sphere(1e5, Vector3( 1e5+1, 40.8, 81.6),  Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_SPECULAR));  // 左
  //addObject(new Sphere(1e5, Vector3(-1e5+99, 40.8, 81.6), Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_SPECULAR));  // 右
  //addObject(new Sphere(1e5, Vector3(50, 40.8,  1e5),      Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_SPECULAR));  // 奥
  //addObject(new Sphere(1e5, Vector3(50, 40.8, -1e250),    Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_SPECULAR));  // 手前
  
  AddObject(new Sphere(1e5, Vector3(50,  1e5,      81.6), Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.75, 0.75, 0.75))), true, false);  // 下
  AddObject(new Sphere(1e5, Vector3(50, -1e5+81.6, 81.6), Material(Material::REFLECTION_TYPE_LAMBERT, Color(), Color(0.75, 0.75, 0.75))), true, false);  // 上

  Model *cube = new Model;
  if (!cube->ReadFromObj("torii.obj")) {
    std::cerr << "failed to load cube.obj!!!" << std::endl;
    getchar();
    exit(-1);
  }
  cube->SetTransform(Vector3(50,20,80), Vector3(2,2,2));//, Matrix::rotateAroundVector(Vector3(0,1,0), 30.0/180*PI));
  AddModel(cube);
  
  //addObject(new Sphere(10,Vector3(),           Material(Material::REFLECTION_TYPE_LAMBERT,    Color(), Color(0.25, 0.75, 0.25))));    // 緑球
  //addObject(new Sphere(16.5,Vector3(27, 16.5, 47),       Material(Material::REFLECTION_TYPE_SPECULAR,   Color(), Color(0.99, 0.99, 0.99))));   // 鏡
  //addObject(new Sphere(16.5,Vector3(77, 16.5, 78),       Material(Material::REFLECTION_TYPE_REFRACTION, Color(), Color(0.99, 0.99, 0.99), REFRACTIVE_INDEX_OBJECT))); // ガラス


	AddObject(new Sphere(15.0,Vector3(50.0, 90.0, 81.6),   Material(Material::REFLECTION_TYPE_LAMBERT, Color(36,36,36), Color())), true, false);    // 照明

  ConstructBVH();
}

}
