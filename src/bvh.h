
#pragma once

#include <vector>
#include "scene.h"
#include "SceneObject.h"

class BVH {
public:
  enum CONSTRUCTION_TYPE {
    OBJECT_MEDIAN,
    //OBJECT_SURFACE,
    //OBJECT_VOLUME,
  };

public:
  explicit BVH() : m_root(NULL) {}
  ~BVH();

  void Construct(const CONSTRUCTION_TYPE type, const std::vector<SceneObject *> &targets);
  bool CheckIntersection(const Ray &ray, Scene::IntersectionInformation &info) const;

private:
  void Construct_internal(const CONSTRUCTION_TYPE type, const std::vector<SceneObject *> &targets, int index);
  void MakeLeaf_internal(const std::vector<SceneObject *> &targets, int index);

private:
  class BVH_structure;
  BVH_structure *m_root;
};
