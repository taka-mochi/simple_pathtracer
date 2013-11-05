
#pragma once

#include <vector>
#include "scene.h"
#include "SceneObject.h"

namespace SimpleRenderer {

class BVH {
public:
  enum CONSTRUCTION_TYPE {
    CONSTRUCTION_OBJECT_MEDIAN,
    CONSTRUCTION_OBJECT_SAH,
  };

public:
  explicit BVH() : m_root(NULL), m_bvh_node_size(0) {}
  ~BVH();

  void Construct(const CONSTRUCTION_TYPE type, const std::vector<SceneObject *> &targets);
  bool CheckIntersection(const Ray &ray, Scene::IntersectionInformation &info) const;

private:
  void Construct_internal(const CONSTRUCTION_TYPE type, const std::vector<SceneObject *> &targets, int index);
  void MakeLeaf_internal(const std::vector<SceneObject *> &targets, int index);

private:
  class BVH_structure;
  BVH_structure *m_root;
  int m_bvh_node_size;
};

}
