#pragma once

#include <vector>
#include "scenes/Scene.h"

namespace SimpleRenderer {
  class SceneObject;
  class Ray;
  class Scene::IntersectionInformation;

  class QBVH {
  public:
    explicit QBVH() : m_root(NULL) {}
    ~QBVH();

    void Construct(const std::vector<SceneObject *> &targets);
    bool CheckIntersection(const Ray &ray, Scene::IntersectionInformation &info) const;

  private:
    void Construct_internal(const std::vector<SceneObject *> &targets, int index);
    void MakeLeaf_internal(const std::vector<SceneObject *> &targets, int index);

  private:
    struct QBVH_structure;
    QBVH_structure *m_root;

  };
}
