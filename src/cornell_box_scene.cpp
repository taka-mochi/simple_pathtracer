
#include "cornell_box_scene.h"
#include "sphere.h"

CornellBoxScene::CornellBoxScene()
{
  addObject(new Sphere(1e5, Vector3( 1e5+1, 40.8, 81.6),  Color(), Color(0.75, 0.25, 0.25), SceneObject::REFLECTION_TYPE_LAMBERT));  // 左
  addObject(new Sphere(1e5, Vector3(-1e5+99, 40.8, 81.6), Color(), Color(0.25, 0.25, 0.75), SceneObject::REFLECTION_TYPE_LAMBERT));  // 右
  addObject(new Sphere(1e5, Vector3(50, 40.8,  1e5),      Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_LAMBERT));  // 奥
  addObject(new Sphere(1e5, Vector3(50, 40.8, -1e250),    Color(), Color(), SceneObject::REFLECTION_TYPE_LAMBERT));                  // 手前
  //addObject(new Sphere(1e5, Vector3( 1e5+1, 40.8, 81.6),  Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_SPECULAR));  // 左
  //addObject(new Sphere(1e5, Vector3(-1e5+99, 40.8, 81.6), Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_SPECULAR));  // 右
  //addObject(new Sphere(1e5, Vector3(50, 40.8,  1e5),      Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_SPECULAR));  // 奥
  //addObject(new Sphere(1e5, Vector3(50, 40.8, -1e250),    Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_SPECULAR));  // 手前
  addObject(new Sphere(1e5, Vector3(50,  1e5,      81.6), Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_LAMBERT));  // 下
  addObject(new Sphere(1e5, Vector3(50, -1e5+81.6, 81.6), Color(), Color(0.75, 0.75, 0.75), SceneObject::REFLECTION_TYPE_LAMBERT));  // 上

	addObject(new Sphere(20,Vector3(65, 20, 20),           Color(),      Color(0.25, 0.75, 0.25), SceneObject::REFLECTION_TYPE_LAMBERT));    // 緑球
  addObject(new Sphere(16.5,Vector3(27, 16.5, 47),       Color(),      Color(0.99, 0.99, 0.99), SceneObject::REFLECTION_TYPE_SPECULAR));   // 鏡
  addObject(new Sphere(16.5,Vector3(77, 16.5, 78),       Color(),      Color(0.99, 0.99, 0.99), SceneObject::REFLECTION_TYPE_REFRACTION)); // ガラス
	addObject(new Sphere(15.0,Vector3(50.0, 90.0, 81.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(45.0, 83.0, 76.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(45.0, 83.0, 81.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(45.0, 83.0, 86.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(45.0, 83.0, 79.1),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(45.0, 83.0, 84.1),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(50.0, 83.0, 76.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(50.0, 83.0, 81.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(50.0, 83.0, 86.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(50.0, 83.0, 84.1),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(50.0, 83.0, 79.1),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(55.0, 83.0, 76.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(55.0, 83.0, 81.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(55.0, 83.0, 86.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(55.0, 83.0, 79.1),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(55.0, 83.0, 84.1),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(47.5, 83.0, 76.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(47.5, 83.0, 81.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(47.5, 83.0, 86.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(47.5, 83.0, 79.1),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(47.5, 83.0, 84.1),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(52.5, 83.0, 76.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(52.5, 83.0, 81.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(52.5, 83.0, 86.6),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(52.5, 83.0, 79.1),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
  //addObject(new Sphere(3.0,Vector3(52.5, 83.0, 84.1),   Color(36,36,36), Color(), SceneObject::REFLECTION_TYPE_LAMBERT             ));    // 照明
}

CornellBoxScene::~CornellBoxScene() {
  for (size_t i=0; i<m_objects.size(); i++) {
    delete m_objects[i];
  }
  m_objects.clear();
}
